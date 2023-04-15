#pragma once

/// <summary>
/// COM에 꽂혀 있을 때, 가장 집중해서 보았던 구현인
/// (가상함수 테이블을 사용한다면 모두 비슷하게 동작할 수 있는)
/// 내부에서 new해서 인터페이스만 노출해서 사용하기
/// 
/// 2022.02.19 LeHide
/// </summary>


// ISoundManager.h에는 구현이 없는 것을 명시적으로 보여주기 위해서 이렇게 예제를 만들었다.
class ISoundManager;

ISoundManager* GetSoundManager();
