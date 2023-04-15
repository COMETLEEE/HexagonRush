#pragma once

#include <memory>

#include "Loader_ParsingData.h"

#ifdef LOADER_EXPORTS
#define LOADER_API __declspec(dllexport)
#else
#define LOADER_API __declspec(dllimport)
#endif

class Loader_Interface
{
	Loader_Interface(const Loader_Interface& other) = delete;

	Loader_Interface& operator =(const Loader_Interface& other) = delete;

	Loader_Interface(Loader_Interface&& other) = delete;

public:
	static LOADER_API std::shared_ptr<Loader_Interface> Get();

	virtual LOADER_API void Initialize() abstract;

	virtual LOADER_API ASEParsingData LoadASE(const tstring& path) abstract;

	virtual LOADER_API ASEParsingData LoadFBX(const tstring& path) abstract;

	virtual LOADER_API void Release() abstract;

protected:
	Loader_Interface() = default;

	virtual ~Loader_Interface() = default;

private:
	static std::shared_ptr<Loader_Interface> _loader;
};