#pragma once


namespace Muscle
{
	class Light : public IComponents
	{
	public:
		Light(std::shared_ptr<GameObject> _GameObject);
		~Light();

	protected:
		std::shared_ptr<LightInfo> _lightInfo;

	public:
		virtual void Start() override;

		virtual void Update() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void Render() abstract; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

	public:
#pragma region PBR Lighting Model
		inline void SetColor(Vector3 rgb) { _lightInfo->_color = rgb; }

		inline void SetLumen(float lumen) { _lightInfo->_lumen = lumen; }
#pragma endregion

#pragma region Legacy Lighting Model
		//void SetAmbient(Vector4 _Ambient) { _lightInfo->_ambient = _Ambient; } // 주변광

		//void SetDiffuse(Vector4 _Diffuse) { _lightInfo->_diffuse = _Diffuse; } // N dot L

		//void SetSpecular(Vector4 _Specular) { _lightInfo->_specular = _Specular; } //반사광
#pragma endregion
	};
}