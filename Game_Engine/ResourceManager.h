#pragma once
#include "Loader_Interface.h"
#include "Vertex_def.h"

#include "BoundingVolumes.h"

namespace Muscle
{
	struct Prefab_ASE;

	constexpr uint64 WHITE_CUBE_MESH_ID = 3;
	constexpr uint64 GREEN_CUBE_MESH_ID = 4;
	constexpr uint64 RED_CUBE_MESH_ID = 5;
	constexpr uint64 GREEN_SPHERE_MESH_ID = 6;
	constexpr uint64 RED_SPHERE_MESH_ID = 7;
	constexpr uint64 GREEN_CYLINDER_MESH_ID = 8;
	constexpr uint64 RED_CYLINDER_MESH_ID = 9;
	constexpr uint64 GREEN_HALFSPHERE_MESH_ID = 10;
	constexpr uint64 RED_HALFSPHERE_MESH_ID = 11;

	/// <summary>
	/// 게임 엔진을 통해 메모리 상에 로드한 모든 것들을 담당하는 관리자
	/// </summary>
	class ResourceManager final
	{
	public:
		ResourceManager() = default;

		~ResourceManager();

		ResourceManager(const ResourceManager& other) = delete;

		ResourceManager& operator= (const ResourceManager& other) = delete;

		ResourceManager(ResourceManager&& other) = delete;

	private:
		static std::shared_ptr<ResourceManager> _resourceManager;

		tstring _resourcePath;

		std::shared_ptr<Loader_Interface> _loader;

	public:
		void LoadEnvironmentCubeMap(const tstring& textureName, const tstring& path);

		void LoadEnvironmentPanoramaMap(const tstring& textureName, const tstring& path);

		void LoadTexture(const tstring& textureName, const tstring& path);

		const uint64 GetTextureID(const tstring& textureName);

		void LoadASE(const tstring& prefabName, const tstring& path, const bool isSmooth, const bool isStatic);

		const std::shared_ptr<Prefab_ASE> GetPrefab(const tstring& prefabName);

		const uint64 GetMeshID(const tstring& meshName);

		std::shared_ptr<BoundingCube> GetBoundingCube(const uint64& meshID);

		void LoadFont(const tstring& fontName, const tstring& path);

		const uint64 GetFontID(const tstring& fontName);

		std::unordered_map<tstring, uint64> _textureIDs;

		std::unordered_map<tstring, uint64> _meshIDs;

		// Key : Mesh ID => Value : Local Center + Scale + Matrix => Culling에도 쓸 수 있다 !
		std::unordered_map<uint64, std::shared_ptr<BoundingCube>> _boundingCubeInfos;

		std::unordered_map<tstring, std::shared_ptr<Prefab_ASE>> _prefabASEs;

		std::unordered_map<tstring, uint64> _fontIDs;

	private:
		std::shared_ptr<Prefab_ASE> BuildPrefabFromASE(ASEParsingData& aseData, const bool isSmooth = false, const bool isStatic = true);

		void CreateBoundingCube(const std::vector<StaticMeshVertex>& vertexVector, const uint64& meshID);

		// Cube, Axis, Grid, Skull
		void CreateBasicMeshes();

		void CreateCubeMesh(Vector4 color, tstring meshName);

		void CreateSphereMesh(Vector4 color, tstring meshName);

		void CreateHalfSphereMesh(Vector4 color, tstring meshName);

		void CreateCylindereMesh(Vector4 color, tstring meshName);

		void CreateAxisMesh();

		void CreateGridMesh();

		void CreateSkullMesh();

		void CreateBasicFonts();

	public:
		static std::shared_ptr<ResourceManager> Get();

		void Initialize();

		void Update();

		void Release();

		void ResetTexture();
	};

}