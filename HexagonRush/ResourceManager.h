#pragma once
/// <summary>
/// 무슨 용도로 사용할까?
/// 일단은 ASE파일을 로드하는 역할을 할까?
/// 사운드도 여기서 할깡?
/// </summary>
/// 
/// 
class Muscle::ResourceManager;

class ResourceManager : public std::enable_shared_from_this<ResourceManager>
{
public:
	ResourceManager() = default;

	~ResourceManager() = default;

private:
	std::shared_ptr<Muscle::ResourceManager> _graphicsResourceManager; //캐싱해서 사용할 것.

	static std::shared_ptr<ResourceManager> _instance;

	std::queue<std::tuple<tstring, tstring, bool,bool>> _aseLoadDataQueue;

	std::queue<std::tuple<tstring, tstring>> _textureLoadDataQueue;

	std::queue<std::tuple<tstring, tstring>> _environmentPanoramaMapLoadDataQueue;

	std::queue<std::tuple<std::string, std::string,unsigned int>> _soundLoadDataQueue;

	int _resourceAllCount;

	bool _isComplete;

	void DataQueuePopToThead(); // 비동기 가자~ 가버려 가버려 가버려 ~

public:
	const uint64 GetTextureID(const tstring& textureName);

	const uint64 GetMeshID(const tstring& meshName);

	const uint64 GetFontID(const tstring& fontName);

	static std::shared_ptr<ResourceManager> Get();

	void LoadASE(const tstring& name, const tstring& path, bool isSmooth, bool isStatic);

	void LoadTexture(const tstring& name, const tstring& path);

	void LoadEnvironmentPanoramaMap(const tstring& name, const tstring& path);

	void LoadSoundFile(const std::string& name, const std::string& filePath, unsigned int mode = 0);

	void StartDataLoadToTread();

	bool GetisComplete();

	int GetAllResourceCount(); // 모든 리소스 카운트

	int GetResourcesRemainingCount(); // 남은 리소스 카운트

public:
	void Initialize();

	void Finalize();

	void Update();

	void LoadAllASE();

	void ResetTexture();
	// 인게임 리소스 로드 코드 .. 죄악이 담긴 곳

public:
	// Stage 1의 메쉬, 텍스처를 로드한다.
	void LoadStage1();

	// Stage 2의 메쉬, 텍스처를 로드한다.
	void LoadStage2();

	// UI의 메쉬, 텍스처를 로드한다. 타이틀, 엔딩, 클리어는 이것만 로드하면 될 듯.
	void LoadUIScene();

private:
	void LoadInGameUI();
	
	// Stage 1
	void LoadStage1MapTexture();
	void LoadStage1MapASE();

	// Stage 2
	void LoadStage2MapTexture();
	void LoadStage2MapASE();

	// 튜토리얼 보스를 메쉬 ,텍스쳐를 로드한다..
	void LoadTu_BossTexture();
	void LoadTu_BossASE();

	// Tile의 메쉬, 텍스처를 로드한다.
	void LoadTileTexture();
	void LoadTileASE();

	// Sound를 로드한다.
	void LoadSound();

	// Player TExture를 로드한다.
	void LoadPlayerTexture();

	// UITexture를 로드한다.
	void LoadUITexture();

	// Loading Scene 리소스를 로드한다.
	void LoadLoadingScene();

	// 2번 보스를 메쉬 ,텍스쳐를 로드한다..
	void Boss2_BossTexture();

	void Boss2_BossASE();
};