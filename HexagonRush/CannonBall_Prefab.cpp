#include "pch.h"
#include "CannonBall_Prefab.h"

#include "OilField.h"
#include "ElectronicField.h"
#include "LandMineField.h"
#include "PanicBlueField.h"

CannonBall_Prefab::CannonBall_Prefab(CANNON_BALL_TYPE type) : IPrefab(Muscle::CreateFromPrefab(TEXT("Cannon_Ball")))
{
	GetGameObject()->GetTransform()->SetPosition(1000.0f, 1000.0f, 1000.0f);

	GetGameObject()->SetTag("Enemy");

	Vector3 from;
	Vector3 To;

	// 왼팔 총구 딱딱한 코딩
	from = Vector3(-6.5f, 10.25f, 2.91f);

	// 랜덤 땅
	To = Vector3(Muscle::CTime::GetFloatRand<float>(-18, 28), 4.0f, Muscle::CTime::GetFloatRand<float>(-25, 25));

	std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(To);

	ObjectMover::Get()->MoveFromTo(GetGameObject()->GetTransform(),
		from,
		To,
		2.f,
		0,
		MoveInfo::Bezier,
		Vector3(0.0f, 10.f, 0.0f),
		Vector3(0.0f, 20.0f, 0.0f),
		MoveInfo::Up,
		true
	);

	switch (type)
	{
		case CANNON_BALL_TYPE::OIL:
		{
			// 오일 필드 생성. 데미지는 주지 않는다.
			auto func = [](std::shared_ptr<Muscle::GameObject> _gameobj)
			{
				auto object = Muscle::CreateFromPrefab(TEXT("Boss2_Flowed"));

				std::shared_ptr<Muscle::MeshRenderer> meshRen = object->GetChildrens()[0]->GetComponent<Muscle::MeshRenderer>();

				std::shared_ptr<RenderingData_3D> data = meshRen->_renderingData;

				data->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Black_Albed"));
				data->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Normal"));
				data->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Roughness"));

				data->_shaderInfo->_vsName = TEXT("VS_ANR");
				data->_shaderInfo->_psName = TEXT("PS_ANR");

				object->AddComponent<OilField>();

				auto col = object->AddComponent<Muscle::SphereCollider>();

				col->SetRadius(4.0f);

				object->GetTransform()->SetPosition(_gameobj->GetTransform()->GetPosition() );

				object->SetTag("Enemy");

				// 수동으로 지워줘야 한다.
				Muscle::DeleteGameObject(_gameobj);

				GetSoundManager()->Play("OillFall", IPlayMode::Effect);

				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);
			};

			Muscle::CTime::Invoke(func, 2.f, GetGameObject());

			break;
		}

		case CANNON_BALL_TYPE::ELECTRONIC:
		{
			// 일렉트로닉 필드 생성. 데미지는 주지 않는다. 옐로우
			auto func2 = [](std::shared_ptr<Muscle::GameObject> _gameobj)
			{
				auto object = Muscle::CreateFromPrefab(TEXT("Boss2_Flowed"));

				std::shared_ptr<Muscle::MeshRenderer> meshRen = object->GetChildrens()[0]->GetComponent<Muscle::MeshRenderer>();

				std::shared_ptr<RenderingData_3D> data = meshRen->_renderingData;

				data->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Yellow_Albed"));
				data->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Yellow_Normal"));
				data->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Yellow_Metallic"));
				data->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Yellow_Roughness"));
				data->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Yellow_Emissive"));

				data->_shaderInfo->_vsName = TEXT("VS_ANMRE");
				data->_shaderInfo->_psName = TEXT("PS_ANMRE");

				object->AddComponent<ElectronicField>();

				auto col = object->AddComponent<Muscle::SphereCollider>();

				col->SetRadius(4.0f);


				object->GetTransform()->SetPosition(_gameobj->GetTransform()->GetPosition() );

				object->SetTag("Enemy");

				// 수동으로 지워줘야 한다.
				Muscle::DeleteGameObject(_gameobj);

				GetSoundManager()->Play("OillFall", IPlayMode::Effect);

				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);
			};

			Muscle::CTime::Invoke(func2, 2.f, GetGameObject());

			break;
		}

		case CANNON_BALL_TYPE::LANDMINE:
		{
			// 랜드마인. 레드
			auto func2 = [](std::shared_ptr<Muscle::GameObject> _gameobj)
			{
				auto object = Muscle::CreateFromPrefab(TEXT("Boss2_Flowed"));

				std::shared_ptr<Muscle::MeshRenderer> meshRen = object->GetChildrens()[0]->GetComponent<Muscle::MeshRenderer>();

				std::shared_ptr<RenderingData_3D> data = meshRen->_renderingData;

				data->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Red_Albed"));
				data->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Red_Normal"));
				data->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Red_Metallic"));
				data->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Red_Roughness"));

				data->_shaderInfo->_vsName = TEXT("VS_ANMR");
				data->_shaderInfo->_psName = TEXT("PS_ANMR");

				object->AddComponent<LandMineField>();

				auto col = object->AddComponent<Muscle::SphereCollider>();

				col->SetRadius(4.0f);


				object->GetTransform()->SetPosition(_gameobj->GetTransform()->GetPosition());

				object->SetTag("Enemy");

				// 수동으로 지워줘야 한다.
				Muscle::DeleteGameObject(_gameobj);

				GetSoundManager()->Play("OillFall", IPlayMode::Effect);

				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);
			};

			Muscle::CTime::Invoke(func2, 2.f, GetGameObject());

			break;
		}

		case CANNON_BALL_TYPE::PANICBLUE:
		{
			// 패닉 블루. 블루
			auto func2 = [](std::shared_ptr<Muscle::GameObject> _gameobj)
			{
				auto object = Muscle::CreateFromPrefab(TEXT("Boss2_Flowed"));

				std::shared_ptr<Muscle::MeshRenderer> meshRen = object->GetChildrens()[0]->GetComponent<Muscle::MeshRenderer>();

				std::shared_ptr<RenderingData_3D> data = meshRen->_renderingData;

				data->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Blue_Albed"));
				data->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Blue_Normal"));
				data->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Blue_Metallic"));
				data->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Flowed_Blue_Roughness"));

				data->_shaderInfo->_vsName = TEXT("VS_ANMR");
				data->_shaderInfo->_psName = TEXT("PS_ANMR");

				object->AddComponent<PanicBlueField>();

				auto col = object->AddComponent<Muscle::SphereCollider>();

				col->SetRadius(4.0f);


				object->GetTransform()->SetPosition(_gameobj->GetTransform()->GetPosition());

				object->SetTag("Enemy");

				// 수동으로 지워줘야 한다.
				Muscle::DeleteGameObject(_gameobj);

				GetSoundManager()->Play("OillFall", IPlayMode::Effect);

				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);
			};

			Muscle::CTime::Invoke(func2, 2.f, GetGameObject());

			break;
		}
	}
}

CannonBall_Prefab::~CannonBall_Prefab()
{

}