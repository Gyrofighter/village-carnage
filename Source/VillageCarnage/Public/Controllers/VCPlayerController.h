// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VCPlayerController.generated.h"

class UPathFollowingComponent;

UCLASS()
class VILLAGECARNAGE_API AVCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AVCPlayerController();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Control)
	TEnumAsByte<ETraceTypeQuery> TouchTraceType = ETraceTypeQuery::TraceTypeQuery1;


protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void PlayerTick(float DeltaTime) override;

private:	

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UPathFollowingComponent* PathFollowing;

	bool bTouched = false;

	void TouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

	void MoveCharacter();

	void GoToLocation(const FVector Location);

	void PausePathFollowing();

	//Server side
	UFUNCTION(Server, Reliable)
	void ServerGoToLocation(const FVector Location);

	UFUNCTION(Server, Reliable)
	void ServerPausePathFollowing();

	
};
