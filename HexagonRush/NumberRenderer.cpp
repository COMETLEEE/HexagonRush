#include "pch.h"
#include "NumberRenderer.h"

NumberRenderer::NumberRenderer(std::shared_ptr<Muscle::GameObject> gameObject) : IComponents(gameObject)
{
	_leftTop = Vector2::Zero;
	_rightBottom = Vector2::Zero;
}

NumberRenderer::~NumberRenderer()
{

}

void NumberRenderer::Initialize()
{
	for (int i = 0; i < 3; i++)
	{
		Vector2 leftTop = Vector2(-1.0f, 1.0f) + (Vector2(0.05f, 0.0f) * i);
		Vector2 rightBottom = leftTop + Vector2(0.05f, -0.05f);
		auto uiRenderer = AddComponent<Muscle::UIRenderer>();
		uiRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("0")));
		uiRenderer->SetDrawNDCPosition(leftTop, rightBottom);
		_numberRenderer.push_back(uiRenderer);
	}

	_criticalRenderer = AddComponent<Muscle::UIRenderer>();
	_criticalRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("NumberBack")));
	_criticalRenderer->SetSortOrder(6);
	//_criticalRenderer->SetIsEnable(false);


}

void NumberRenderer::SetNDCPostion(Vector2 _pos)
{
	if (_numberRenderer.empty())
		Initialize();

	int index = -1;

	float left = 2000.0f;
	float top = -2000.f;
	float right = -2000.f;
	float bottom = 2000.f;

	for (auto& iter : _numberRenderer)
	{
		Vector2 leftTop = _pos + (Vector2(0.05f, 0.0f) * index++);
		Vector2 rightBottom = leftTop + Vector2(0.05f, -0.05f);



		left = min(leftTop.x, left);
		top = max(leftTop.y, top);
		bottom = min(rightBottom.y, bottom);
		right = max(rightBottom.x, right);

		iter->SetDrawNDCPosition(leftTop, rightBottom);
	}

	_criticalRenderer->SetDrawNDCPosition(Vector2(left - 0.04f, top + 0.04f), Vector2(right + 0.04f, bottom - 0.04f));
}

void NumberRenderer::SetRectNDCPostion(Vector2 leftTop, Vector2 rightBottom)
{
	if (_numberRenderer.empty())
		Initialize();
	float width = rightBottom.x - leftTop.x;
	float height = leftTop.y - rightBottom.y;
	Vector2 pos = leftTop;
	int index = 0;
	for (auto& iter : _numberRenderer)
	{
		Vector2 leftTop = pos + (Vector2(width / 3, 0.0f) * index++);
		Vector2 rightBottom = leftTop + Vector2(width / 3, -height);
		iter->SetDrawNDCPosition(leftTop, rightBottom);
	}
}

void NumberRenderer::SetPostion(Vector3 _pos)
{
	auto viewProj = Muscle::IGameEngine::Get()->GetMainCamera()->View() * Muscle::IGameEngine::Get()->GetMainCamera()->Proj();

	Vector3 posVector = Vector3::Transform(_pos, viewProj);

	if (_isnanf(posVector.x))return;
	if (_isnanf(posVector.y))return;

	SetNDCPostion(Vector2(posVector.x, posVector.y));

}


void NumberRenderer::SetNDCPosition(Vector2 leftTop, Vector2 rightBottom)
{
	_leftTop = leftTop;
	_rightBottom = rightBottom;
}

void NumberRenderer::SetNumber(int number)
{
	if (_numberRenderer.empty())
		Initialize();

	SetIsEnable(true);


	for (int i = 0; i < 3; i++)
	{
		_numberRenderer[i]->SetIsEnable(false);
	}
	_criticalRenderer->SetIsEnable(false);
	if (number / 100 > 0) // 세 자리.
	{
		int _number[3];
		_number[0] = number / 100; // 백의 자리
		_number[1] = (number % 100) / 10; //십의 자리
		_number[2] = (number % 10); // 일의 자리


		for (int i = 0; i < 3; i++)
		{
			_numberRenderer[i]->SetTextureID(ResourceManager::Get()->GetTextureID(std::to_wstring(_number[i])));
			_numberRenderer[i]->SetIsEnable(true);
		}
		_criticalRenderer->SetIsEnable(true);

	}
	else if (number / 10 > 0) // 두 자리.
	{
		int _number[2];
		_number[0] = number / 10; //십의 자리
		_number[1] = (number % 10); // 일의 자리
		if (number > 70)
		{

			for (int i = 0; i < 2; i++)
			{
				_numberRenderer[i]->SetTextureID(ResourceManager::Get()->GetTextureID(std::to_wstring(_number[i])));
				_numberRenderer[i]->SetIsEnable(true);
			}
		}
		else
		{

			for (int i = 0; i < 2; i++)
			{
				_numberRenderer[i]->SetTextureID(ResourceManager::Get()->GetTextureID(std::to_wstring(_number[i]) + L"_gray"));
				_numberRenderer[i]->SetIsEnable(true);
			}
		}
	}
	else// 한 자리.
	{
		int _number = number; // 일의 자리

		_numberRenderer[1]->SetTextureID(ResourceManager::Get()->GetTextureID(std::to_wstring(_number)));
		_numberRenderer[1]->SetIsEnable(true);
	}
}

void NumberRenderer::Alpha_Hide_Disabled(float speed)
{
	_speed = speed;
	_isAlpha_Hide_Disabled = true;
}

void NumberRenderer::Start()
{
	if (!_numberRenderer.empty()) return;

	Initialize();
}

void NumberRenderer::Update()
{
	if (_leftTop != Vector2::Zero || _rightBottom != Vector2::Zero)
	{
		SetRectNDCPostion(_leftTop, _rightBottom);
	}
	else
	{
		SetPostion(GetGameObject()->GetTransform()->GetWorldPosition());
	}

	if (_isAlpha_Hide_Disabled)
	{
		if (_numberRenderer[0]->GetUIData()->_drawInfo->_color.w < 0)
		{
			_isAlpha_Hide_Disabled = false;
			for (auto& iter : _numberRenderer)
			{
				iter->GetUIData()->_drawInfo->_color.w = 1.0f;
			}
			_criticalRenderer->GetUIData()->_drawInfo->_color.w = 1.0f;
			GetGameObject()->SetIsEnable(false);
			return;
		}

		for (auto& iter : _numberRenderer)
		{
			iter->GetUIData()->_drawInfo->_color.w -= _speed * Muscle::CTime::GetGameDeltaTime();
		}
		_criticalRenderer->GetUIData()->_drawInfo->_color.w -= _speed * Muscle::CTime::GetGameDeltaTime();
	}



}

void NumberRenderer::Finalize()
{
	for (auto& iter : _numberRenderer)
	{
		iter.reset();
	}
	_numberRenderer.clear();
}
