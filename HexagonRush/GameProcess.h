#pragma once

/// <summary>
/// 이 친구를 게임매니저라고 생각하자!
/// 게임 매니저 클래스를 만드려고 했는데 게임 프로세스 역할이 너무 적은것 같으니...
/// </summary>
class ISoundManager;

class ResourceManager;

class UIManager;

class GameProcess
{
public:
	GameProcess() = default;

	~GameProcess();

private:
	std::shared_ptr<Muscle::IGameEngine> m_GameEngine;

	ISoundManager* m_SoundManager;

	std::shared_ptr<ResourceManager> _resourceManager;

	// 일시 정지 패널이 Pop-Up 되었습니까 ..?
	// 이걸 통해서 모든 Script Component 및 Scene의 Update를 통제하겠습니다.
	bool _isPausePanelUp = false;

public:
	// 싱글턴화
	static std::shared_ptr<GameProcess> Get();

	static std::shared_ptr<GameProcess> _gameProcess;
	
public:
	inline bool GetIsPausePanelUp() { return _isPausePanelUp; };

	inline void SetIsPausePanelUp(bool value) { _isPausePanelUp = value; }

	void Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY);

	void Update();

	void Render();

	// 메시지 핸들러 (윈도 콜백)
	int MessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
