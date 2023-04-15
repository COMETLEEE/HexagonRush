#include "pch.h"
#include "Light.h"

namespace Muscle
{
	/*int Light::m_PointCount = 0;
	PointLight Light::mPointLight[10];*/
	Light::Light(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
	{
	}

	Light::~Light()
	{
	}

	void Light::Start()
	{
	}

	void Light::Update()
	{
	}




}