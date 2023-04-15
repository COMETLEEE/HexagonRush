#include "pch.h"
#include "ResourceLoadBar.h"

ResourceLoadBar::ResourceLoadBar(std::shared_ptr<Muscle::GameObject> gameObject) : Muscle::IComponents(gameObject)
{
}

void ResourceLoadBar::Start()
{
	_uiRenderer = GetComponent<Muscle::UIRenderer>();
}

void ResourceLoadBar::LateUpdate()
{
	_uiRenderer->SetRectFill((float)(ResourceManager::Get()->GetAllResourceCount() - ResourceManager::Get()->GetResourcesRemainingCount()) / (float)ResourceManager::Get()->GetAllResourceCount());
}
