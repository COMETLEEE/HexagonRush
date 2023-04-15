#include "pch.h"
#include "Loader_Comet.h"

#include "ASEManager.h"

namespace DataLoader
{
	void Loader_Comet::Initialize()
	{
		_aseManager = ASEManager::Get();

		_aseManager->Initialize();
	}

	ASEParsingData Loader_Comet::LoadASE(const tstring& path)
	{
		ASEParsingData parsedData = _aseManager->Parse(path);

		return parsedData;
	}

	ASEParsingData Loader_Comet::LoadFBX(const tstring& path)
	{
		return ASEParsingData();
	}

	void Loader_Comet::Release()
	{
		_aseManager->Release();
	}
}