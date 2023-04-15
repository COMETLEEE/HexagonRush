#pragma once
class DrillSpiner : public  Muscle::IComponents
{
public:
	DrillSpiner(std::shared_ptr<Muscle::GameObject> GameObject);

	virtual ~DrillSpiner();

private:
	std::shared_ptr<Muscle::Transform> _rendererTransform; // 자식이 렌더러를 가지고 있다. 캐싱해서 쓰자.

public:

	virtual void Start() override;

	virtual void Update() override;

	virtual void Finalize() override;
};

