// MyActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class HW05_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();

protected:
	virtual void BeginPlay() override;

public:	
	void RandomMove(float InDistance);
	void RandomTurn_45();
	bool ShouldTriggerZMove();

private:
	int Trigger_z200_cnt = 0;
	float TravelDistance = 0.f;
};
