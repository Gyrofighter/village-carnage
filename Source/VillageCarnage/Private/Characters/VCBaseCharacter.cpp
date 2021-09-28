// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VCBaseCharacter.h"
#include "Components/VCHealthComponent.h"

// Sets default values
AVCBaseCharacter::AVCBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UVCHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void AVCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AVCBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



