// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VCPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AVCPlayerCharacter::AVCPlayerCharacter()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bInheritYaw = false;
	SpringArm->SetRelativeRotation(FRotator(300.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

	


}

void AVCPlayerCharacter::BeginPlay()
{


	Super::BeginPlay();

	

}


