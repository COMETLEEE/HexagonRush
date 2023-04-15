#pragma once
#include "Loader_Interface.h"

namespace DataLoader
{
	class ASEManager;

	// 데이터 로더 구현부
	class Loader_Comet : public Loader_Interface
	{
	public:
		Loader_Comet(const Loader_Comet& other) = delete;

		Loader_Comet& operator =(const Loader_Comet& other) = delete;

		Loader_Comet(Loader_Comet&& other) = delete;

	public:
		Loader_Comet() = default;

		virtual ~Loader_Comet() = default;

	private:
		std::shared_ptr<ASEManager> _aseManager;

	public:
		virtual void Initialize() override;

		virtual ASEParsingData LoadASE(const tstring& path) override;

		virtual ASEParsingData LoadFBX(const tstring& path) override;

		virtual void Release() override;
	};
}