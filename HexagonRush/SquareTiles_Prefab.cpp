#include "pch.h"
#include "SquareTiles_Prefab.h"

SquareTiles_Prefab::SquareTiles_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	const int width = 10;
	const int height = 10;

	for (int j = 0; j < height / 2; j++)
	{
		for (int i = 0; i < width; i++)
		{
			std::shared_ptr<Muscle::GameObject> tile = std::make_shared<Platform_Prefab>()->GetGameObject();

			GetGameObject()->SetChild(tile);

			std::shared_ptr<Platform> paltform = tile->GetComponent<Platform>();

			paltform->SetPosition(Vector3(i * _TileOffset, 0.0f, j * _TileOffset * 2));
			tile->GetTransform()->RotateY(3.14f / 2);
			TileManager::Get()->PushBackTile(paltform);
		}
	}

	for (int j = 0; j < height / 2; j++)
	{
		for (int i = 0; i < width; i++)
		{
			std::shared_ptr<Muscle::GameObject> tile = std::make_shared<Platform_Prefab>()->GetGameObject();

			GetGameObject()->SetChild(tile);

			std::shared_ptr<Platform> paltform = tile->GetComponent<Platform>();

			paltform->SetPosition(Vector3(_TileOffset/2 + i * _TileOffset, 0.0f, _TileOffset + j * _TileOffset * 2));
			tile->GetTransform()->RotateY(3.14f / 2);
			TileManager::Get()->PushBackTile(paltform);
		}
	}

	GetGameObject()->SetTag("Tile");
}
