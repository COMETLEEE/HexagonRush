#include "pch.h"
#include "ResourceManager.h"
//#include "GraphicsManager.h"
#include <fstream>
#include "Prefab_ASE.h"
#include "StringHelper.h"


namespace Muscle
{
	using namespace GraphicsEngine;

	std::shared_ptr<ResourceManager> ResourceManager::_resourceManager = nullptr;

	ResourceManager::~ResourceManager()
	{
		Release();
	}
	std::shared_ptr<Prefab_ASE> ResourceManager::BuildPrefabFromASE(ASEParsingData& aseData, const bool isSmooth, const bool isStatic)
	{
		std::vector<std::shared_ptr<NodeObjectBase>> rootNodes;
		std::map<tstring, std::shared_ptr<NodeObjectBase>> nodeMap;

		std::vector<uint64> meshIDVector(aseData.geoObjectList.size());
		std::vector<uint32> materialRefVector(aseData.geoObjectList.size());
		std::vector<std::shared_ptr<NodeMaterial>> materialVector;

		// 노드 머터리얼 정보 (빛의 반사도 및 사용하는 맵의 ID) 를 생성합니다.
		for (int i = 0; i < static_cast<int>(aseData.materialList.materialCount); i++)
		{
			std::shared_ptr<NodeMaterial> newMat = std::make_shared<NodeMaterial>();

			tstring diffuseFileName = StringHelper::ToTString(aseData.materialList.materialList[i].mapDiffuse.bitmap);

			tstring normalFileName = StringHelper::ToTString(aseData.materialList.materialList[i].mapNormal.bitmap);

			if (diffuseFileName != TEXT("") && diffuseFileName != TEXT("None"))
			{
				tstring diffuseBitmapPath = (_resourcePath + TEXT("Texture\\")) + diffuseFileName;
				uint64 diffuseMapID = Graphics_Interface::Get()->LoadTexture(diffuseFileName, diffuseBitmapPath);
				newMat->_diffuseMapID = diffuseMapID;
			}
			else
				newMat->_diffuseMapID = ULLONG_MAX;

			if (normalFileName != TEXT("") && normalFileName != TEXT("None"))
			{
				tstring normalBitmapPath = (_resourcePath + TEXT("Texture\\")) + normalFileName;
				uint64 normalMapID = Graphics_Interface::Get()->LoadTexture(normalFileName, normalBitmapPath);
				newMat->_normalMapID = normalMapID;
			}
			else
				newMat->_normalMapID = ULLONG_MAX;

			newMat->_ambient = aseData.materialList.materialList[i].ambient;
			newMat->_diffuse = aseData.materialList.materialList[i].diffuse;
			newMat->_specular = aseData.materialList.materialList[i].specular;
			newMat->_reflect = Vector4(0.3f, 0.3f, 0.3f, 1.f);

			materialVector.push_back(newMat);
		}

		// Geometry Object => Mesh 생성
		for (int geoObjIndex = 0; geoObjIndex < aseData.geoObjectList.size(); geoObjIndex++)
		{
			const tstring nodeName = StringHelper::ToTString(aseData.geoObjectList[geoObjIndex].nodeName);
			const tstring nodeParent = StringHelper::ToTString(aseData.geoObjectList[geoObjIndex].nodeParent);
			ASEParsingData::NodeTM nodeTM = aseData.geoObjectList[geoObjIndex].nodeTM;
			const int& materialRef = aseData.geoObjectList[geoObjIndex].materialRef;
			const auto& tmAnimations = aseData.geoObjectList[geoObjIndex].tmAnimation;
			const auto& boneList = aseData.geoObjectList[geoObjIndex].boneList;
			const auto& wVertexs = aseData.geoObjectList[geoObjIndex].wVertexs;

			auto vertexList = aseData.geoObjectList[geoObjIndex].mesh.vertexList;
			auto tVertexList = aseData.geoObjectList[geoObjIndex].mesh.tVertexList;
			const auto& faceNormals = aseData.geoObjectList[geoObjIndex].mesh.faceNormals;
			const auto& faceList = aseData.geoObjectList[geoObjIndex].mesh.faceList;
			const auto& tFaceList = aseData.geoObjectList[geoObjIndex].mesh.tFaceList;



			std::vector<SkinnedMeshVertex> vertexVector;
			std::vector<uint32> indexVector(faceList.size() * 3);

			// Key tuple : vIndex, vtIndex, vNormal.x, vNormal.y, vNormal.z => 버텍스들을 순회하면서 같은 녀석이 이미 만들어졌는지 확인.
			std::map<std::tuple<unsigned, unsigned, float, float, float>, unsigned> indexMap;

			Matrix nodeTMInv = nodeTM.tm.Invert();

			// Blend UV 좌표계로 변경
			for (int i = 0; i < tVertexList.size(); i++)
			{
				tVertexList[i].v = 1.f - tVertexList[i].v;
			}

			for (int i = 0; i < faceList.size(); i++)
			{
				const auto& face = faceList[i];
				ASEParsingData::Mesh::TFace tFace;
				ASEParsingData::Mesh::FaceNormal faceNormal;

				bool bUseTexture = false;
				bool bUseNormal = false;

				if (tFaceList.size() > i)
				{
					tFace = tFaceList[i];
					bUseTexture = true;
				}

				if (faceNormals.size() > i)
				{
					faceNormal = faceNormals[i];
					bUseNormal = true;
				}

				if (bUseNormal && nodeTM.bHasNegativeScale)
				{
					// 이 geomObject의 nodeTM의 scale이 negative scale이였다면
					// 노멀도 역시 거꾸로 뒤집어주어야 합니다.
					faceNormal.faceNormal = faceNormal.faceNormal * -1.f;
					faceNormal.vertexNormal[0].normal = faceNormal.vertexNormal[0].normal * -1.f;
					faceNormal.vertexNormal[1].normal = faceNormal.vertexNormal[1].normal * -1.f;
					faceNormal.vertexNormal[2].normal = faceNormal.vertexNormal[2].normal * -1.f;
				}

				// 페이스의 정점을 순회하면서 중복된 것은 추가하지 않고, 하나씩 버텍스를 만든다.
				for (int j = 0; j < 3; j++)
				{
					const int vertexIndex = face.vertex[j];
					const int tVertexIndex = bUseTexture ? tFace.vertexIndex[j] : -1;

					Vector3 vertexNormal;

					if (isSmooth)
						vertexNormal = bUseNormal ? faceNormal.vertexNormal[j].normal : Vector3(-1.f, -1.f, -1.f);
					else
						vertexNormal = bUseNormal ? faceNormal.faceNormal : Vector3(-1.f, -1.f, -1.f);


					const auto indexPair = std::make_tuple(vertexIndex, tVertexIndex, vertexNormal.x, vertexNormal.y, vertexNormal.z);

					const auto iter = indexMap.find(indexPair);

					if (iter == indexMap.end())
					{
						// 새로운 버텍스의 조합이다 => 만들어야할 필요가 있다.
						const Vector3 position = vertexList[vertexIndex].position;

						const Vector2 uv = bUseTexture ? Vector2{ tVertexList[tVertexIndex].u, tVertexList[tVertexIndex].v } : Vector2(-1.f, -1.f);

						SkinnedMeshVertex newVertex(position, uv, vertexNormal);

						// 뼈가 있다.
						if (!isStatic)
						{
							const auto wVertex = wVertexs[vertexIndex];

							// 뼈값을 준다.
							for (int bWIndex = 0; bWIndex < wVertex.boneBlendingWeight.size(); bWIndex++)
							{
								newVertex._boneIndex[bWIndex] = wVertex.boneBlendingWeight[bWIndex].first;
								newVertex._boneWeight[bWIndex] = wVertex.boneBlendingWeight[bWIndex].second;

							}
						}

						vertexVector.push_back(newVertex);
						const uint32 newVertexIndex = static_cast<uint32>(vertexVector.size() - 1);
						UINT index = i * 3 + j;
						indexVector[index] = newVertexIndex;
						indexMap.insert({ indexPair, newVertexIndex });
					}
					else
					{
						// 이미 같은 위치, 노말, 텍스쳐 성분을 가지는 버텍스가 있으니 현재 인덱스 순서가 이 버텍스를 가르키도록 한다.
						UINT index = i * 3 + j;
						indexVector[index] = iter->second;
					}
				}
			}

			// Vertex => Tangent Vector
			for (size_t i = 0; i < indexVector.size(); i += 3)
			{
				int _i0 = indexVector[static_cast<size_t>(i)];
				int _i1 = indexVector[static_cast<size_t>(i + 1)];
				int _i2 = indexVector[static_cast<size_t>(i + 2)];

				Vector3 e1 = vertexVector[_i1]._position - vertexVector[_i0]._position;
				Vector3 e2 = vertexVector[_i2]._position - vertexVector[_i0]._position;

				float x1 = vertexVector[_i1]._uv.x - vertexVector[_i0]._uv.x;
				float x2 = vertexVector[_i2]._uv.x - vertexVector[_i0]._uv.x;

				float y1 = vertexVector[_i1]._uv.y - vertexVector[_i0]._uv.y;
				float y2 = vertexVector[_i2]._uv.y - vertexVector[_i0]._uv.y;

				float temp = x1 * y2 - x2 * y1;

				// 예외처리 (플로팅 연산 오차때문에 0이 되는 경우가 분명히 있다.)
				if (temp == 0.f)
					temp = 0.000001f;

				float r = 1.0f / temp;

				// Tangent Vector
				Vector3 tangent = (e1 * y2 - e2 * y1) * r;

				vertexVector[_i0]._tangent += tangent;
				vertexVector[_i1]._tangent += tangent;
				vertexVector[_i2]._tangent += tangent;
			}

			for (int i = 0; i < vertexVector.size(); i++)
			{
				vertexVector[i]._tangent.Normalize();

				(vertexVector[i]._tangent - vertexVector[i]._normal * vertexVector[i]._tangent.Dot(vertexVector[i]._normal)).Normalize(vertexVector[i]._tangent);
			}

			// Optimizing 및 버텍스 벡터, 인덱스 벡터 구축 완료
			uint64 nodesMeshID;

			std::vector<StaticMeshVertex> staticVertexVector(vertexVector.size());

			for (int i = 0; i < staticVertexVector.size(); i++)
				staticVertexVector[i] = vertexVector[i];

			if (!isStatic)
				nodesMeshID = Graphics_Interface::Get()->LoadMesh(nodeName, vertexVector, indexVector);
			else
				nodesMeshID = Graphics_Interface::Get()->LoadMesh(nodeName, staticVertexVector, indexVector);

			CreateBoundingCube(staticVertexVector, nodesMeshID);

			meshIDVector[geoObjIndex] = nodesMeshID;
			materialRefVector[geoObjIndex] = materialRef;

			auto newNodeObject = std::make_shared<NodeGeoObject>();
			newNodeObject->_nodeName = nodeName;
			newNodeObject->_nodeParent = nodeParent;
			newNodeObject->_nodeTM = nodeTM;
			newNodeObject->_meshID = nodesMeshID;
			newNodeObject->_tmAnimations = tmAnimations;
			newNodeObject->_materialRef = materialRef;



			if (!isStatic)
			{
				// 애니메이션 객체인데 버텍스가 6개 이하이면 헬퍼 or 본이다.
				aseData.geoObjectList[geoObjIndex].boneList;
				bool isBone = false;
				if (vertexList.size() <= 8)
				{
					for (auto& iter : aseData.geoObjectList)
					{
						if (isBone)
							break;
						for (auto& iter2 : iter.boneList)
						{
							if (aseData.geoObjectList[geoObjIndex].nodeName == iter2);
							{
								isBone = true;
								break;
							}
						}
					}

					if (!isBone)
						continue;
				}
					newNodeObject->_isBone = isBone;
			}



			for (int i = 0; i < boneList.size(); i++)
				newNodeObject->_boneNameList.push_back(StringHelper::ToTString(boneList[i]));

			newNodeObject->_wVertexs = wVertexs;
			nodeMap.insert({ nodeName, newNodeObject });

			_meshIDs.insert({ nodeName, nodesMeshID });
		}

		// Helper Object 생성
		for (int i = 0; i < aseData.helperObjectList.size(); i++)
		{
			const auto& helperObject = aseData.helperObjectList[i];

			const tstring& nodeName = StringHelper::ToTString(helperObject.nodeName);
			const tstring& nodeParent = StringHelper::ToTString(helperObject.nodeParent);
			const ASEParsingData::NodeTM& nodeTM = helperObject.nodeTM;
			const auto& tmAnimations = helperObject._tmAnimations;

			auto newHelperObject = std::make_shared<NodeHelperObject>();
			newHelperObject->_nodeName = nodeName;
			newHelperObject->_nodeParent = nodeParent;
			newHelperObject->_nodeTM = nodeTM;
			newHelperObject->_tmAnimations = tmAnimations;
			nodeMap.insert({ nodeName, newHelperObject });
		}

		// Shape Object 생성
		for (int i = 0; i < aseData.shapeObjectList.size(); i++)
		{
			const auto& shapeObject = aseData.shapeObjectList[i];

			const tstring& nodeName = StringHelper::ToTString(shapeObject.nodeName);
			const tstring& nodeParent = StringHelper::ToTString(shapeObject.nodeParent);
			const ASEParsingData::NodeTM& nodeTM = shapeObject.nodeTM;
			const auto& tmAnimations = shapeObject._tmAnimations;

			auto newShapeObject = std::make_shared<NodeShapeObject>();
			newShapeObject->_nodeName = nodeName;
			newShapeObject->_nodeParent = nodeParent;
			newShapeObject->_nodeTM = nodeTM;
			newShapeObject->_tmAnimations = tmAnimations;
			nodeMap.insert({ nodeName, newShapeObject });
		}

		// 노드간의 계층 관계를 설정합니다.
		for (auto& node : nodeMap)
		{
			const tstring parentNodeName = node.second->_nodeParent;

			// 부모가 없는 노드. 즉, 최상위 Root Node
			if (parentNodeName.empty())
			{
				rootNodes.push_back(node.second);
				node.second->_localTM = node.second->_nodeTM.tm;

				continue;
			}

			// 부모가 있다. 찾아서 연결해주자.
			const auto iter = nodeMap.find(parentNodeName);

			if (iter != nodeMap.end())
			{
				std::shared_ptr<NodeObjectBase> parentNode = iter->second;

				node.second->_parentNode = parentNode;
				parentNode->_childNodes.push_back(node.second);

				Matrix parentWorldTMInv = parentNode->_nodeTM.tm.Invert();

				node.second->_localTM = node.second->_nodeTM.tm * parentWorldTMInv;
			}
			else
			{
				// 부모가 있지만 찾지 못 함. 파일이 잘 못 되어있는 경우이므로 예외를 던진다.
				assert(false);

				continue;
			}
		}

		std::shared_ptr<Prefab_ASE> outPrefab = std::make_shared<Prefab_ASE>(rootNodes, nodeMap, materialVector);

		return outPrefab;
	}

