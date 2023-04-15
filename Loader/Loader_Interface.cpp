#include "pch.h"
#include "Loader_Interface.h"

#include "Loader_Comet.h"

std::shared_ptr<Loader_Interface> Loader_Interface::_loader = nullptr;

std::shared_ptr<Loader_Interface> Loader_Interface::Get()
{
	if (_loader == nullptr)
		_loader = std::make_shared<DataLoader::Loader_Comet>();

	return _loader;
}