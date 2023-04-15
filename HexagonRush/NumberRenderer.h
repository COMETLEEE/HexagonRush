#pragma once
class NumberRenderer : public Muscle::IComponents
{
public:
	NumberRenderer(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~NumberRenderer();
private:

	void Initialize();

	void SetNDCPostion(Vector2 _pos);

	void SetRectNDCPostion(Vector2 leftTop, Vector2 rightBottom);

	void SetPostion(Vector3 _pos); // 월드 포지션을 넣으면 NDC로 변환해준다.

	Vector2 _leftTop;

	Vector2 _rightBottom;

	bool _isAlpha_Hide_Disabled = false;

	float _speed = 1.0f;
public:
	void SetNDCPosition(Vector2 leftTop, Vector2 rightBottom);

	void SetNumber(int number); // 숫자를 3자리를 초과 하지말자..!

	std::vector<std::shared_ptr<Muscle::UIRenderer>> _numberRenderer;

	std::shared_ptr<Muscle::UIRenderer> _criticalRenderer;

	void Alpha_Hide_Disabled(float speed = 1.0f);

public:
	virtual void Start();

	virtual void Update();

	virtual void Finalize();
};