	void ResourceManager::CreateBoundingCube(const std::vector<StaticMeshVertex>& vertexVector, const uint64& meshID)
	{
		float minX = 2000000.f;
		float maxX = -2000000.f;
		float minY = 2000000.f;
		float maxY = -2000000.f;
		float minZ = 2000000.f;
		float maxZ = -2000000.f;

		for (auto& vertex : vertexVector)
		{
			Vector3 pos = vertex._position;

			float x = pos.x;
			float y = pos.y;
			float z = pos.z;

			maxX = std::max<float>(maxX, x);

			minX = std::min<float>(minX, x);

			maxY = std::max<float>(maxY, y);

			minY = std::min<float>(minY, y);

			maxZ = std::max<float>(maxZ, z);

			minZ = std::min<float>(minZ, z);
		}

		Vector3 localCenter = Vector3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);

		Vector3 localScale = Vector3(std::abs(maxX - minX), std::abs(maxY - minY), std::abs(maxZ - minZ));

		Matrix localMatrix = Matrix::CreateScale(localScale) * Matrix::CreateTranslation(localCenter);

		_boundingCubeInfos.insert({ meshID, std::make_shared<BoundingCube>(localScale, localCenter, localMatrix) });
	}

	void ResourceManager::CreateBasicMeshes()
	{
		CreateCubeMesh(Vector4(1.f, 1.f, 1.f, 1.f), TEXT("WhiteCube"));				// 4번

		CreateCubeMesh(Vector4(0.f, 1.f, 0.2f, 1.f), TEXT("GreenCube"));			// 5번

		CreateCubeMesh(Vector4(1.f, 0.f, 0.f, 1.f), TEXT("RedCube"));				// 6번

		CreateSphereMesh(Vector4(0.f, 1.f, 0.2f, 1.f), TEXT("GreenSphere"));		// 7번

		CreateSphereMesh(Vector4(1.f, 0.f, 0.f, 1.f), TEXT("RedSphere"));			// 8번

		CreateCylindereMesh(Vector4(0.f, 1.f, 0.2f, 1.f), TEXT("GreenCylinder"));		// 9번

		CreateCylindereMesh(Vector4(1.f, 0.f, 0.f, 1.f), TEXT("RedCylinder"));			// 10번

		CreateHalfSphereMesh(Vector4(0.f, 1.f, 0.2f, 1.f), TEXT("GreenHalfSphere"));		// 11번

		CreateHalfSphereMesh(Vector4(1.f, 0.f, 0.f, 1.f), TEXT("RedHalfSphere"));			// 12번

		CreateAxisMesh();

		CreateGridMesh();

		CreateSkullMesh();
	}

	void ResourceManager::CreateCubeMesh(Vector4 color, tstring meshName)
	{
		float x = 0.5f;
		float y = 0.5f;
		float z = 0.5f;

		std::vector<StaticMeshVertex> vertices(24);

		// 앞면
		vertices[0] = StaticMeshVertex(Vector3(-x, -y, -z), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[1] = StaticMeshVertex(Vector3(-x, +y, -z), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[2] = StaticMeshVertex(Vector3(+x, +y, -z), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[3] = StaticMeshVertex(Vector3(+x, -y, -z), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), color);

		// 뒷면
		vertices[4] = StaticMeshVertex(Vector3(-x, -y, +z), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[5] = StaticMeshVertex(Vector3(+x, -y, +z), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[6] = StaticMeshVertex(Vector3(+x, +y, +z), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[7] = StaticMeshVertex(Vector3(-x, +y, +z), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), color);

		// 윗면
		vertices[8] = StaticMeshVertex(Vector3(-x, +y, -z), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[9] = StaticMeshVertex(Vector3(-x, +y, +z), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[10] = StaticMeshVertex(Vector3(+x, +y, +z), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), color);
		vertices[11] = StaticMeshVertex(Vector3(+x, +y, -z), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), color);

		// 아랫면
		vertices[12] = StaticMeshVertex(Vector3(-x, -y, -z), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[13] = StaticMeshVertex(Vector3(+x, -y, -z), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[14] = StaticMeshVertex(Vector3(+x, -y, +z), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), color);
		vertices[15] = StaticMeshVertex(Vector3(-x, -y, +z), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), color);

		// 왼쪽면
		vertices[16] = StaticMeshVertex(Vector3(-x, -y, +z), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), color);
		vertices[17] = StaticMeshVertex(Vector3(-x, +y, +z), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), color);
		vertices[18] = StaticMeshVertex(Vector3(-x, +y, -z), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), color);
		vertices[19] = StaticMeshVertex(Vector3(-x, -y, -z), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), color);

		// 오른쪽면
		vertices[20] = StaticMeshVertex(Vector3(+x, -y, -z), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), color);
		vertices[21] = StaticMeshVertex(Vector3(+x, +y, -z), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), color);
		vertices[22] = StaticMeshVertex(Vector3(+x, +y, +z), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), color);
		vertices[23] = StaticMeshVertex(Vector3(+x, -y, +z), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), color);

		std::vector<unsigned int> idx(36);

		// 앞면
		idx[0] = 0; idx[1] = 1; idx[2] = 2;
		idx[3] = 0; idx[4] = 2; idx[5] = 3;

		// 뒷면
		idx[6] = 4; idx[7] = 5; idx[8] = 6;
		idx[9] = 4; idx[10] = 6; idx[11] = 7;

		// 윗면
		idx[12] = 8; idx[13] = 9; idx[14] = 10;
		idx[15] = 8; idx[16] = 10; idx[17] = 11;

		// 아랫면
		idx[18] = 12; idx[19] = 13; idx[20] = 14;
		idx[21] = 12; idx[22] = 14; idx[23] = 15;

		// 왼쪽면
		idx[24] = 16; idx[25] = 17; idx[26] = 18;
		idx[27] = 16; idx[28] = 18; idx[29] = 19;

		// 오른쪽면
		idx[30] = 20; idx[31] = 21; idx[32] = 22;
		idx[33] = 20; idx[34] = 22; idx[35] = 23;

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(meshName, vertices, idx);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ meshName, meshID });
	}

	void ResourceManager::CreateSphereMesh(Vector4 color, tstring meshName)
	{
		float radius = 1.f;

		std::vector<StaticMeshVertex> vertices;

		std::vector<uint32> indices;

		StaticMeshVertex topVertex;
		StaticMeshVertex bottomVertex;

		topVertex._color = color;
		bottomVertex._color = color;
		topVertex._position = Vector3(0.f, radius, 0.f);
		bottomVertex._position = Vector3(0.f, -radius, 0.f);

		vertices.push_back(topVertex);

		uint32 stackCount = 20;
		uint32 sliceCount = 20;

		float phiStep = PI / stackCount;
		float thetaStep = 2.0f * PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				StaticMeshVertex v;

				// spherical to cartesian
				v._position.x = radius * sinf(phi) * cosf(theta);
				v._position.y = radius * cosf(phi);
				v._position.z = radius * sinf(phi) * sinf(theta);

				v._color = color;

				v._uv.x = theta / (2 * PI);
				v._uv.y = phi / PI;

				vertices.push_back(v);
			}
		}

		vertices.push_back(bottomVertex);

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(meshName, vertices, indices);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ meshName, meshID });
	}

	void ResourceManager::CreateHalfSphereMesh(Vector4 color, tstring meshName)
	{
		float radius = 1.f;

		std::vector<StaticMeshVertex> vertices;

		std::vector<uint32> indices;

		StaticMeshVertex topVertex;

		topVertex._color = color;
		topVertex._position = Vector3(0.f, radius, 0.f);

		vertices.push_back(topVertex);

		uint32 stackCount = 50;
		uint32 sliceCount = 50;

		float phiStep = PI / stackCount;
		float thetaStep = 2.0f * PI / sliceCount;
		// Compute vertices for each stack ring (do not count the poles as rings).

		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				StaticMeshVertex v;

				// spherical to cartesian
				v._position.x = radius * sinf(phi) * cosf(theta);
				v._position.y = abs(radius * cosf(phi));
				v._position.z = radius * sinf(phi) * sinf(theta);

				v._color = color;

				v._uv.x = theta / (2 * PI);
				v._uv.y = phi / PI;

				vertices.push_back(v);
			}
		}


		for (UINT i = 1; i <= sliceCount; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(meshName, vertices, indices);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ meshName, meshID });
	}

	void ResourceManager::CreateCylindereMesh(Vector4 color, tstring meshName)
	{

		std::vector<StaticMeshVertex> vertices;
		std::vector<uint32> indices;

		//
		// Build Stacks.
		// 
		float height = 1;
		float topRadius = 1.0f;
		float bottomRadius = 1.0f;
		UINT sliceCount = 16;
		UINT stackCount = 16;
		float stackHeight = height / stackCount;
		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		UINT ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (UINT i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;

			// vertices of ring
			float dTheta = 2.0f * XM_PI / sliceCount;
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				StaticMeshVertex vertex;
				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				vertex._position = XMFLOAT3(r * c, y, r * s);
				vertex._color = color;
				vertex._uv.x = (float)j / sliceCount;
				vertex._uv.y = 1.0f - (float)i / stackCount;

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				vertex._tangent = XMFLOAT3(-s, 0.0f, c);

				float dr = bottomRadius - topRadius;
				XMFLOAT3 bitangent(dr * c, -height, dr * s);

				XMVECTOR T = XMLoadFloat3(&vertex._tangent);
				XMVECTOR B = XMLoadFloat3(&bitangent);
				XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&vertex._normal, N);

				vertices.push_back(vertex);
			}
		}

		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		UINT ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (UINT i = 0; i < stackCount; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(i * ringVertexCount + j);
				indices.push_back((i + 1) * ringVertexCount + j);
				indices.push_back((i + 1) * ringVertexCount + j + 1);

				indices.push_back(i * ringVertexCount + j);
				indices.push_back((i + 1) * ringVertexCount + j + 1);
				indices.push_back(i * ringVertexCount + j + 1);
			}
		}

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(meshName, vertices, indices);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ meshName, meshID });
	}

	void ResourceManager::CreateAxisMesh()
	{
		std::vector<StaticMeshVertex> vertices(6);

		vertices[0]._position = Vector3(0.f, 0.f, 0.f);
		vertices[0]._color = Vector4(1, 0, 0, 1);

		vertices[1]._position = Vector3(1000.f, 0.f, 0.f);
		vertices[1]._color = Vector4(1, 0, 0, 1);

		vertices[2]._position = Vector3(0.f, 0.f, 0.f);
		vertices[2]._color = Vector4(0, 1, 0, 1);

		vertices[3]._position = Vector3(0.f, 1000.f, 0.f);
		vertices[3]._color = Vector4(0, 1, 0, 1);

		vertices[4]._position = Vector3(0.f, 0.f, 0.f);
		vertices[4]._color = Vector4(0, 0, 1, 1);

		vertices[5]._position = Vector3(0.f, 0.f, 1000.f);
		vertices[5]._color = Vector4(0, 0, 1, 1);

		std::vector<unsigned int> indices(6);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(TEXT("Axis"), vertices, indices, false, true);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ TEXT("Axis"), meshID });
	}

	void ResourceManager::CreateGridMesh()
	{
		std::vector<StaticMeshVertex> _vertices(100);

		Vector4 _color = Vector4(1.f, 1.f, 1.f, 1.f);

		for (int i = 0; i < 100; i++)
		{
			_vertices[i]._position = Vector3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
			_vertices[i]._color = Vector4(_color.x, _color.y, _color.z, _color.w);	// (플로랄 화이트)
		}

		// 인덱스 버퍼를 생성한다.
		// 역시 100개의 라인을 나타내도록 했다.
		std::vector<uint32> _indices(40);

		for (uint32 i = 0; i < 10; i++)
		{
			_indices[i * 2] = i;
			_indices[i * 2 + 1] = i + 90;
		}

		for (uint32 i = 0; i < 10; i++)
		{
			_indices[20 + (i * 2)] = i * 10;
			_indices[20 + (i * 2) + 1] = i * 10 + 9;
		}

		uint64 meshID = Graphics_Interface::Get()->LoadMesh(TEXT("Grid"), _vertices, _indices, false, true);

		CreateBoundingCube(_vertices, meshID);

		_meshIDs.insert({ TEXT("Grid"), meshID });
	}

	void ResourceManager::CreateSkullMesh()
	{
		std::ifstream fin("Resource\\DragonBookMesh\\skull.txt");

		if (!fin)
		{
			MessageBox(0, L"Skull.txt is not found.", 0, 0);
			return;
		}

		UINT vcount = 0;
		UINT tcount = 0;
		std::string ignore;

		fin >> ignore >> vcount;
		fin >> ignore >> tcount;
		fin >> ignore >> ignore >> ignore >> ignore;

		std::vector<StaticMeshVertex> vertices(vcount);
		for (UINT i = 0; i < vcount; ++i)
		{
			fin >> vertices[i]._position.x >> vertices[i]._position.y >> vertices[i]._position.z;
			fin >> vertices[i]._normal.x >> vertices[i]._normal.y >> vertices[i]._normal.z;
		}

		fin >> ignore;
		fin >> ignore;
		fin >> ignore;

		std::vector<UINT> indices(3 * tcount);
		for (UINT i = 0; i < tcount; ++i)
			fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];

		fin.close();

		auto meshID = Graphics_Interface::Get()->LoadMesh(TEXT("Skull"), vertices, indices);

		CreateBoundingCube(vertices, meshID);

		_meshIDs.insert({ TEXT("Skull"), meshID });
	}

	void ResourceManager::CreateBasicFonts()
	{
		uint64 fontID1 = Graphics_Interface::Get()->LoadFont(TEXT("Consolas"), _resourcePath + TEXT("Font\\Consolas10.spritefont"));

		uint64 fontID2 = Graphics_Interface::Get()->LoadFont(TEXT("Segoe"), _resourcePath + TEXT("Font\\SegoeUI_18.spritefont"));

		_fontIDs.insert({ TEXT("Consolas"), fontID1 });
		_fontIDs.insert({ TEXT("Segoe"), fontID2 });
	}

	std::shared_ptr<ResourceManager> ResourceManager::Get()
	{
		if (_resourceManager == nullptr)
			_resourceManager = std::make_shared<ResourceManager>();

		return _resourceManager;
	}

	void ResourceManager::Initialize()
	{
		// File Path
		_resourcePath = TEXT("Resource\\");

		// 로더 준비
		_loader = ::Loader_Interface::Get();

		_loader->Initialize();

		CreateBasicMeshes();

		CreateBasicFonts();
	}

	void ResourceManager::Update()
	{
		// Reference Counting ..? Resource Load 및 Unload
	}

	void ResourceManager::Release()
	{
		_loader->Release();
	}

	void ResourceManager::ResetTexture()
	{
		Muscle::MuscleEngine::Get()->GetGraphicsManager()->TextureRelease();
		_textureIDs.clear();
	}

	void ResourceManager::LoadEnvironmentCubeMap(const tstring& textureName, const tstring& path)
	{
		// Environment Cube Map Load ..! => Irradiance, Specular Pre-Filtered, Specular BRDF LUT 생성하고 묶음 처리
		const uint64& id = Graphics_Interface::Get()->LoadEnvironmentCubeMap(textureName, path);

		_textureIDs.insert({ textureName, id });
	}

	void ResourceManager::LoadEnvironmentPanoramaMap(const tstring& textureName, const tstring& path)
	{
		const uint64& id = Graphics_Interface::Get()->LoadEnvironmentPanoramaMap(textureName, path);

		_textureIDs.insert({ textureName, id });
	}

	void ResourceManager::LoadTexture(const tstring& textureName, const tstring& path)
	{
		const uint64& id = Graphics_Interface::Get()->LoadTexture(textureName, path);

		_textureIDs.insert({ textureName, id });
	}

	const uint64 ResourceManager::GetTextureID(const tstring& textureName)
	{
		return _textureIDs.find(textureName) != _textureIDs.end() ? _textureIDs.at(textureName) : ULLONG_MAX;
	}

	void ResourceManager::LoadASE(const tstring& prefabName, const tstring& path, const bool isSmooth, const bool isStatic)
	{
		auto parsedData = _loader->LoadASE(path);

		auto prefabASE = BuildPrefabFromASE(parsedData, isSmooth, isStatic);

		prefabASE->_prefabName = prefabName;

		_prefabASEs.insert({ prefabName, prefabASE });
	}

	const std::shared_ptr<Prefab_ASE> ResourceManager::GetPrefab(const tstring& prefabName)
	{
		return _prefabASEs.find(prefabName) != _prefabASEs.end() ? _prefabASEs[prefabName] : nullptr;
	}

	const uint64 ResourceManager::GetMeshID(const tstring& meshName)
	{
		return _meshIDs.find(meshName) != _meshIDs.end() ? _meshIDs.at(meshName) : ULLONG_MAX;
	}

	std::shared_ptr<BoundingCube> ResourceManager::GetBoundingCube(const uint64& meshID)
	{
		return _boundingCubeInfos.find(meshID) != _boundingCubeInfos.end() ? _boundingCubeInfos.at(meshID) : nullptr;
	}

	const uint64 ResourceManager::GetFontID(const tstring& fontName)
	{
		return _fontIDs.find(fontName) != _fontIDs.end() ? _fontIDs.at(fontName) : ULLONG_MAX;
	}
}