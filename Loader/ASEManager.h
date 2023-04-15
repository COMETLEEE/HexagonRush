#pragma once

#include "ASEReader.h"
#include "ASEParsingData.h"

namespace DataLoader
{
	/// <summary>
	/// 일단 파싱하고 완성본 (정점 정보들의 배열, 애니메이션 데이터 등 ..) 을 넘기는 것부터 시작하자.
	/// </summary>
	class ASEManager
	{
	public:
		ASEManager() = default;

		~ASEManager() = default;

		ASEManager(const ASEManager& other) = delete;

		ASEManager& operator= (const ASEManager& other) = delete;

		ASEManager(ASEManager&& other) = delete;
		
	public:
		void Initialize();

		void Release();

		ASEParsingData Parse(const tstring& path);

	private:
		// ---------- Scene ----------
		void ParseScene();
		// ---------- Scene ----------


		// ---------- Material ----------
		void ParseMaterialList();

		void ParsingMaterial();

		void ParsingMaterialMapDiffuse(ASEParsingData::MaterialList::Material& parsingMat);

		void ParsingMaterialMapNormal(ASEParsingData::MaterialList::Material& parsingMat);
		// ---------- Material ----------


		// ---------- Object ----------
		void ParseGeomObject();

		void ParseMesh(ASEParsingData::GeoObject& parsingGeo);

		void ParseMeshVertexList(ASEParsingData::Mesh& parsingMesh);

		void ParseMeshFaceList(ASEParsingData::Mesh& parsingMesh);

		void ParseMeshNormals(ASEParsingData::Mesh& parsingMesh);

		void ParseMeshTVertexList(ASEParsingData::Mesh& parsingMesh);

		void ParseMeshTFaceList(ASEParsingData::Mesh& parsingMesh);

		ASEParsingData::TMAnimation ParseTMAnimation();

		void ParseTMAnimationControlPosTrack(ASEParsingData::TMAnimation& parsingAnim);

		void ParseTMAnimationControlRotTrack(ASEParsingData::TMAnimation& parsingAnim);

		ASEParsingData::NodeTM ParseNodeTM();

		void ParseBoneList(ASEParsingData::GeoObject& parsingGeo);

		void ParseWVertexs(ASEParsingData::GeoObject& parsingGeo);

		void ParseWVertexsMeshWeight(ASEParsingData::WVertex& wVertex);

		void ParseHelperObject();

		void ParseShapeObject();
		// ---------- Object ----------

	private:
		// ASE 파일의 파싱 후 결과물이 담길 구조체이다.
		ASEParsingData _aseParsingData;

		// ASE 파일의 문장을 읽어준다. 뭐 이게 Lexer ..? 같은 녀석으로 바뀌는 듯 ..
		ASEReader _aseReader;

	private:
		static std::shared_ptr<ASEManager> _aseManager;

	public:
		static std::shared_ptr<ASEManager> Get();
	};
}