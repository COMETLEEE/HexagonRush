#pragma once

struct ASEParsingData
{
	struct Scene
	{
		std::string fileName;
		unsigned firstFrame;
		unsigned lastFrame;
		unsigned frameSpeed;
		unsigned ticksPerFrame;

		// 사용하지 않는 값입니다.

		Vector3 backgroundStatic;
		Vector3 ambientStatic;
	};

	struct MaterialList
	{
		struct Material
		{
			struct MapDiffuse
			{
				std::string bitmap = "";
			};

			struct MapNormal
			{
				std::string bitmap = "";
			};

			Vector3 ambient;
			Vector3 diffuse;
			Vector3 specular;
			MapDiffuse mapDiffuse;
			MapNormal mapNormal;
		};

		unsigned materialCount;
		std::vector<Material> materialList;
	};

	struct Mesh
	{
		struct Vertex
		{
			unsigned index;
			Vector3 position;
		};

		struct Face
		{
			unsigned index;
			unsigned vertex[3];
		};

		struct FaceNormal
		{
			struct VertexNormal
			{
				unsigned index;
				Vector3 normal;
			};

			unsigned index;
			Vector3 faceNormal;
			VertexNormal vertexNormal[3];
		};

		struct TVertex
		{
			unsigned index;
			float u;
			float v;
		};

		struct TFace
		{
			unsigned faceIndex;
			unsigned vertexIndex[3];
		};

		int timeValue;
		int numVertex;
		int numFaces;
		int numTVertex;
		int numTVFace;
		std::vector<Vertex> vertexList;
		std::vector<Face> faceList;
		std::vector<FaceNormal> faceNormals;
		std::vector<TVertex> tVertexList;
		std::vector<TFace> tFaceList;
	};

	struct WVertex
	{
		int index;
		std::vector<std::pair<int, float>> boneBlendingWeight;
	};

	struct NodeTM
	{
		struct TMRow
		{
			unsigned index;
			float x;
			float y;
			float z;
		};

		std::string nodeName;
		Matrix tm;

		bool bHasNegativeScale = false;
	};

	struct TMAnimation
	{
		std::string nodeName;
		std::string AnimName;
		std::vector<std::pair<unsigned, Vector3>> controlPosSamples;
		std::vector<std::pair<unsigned, Quaternion>> controlRotSamples;
	};

	struct GeoObject
	{
		std::string nodeName;
		std::string nodeParent;
		NodeTM nodeTM;
		Mesh mesh;
		int materialRef;
		std::vector<TMAnimation> tmAnimation;
		int numBone;

		std::vector<std::string> boneList;

		std::vector<WVertex> wVertexs;
	};

	struct HelperObject
	{
		std::string nodeName;
		std::string nodeParent;
		NodeTM nodeTM;
		std::vector<ASEParsingData::TMAnimation> _tmAnimations;
	};

	struct ShapeObject
	{
		std::string nodeName;
		std::string nodeParent;
		NodeTM nodeTM;
		std::vector<ASEParsingData::TMAnimation> _tmAnimations;
	};

	Scene scene;

	MaterialList materialList;

	std::vector<HelperObject> helperObjectList;

	std::vector<ShapeObject> shapeObjectList;

	std::vector<GeoObject> geoObjectList;
};