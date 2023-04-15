#pragma once
class SceneFader;

class EndingScene : public Muscle::IScene
{
public:
	EndingScene();

	virtual ~EndingScene();

private:

	std::shared_ptr<Muscle::UIRenderer> _to_be_Continue;

	std::shared_ptr<Muscle::UIRenderer> _textRenderer;

	std::shared_ptr<Muscle::UIRenderer> _BackgroundRenderer;
	
	Vector2 _textPosition;

	float _timer = 0;

public:
	virtual void Start() override;

	virtual void RapidUpdate() override;

	virtual void Update() override;

};

