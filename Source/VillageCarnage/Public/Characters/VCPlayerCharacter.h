// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/VCBaseCharacter.h"
#include "VCPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;



UCLASS()
class VILLAGECARNAGE_API AVCPlayerCharacter : public AVCBaseCharacter
{
	GENERATED_BODY()
	

public:

	AVCPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	UCameraComponent* Camera;

	UFUNCTION(Blueprintcallable)
	void HitMePlz() 
	{ 
		AActor::TakeDamage(1.0f, FDamageEvent{}, GetController(), this); 
	}

	

	

protected:

	virtual void BeginPlay() override;

private:

	


};
