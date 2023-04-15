#pragma once
class Drill_Sign : public Muscle::IComponents
{
public:
	Drill_Sign(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~Drill_Sign();

private:
	std::shared_ptr<Muscle::RendererBase> _rendererBase;

public:
	virtual void Start() override;

	virtual void Update() override;
};

