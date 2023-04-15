#include "pch.h"
#include "Platform.h"

Platform::Platform(std::shared_ptr<Muscle::GameObject> gameObj) : IComponents(gameObj), _color(StateColor::Red), _status(PlatStatus::Idle), _dropPostion(nullptr)
{
	_status = PlatStatus::Idle;
	_preStatus = PlatStatus::Idle;

}

Platform::~Platform()
{

}

void Platform::DeletePlatform()
{
	// 딜리트될 때도 끈다.
	_particleRenderer->Stop();

	if (_status == PlatStatus::Delete || _status == PlatStatus::Respawn) return;
	_status = PlatStatus::Delete;
}

void Platform::CreatePlatform()
{
	_status = PlatStatus::Respawn;
}

void Platform::SetColor(StateColor color)
{
	_color = color;

	switch (color)
	{
	case StateColor::Red:
	{
		_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_Albedo"));
		_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_Normal"));
		_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_Metallic"));
		_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_Roughness"));
		_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_AO"));
		_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Red_Emissive"));

		break;
	}

	case StateColor::Blue:
	{
		_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_Albedo"));
		_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_Normal"));
		_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_Metallic"));
		_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_Roughness"));
		_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_AO"));
		_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Blue_Emissive"));

		break;
	}

	case StateColor::Green:
	{
		_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_Albedo"));
		_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_Normal"));
		_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_Metallic"));
		_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_Roughness"));
		_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_AO"));
		_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_Green_Emissive"));

		break;
	}

	case StateColor::None:
	{
		_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Albedo"));
		_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Normal"));
		_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Metallic"));
		_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Roughness"));
		_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_AO"));
		_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Emissive"));

		break;
	}
	default:
		break;
	}
}

void Platform::SetPosition(Vector3 initialPosition)
{

	_initialPosition = initialPosition;
	GetGameObject()->GetTransform()->SetPosition(_initialPosition);
}

void Platform::SelectPlatform()
{
	// 나중에 이펙트가 생길시에 사용하자!.
	_status = PlatStatus::Select;

	_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Albedo"));
	_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Normal"));
	_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Metallic"));
	_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Roughness"));
	_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_AO"));
	_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_N.color_Emissive"));

	// 같게 올려보내면 진짜 ㅈㄴ 안보임.
	_particleRenderingData->_shapeInfo->_color = Vector4(1.f, 1.f, 1.f, 1.f);

	_particleRenderer->Play();
}

void Platform::SelectCanclePlatform()
{

	_status = PlatStatus::Idle;

	SetColor(_color);
	// 취소될 때는 끈다
	_particleRenderer->Stop();
}

void Platform::PermanentlyDelete()
{
	_status = PlatStatus::PermanentlyDelete;

	_particleRenderer->Stop();
}

void Platform::Drop(std::shared_ptr<Platform> platform)
{
	_status = PlatStatus::Drop;

	_dropPostion = platform;

	GetGameObject()->SetIsEnable(true);
}

void Platform::DropReset()
{
	if (!_dropPostion)
		return;

	_status = PlatStatus::Idle;

	if (_dropPostion->GetColor() != _color)
		if (TileManager::Get()->FindSelectTile(_dropPostion))
			TileManager::Get()->SelectCancel();

	if (_dropPostion->GetState() != PlatStatus::PermanentlyDelete)
		_dropPostion->SetColor(_color);

	_dropPostion.reset();
	SetColor(StateColor(Muscle::CTime::GetRand<int>(0, 2)));
	GetGameObject()->SetIsEnable(false);
	_preStatus = _status;
}

