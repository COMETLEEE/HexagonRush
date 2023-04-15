#include "pch.h"
#include "Example_Anim_Prefab.h"


void TestFuction(std::shared_ptr<Muscle::GameObject> gameObject)
{
	gameObject->GetTransform()->SetPosition(0, 0, 0);
	// 테스트용 함수.
}

Example_Anim_Prefab::Example_Anim_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("워크맨")))
{
	GetGameObject()->GetComponent<Muscle::Transform>()->SetScale(0.02f, 0.02f, 0.02f);
	std::shared_ptr<Muscle::CapsuleCollider> col = GetGameObject()->AddComponent<Muscle::CapsuleCollider>();
	col->SetCapsule(60.0f, 60.0f);
	col->SetLocalPostion(Vector3(0, 1.0f, 0));
	GetGameObject()->AddComponent<Player>();
	std::shared_ptr<Muscle::Animater> anim = GetGameObject()->GetComponent<Muscle::Animater>();
	anim->Play("Armature|mixamo.com|Layer0.002");




	Muscle::AnimEventKey animEventKey;
	animEventKey._eventfunc = TestFuction; // 함수 포인터 안넘겨도 작동은 된다. 
	// 안넘기면 bool 값을 체크해서 사용하는 컴포넌트를 따로 제작 해야함.
	// 매개변수로 게임오브젝트를 받아야 한다..

	animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;

	animEventKey._name = "test"; // 이벤트 키의 이름을 넣는 곳.

	animEventKey._targetFream = 5.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!

	animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?

	anim->AddAnimEvent("Armature|mixamo.com|Layer0.002", animEventKey); // 이벤트 등록 완료.



	bool _test = anim->GetEventKey("test"); // 이벤트 key의 현재 상태를 확인 하는 함수.
	//Components의 Update에서 조건으로 사용하면 좋을듯..?
	//초기화할 때는 사용 안해도 괜찮다.


}
