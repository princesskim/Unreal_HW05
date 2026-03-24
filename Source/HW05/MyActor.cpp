// MyActor.cpp


#include "MyActor.h"
#include "Engine/Engine.h" //Gengine 및 디버깅 메시지 관련 헤더

AMyActor::AMyActor()
{
 	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0, 50, 0));

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("Hello, Unreal!"));

		// FString::Printf()의 %s는 TCHAR* 타입 문자열 포인터를 요구함
		// FString == 클래스 객체
		// 약속 "FString 앞에 * 붙이면 내부 문자열 주소 반환"
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Actor: %s"), *GetName())); //GetName() 결과는 FString, 역참조 필요
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, TEXT(">> State 0"));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Actor: %s"), *GetActorLocation().ToString())); //ToString() 결과는 FString, 역참조 필요
	}

	/*
	// AddOnScreenDebugMessage()는 GEngine을 통해 호출해야 함

	AddOnScreenDebugMessage(
		int32(혹은 uint64) Key,
		float TimeToDisplay,
		FColor DisplayColor,
		const FString & DebugMessage,
		bool bNewerOnTop,
		const FVector2D & TextScale
	);

	// Key : 출력 메시지의 고유 키, -1을 하면 메시지가 누적 출력됨
	// TimeToDisplay : 메시지가 화면에 표시될 시간(초 단위)
	// DisplayColor : 메시지의 색상, FColor:: 언리얼에서 쓰는 8비트 RGBA 색 구조체
	// TEXT : C++ 문자열 리터럴을 엔진의 문자 타입(TCHAR)용 유니코드 리터럴로 바꿔 주는 매크로
	// GetName : AActor클래스에서 제공하는 함수, 액터의 이름을 FString 형식으로 반환
	// Tostring() : 언리얼의 많은 타입이 자신을 사람이 읽기 쉬운 문자열로 바꾸는 멤버 함수
	*/
	
	FVector prevVector = GetActorLocation();


	for (int i = 0; i < 10; ++i) {
		switch (FMath::RandRange(0,1)) {
		case 0:
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT(">> State %d"), i + 1));
			}
			RandomMove(200.f);
			break;
		case 1:
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT(">> State %d"), i + 1));
			}
			RandomTurn_45();
			break;
		}

		TravelDistance += FVector::Dist(prevVector, GetActorLocation());
		prevVector = GetActorLocation();
	}

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT(">> Travel Distance : %.2f"), TravelDistance));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT(">> Event Counts: %d"), Trigger_z200_cnt));
	}

}

/*
SetActorLocation(FVector(0, 50, 0));
SetActorRotation(FRotator(100.f, 100.f, 100.f)); 

1) 위치 설정 - 액터를 월드 위치를 지정한 좌표로 이동
bool AActor::SetActorLocation(NewLocation)
매개변수: FVector(X, Y, Z), 단위 cm
반환값 : 실제로 위치가 바뀌면 true

2) 회전 설정(로테이터 버전) - 액터의 월드 회전을 지정한 로테이터 값으로 설정
bool AActor::SetActorRotation(NewRotation)
매개변수: FRotator(Pitch(상하), Yaw(좌우), Roll(비틀기)), 단위 도(degree)
	100.f 같은 값은 자동으로 내부 정규화(래핑)되어 적용됨.
반환값 : 실제로 회전하면 true

선택) Sweep, OutHit, Teleport 옵션까지 사용하는 확장형 예시
SetActorLocation(
	NewLocation : 목표 위치
	bSweep : true면 시작->목표 경로를 콜리전 모양으로 쓸며 검사, 벽에 막히면 그 앞에 멈춤/슬라이드
	OutHit : 스윕 결과(첫 블로킹 히트)를 받는 FHitResult* //nullptr 가능
	Teleport : ETeleportType //None(기본) 또는 TeleportPhysics(물리 객체를 워프)
)
*/

void AMyActor::RandomMove(float InDistance) {

	FVector ActorLocation = GetActorLocation();

	switch (FMath::RandRange(1, 4)) { //양 끝 포함
	case 1:
		ActorLocation += FVector(0, InDistance, 0);
		break;
	case 2:
		ActorLocation -= FVector(0, InDistance, 0);
		break;
	case 3:
		ActorLocation += FVector(InDistance, 0, 0);
		break;
	case 4:
		ActorLocation -= FVector(InDistance, 0, 0);
		break;
	}
	 
	if (ShouldTriggerZMove()) { //30퍼 확률로 z축으로 이동
		++Trigger_z200_cnt;
		ActorLocation += FVector(0, 0, InDistance);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("z200 triggered"));
		}
	}

	SetActorLocation(ActorLocation);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Move to %s"), *GetActorLocation().ToString())); 
	}
}

void AMyActor::RandomTurn_45() {

	FRotator ActorRotation = GetActorRotation();

	switch (FMath::RandRange(1, 3)) { //양 끝 포함
	case 1:
		ActorRotation += FRotator(45.f, 0.f, 0.f);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Pitch 45 degree"));
		}
		break;
	case 2:
		ActorRotation += FRotator(0.f, 45.f, 0.f);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Yaw 45 degree"));
		}
		break;
	case 3:
		ActorRotation += FRotator(0.f, 0.f, 45.f);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Roll 45 degree"));
		}
		break;
	}
	SetActorRotation(ActorRotation);
}


bool AMyActor::ShouldTriggerZMove() {
	return FMath::RandRange(1, 100) <= 30;
}