void Platform::Start()
{
	// 이거 너무 좀 그런데? ㅋㅋㅋㅋㅋ 나중에 다시 생각해보자

	std::shared_ptr<Muscle::MeshRenderer> renderer = nullptr;

	if (!renderer)
		renderer = GetGameObject()->GetComponent<Muscle::MeshRenderer>();

	if (!renderer)
		renderer = GetGameObject()->GetChildrens()[0]->GetComponent<Muscle::MeshRenderer>();

	std::shared_ptr<Muscle::ParticleRenderer> parRen = nullptr;

	if (!parRen)
		parRen = GetGameObject()->GetComponent<Muscle::ParticleRenderer>();

	if (!parRen)
		parRen = GetGameObject()->GetChildrens()[0]->GetComponent<Muscle::ParticleRenderer>();

	_particleRenderer = parRen;

	_particleRenderer->SetOffsetMatrix(Matrix::CreateRotationX(3.141592f / 2));

	_particleRenderingData = _particleRenderer->GetParticleData();

	// 파티클 렌더링 데이터 셋팅

	// Init Info
	_particleRenderingData->_initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	_particleRenderingData->_initInfo->_firstRun = true;
	_particleRenderingData->_initInfo->_isLoop = true;
	_particleRenderingData->_initInfo->_maxParticleCount = 1000;

	// Shape Info
	_particleRenderingData->_shapeInfo->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("TileParticle"));

	_particleRenderingData->_shapeInfo->_emitVelocity = Vector3::Up * 1.5f;
	_particleRenderingData->_shapeInfo->_emitTime = 0.010f;
	_particleRenderingData->_shapeInfo->_radius = 1.5f;

	_particleRenderingData->_shapeInfo->_deviation = Vector2(0.1f, 0.1f);			// 오오라에는 편차를 좀 주자 !
	_particleRenderingData->_shapeInfo->_useXYSameDeviation = true;
	_particleRenderingData->_shapeInfo->_particleAverageSize = Vector2(0.25f, 0.25f);
	_particleRenderingData->_shapeInfo->_period = 0.2f;

	_particleRenderingData->_shapeInfo->_acceleration = Vector3(0.f, 1.5f, 0.f);
	_particleRenderingData->_shapeInfo->_lifeSpan = 1.2f;

	// 파티클 렌더링 데이터 셋팅

	_renderingData = renderer->_renderingData;

	_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
	_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");

	_renderingData->_materialInfo->_metallic = 0.2f;
	_renderingData->_materialInfo->_roughness = 0.5f;

	SetColor(StateColor(Muscle::CTime::GetRand<int>(0, 2)));

	_collider = GetComponent<Muscle::Collider>();
	//_collider->SetIsEnable(true);
}

void Platform::Update()
{
	if (GameProcess::Get()->GetIsPausePanelUp()) return;
	if (_preStatus == PlatStatus::PermanentlyDelete) return;
	/// 없어도 되지만 기분이 좋으니..
	if (Muscle::KeyBoard::Get()->KeyDown('Q'))
		CreatePlatform();

	if (_preStatus == PlatStatus::Respawn)
	{
		if (_color == StateColor::None)
		{
			auto thisScale = GetGameObject()->GetTransform()->GetScale();
			if (thisScale.x > 0)
			{
				Vector3 scale = thisScale - (Vector3(1.0f, 1.0f, 1.0f) * Muscle::CTime::GetGameDeltaTime() * 0.5f);

				if (scale.x <= 0)
					scale = Vector3(0.0f, 0.0f, 0.0f);

				GetGameObject()->GetTransform()->SetScale(scale);
			}
			else
			{
				SetColor(StateColor(Muscle::CTime::GetRand<int>(0, 2)));
			}
		}
		else
		{

			auto thisScale = GetGameObject()->GetTransform()->GetScale();
			if (thisScale.x < 1)
			{
				Vector3 scale = thisScale + (Vector3(1.0f, 1.0f, 1.0f) * Muscle::CTime::GetGameDeltaTime() * 0.5f);

				if (scale.x >= 1)
					scale = Vector3(1.0f, 1.0f, 1.0f);

				GetGameObject()->GetTransform()->SetScale(scale);
			}
			else
			{
				_status = PlatStatus::Idle;
				_collider->SetIsEnable(true);
			}
		}
	}

	if (_status != _preStatus)
	{

		switch (_status)
		{
		case PlatStatus::Idle:
			GetGameObject()->GetTransform()->SetPosition(_initialPosition);
			break;
		case PlatStatus::Select:
		{
			// GetGameObject()->GetTransform()->SetPosition(_initialPosition + Vector3(0, -0.5f, 0));

			break;
		}
		case PlatStatus::Delete:
			GetGameObject()->GetTransform()->SetPosition(1000.0f, 1000.0f, 1000.0f);
			_collider->SetIsEnable(false);
			_status = PlatStatus::Respawn; // 바로 재생성..!;
			return; // OK 이 친구는 예외적인 친구다..
		case PlatStatus::Respawn:
			SetColor(StateColor::None);
			GetGameObject()->GetTransform()->SetPosition(_initialPosition);
			_collider->SetIsEnable(false);
			break;
		case PlatStatus::Drop:
			ObjectMover::Get()->MoveFromTo(GetGameObject()->GetTopParent()->GetTransform(),
				_dropPostion->GetGameObject()->GetTransform()->GetWorldPosition() + Vector3(0, 40, 0),
				_dropPostion->GetGameObject()->GetTransform()->GetWorldPosition(), 2.0f);
			auto func = [](std::shared_ptr<Muscle::GameObject> target)
			{
				target->GetComponent<Platform>()->DropReset();
			};
			Muscle::CTime::Invoke(func, 2.0f, GetGameObject());
			break;
		case PlatStatus::PermanentlyDelete:
			SetColor(StateColor::None);
			GetGameObject()->GetTransform()->SetPosition(_initialPosition);
			GetGameObject()->GetTransform()->SetScale(Vector3(1, 1, 1));
			_collider->SetIsEnable(false);
			break;
		default:
			break;
		}
	}
	_preStatus = _status;

}
