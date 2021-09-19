// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/VCPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"



AVCPlayerController::AVCPlayerController()
{
	bEnableTouchEvents = false;
	bShowMouseCursor = true;

	PathFollowing = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowing"));
}



void AVCPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AVCPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (bTouched)
	{
		MoveCharacter();
	}
}

void AVCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AVCPlayerController::TouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AVCPlayerController::TouchReleased);
}

void AVCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (PathFollowing)
	{
		PathFollowing->Initialize();
	}	
}

void AVCPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (PathFollowing)
	{
		PathFollowing->Cleanup();
	}
}

void AVCPlayerController::TouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bTouched = true;
	PausePathFollowing();
}

void AVCPlayerController::TouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{		
	bTouched = false;

	FVector2D ScreenLocation(Location);	
	FHitResult HitResult;

	GetHitResultAtScreenPosition(ScreenLocation, TouchTraceType, false, HitResult);

	if (!HitResult.bBlockingHit)
	{
		return;
	}
	else
	{
		GoToLocation(HitResult.ImpactPoint);
	}	
}

void AVCPlayerController::MoveCharacter()
{
	if (!GetPawn()) return;
	
	FHitResult HitResult;
	GetHitResultUnderFingerByChannel(ETouchIndex::Touch1, TouchTraceType, false, HitResult);

	if (!HitResult.bBlockingHit) return;
	
	FVector PawnLocation = GetPawn()->GetActorLocation();

	FVector DirectionUnitVector = UKismetMathLibrary::GetDirectionUnitVector(PawnLocation, HitResult.ImpactPoint);
	FVector WorldDirection = UKismetMathLibrary::Vector_Normal2D(DirectionUnitVector);
	
	GetPawn()->AddMovementInput(WorldDirection);
}

void AVCPlayerController::GoToLocation(const FVector Location)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerGoToLocation(Location);
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (!NavSys || !GetPawn() || !PathFollowing) return;

	if (!PathFollowing->IsPathFollowingAllowed())
	{
		PathFollowing->Initialize();
		if (!PathFollowing->IsPathFollowingAllowed()) return;

	}
	
	const bool bAlreadyAtGoal = PathFollowing->HasReached(Location, EPathFollowingReachMode::OverlapAgent);

	
	if (PathFollowing->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowing->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	}

	
	if (PathFollowing->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowing->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}

	if (bAlreadyAtGoal)
	{
		PathFollowing->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	}
	else
	{
		const FVector AgentNavLocation = GetNavAgentLocation();
		const ANavigationData* NavData = NavSys->GetNavDataForProps(GetNavAgentPropertiesRef(), AgentNavLocation);
		if (NavData)
		{
			FPathFindingQuery Query(this, *NavData, AgentNavLocation, Location);
			FPathFindingResult Result = NavSys->FindPathSync(Query);
			if (Result.IsSuccessful())
			{
				PathFollowing->RequestMove(FAIMoveRequest(Location), Result.Path);
			}
			else if (PathFollowing->GetStatus() != EPathFollowingStatus::Idle)
			{
				PathFollowing->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
			}
		}
	}
}



void AVCPlayerController::PausePathFollowing()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPausePathFollowing();
	}
	PathFollowing->PauseMove(FAIRequestID::CurrentRequest, false);
}



///////////////////////////////SERVER SIDE//////////////////////////////

void AVCPlayerController::ServerGoToLocation_Implementation(const FVector Location)
{
	GoToLocation(Location);
}


void AVCPlayerController::ServerPausePathFollowing_Implementation()
{
	PausePathFollowing();
}