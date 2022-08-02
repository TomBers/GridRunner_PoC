// Fill out your copyright notice in the Description page of Project Settings.


#include "GridRunnerCharacter.h"

#include "GridPoints.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Subsystems/EditorActorSubsystem.h"

// Sets default values
AGridRunnerCharacter::AGridRunnerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup a Track for the character to ride on
	if (GridPoints)
	{
		ConnectorSplineRef = GridPoints->GetConnectorSpline();
	}
}

// Called every frame
void AGridRunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGridRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleVisible", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::TogglePointsVisible);
	// Build connectors in different directions


	PlayerInputComponent->BindAction("BuildConnectorUp", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::BuildConnectorUp);
	PlayerInputComponent->BindAction("BuildConnectorDown", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::BuildConnectorDown);
	PlayerInputComponent->BindAction("BuildConnectorLeft", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::BuildConnectorLeft);
	PlayerInputComponent->BindAction("BuildConnectorRight", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::BuildConnectorRight);
	PlayerInputComponent->BindAction("BuildConnectorStraight", EInputEvent::IE_Pressed, this,
	                                 &AGridRunnerCharacter::BuildConnectorStraight);

	PlayerInputComponent->BindAction("Rotate", EInputEvent::IE_Pressed, this,
									 &AGridRunnerCharacter::ChangeDirection);
}

void AGridRunnerCharacter::TogglePointsVisible()
{
	GridPoints->TogglePointsVisible();
}

void AGridRunnerCharacter::BuildConnector(FVector Direction)
{
	FHitResult HitResult;
	
	const FVector Start = GetArrowComponent()->GetComponentLocation() + GetArrowComponent()->GetForwardVector() *
		StartHitOffset;
	const FVector End = Start + GetArrowComponent()->GetForwardVector() * EndHitOffset;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, -1, 0, 4);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere);

	if (HasHit)
	{
		GridPoints->BuildConnection(HitResult.GetActor(), Direction);
		ConnectorSplineRef = GridPoints->GetConnectorSpline();
	}
}

void AGridRunnerCharacter::PrintRotation(FRotator Rot, FString PreMsg)
{
	FString RotString = Rot.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s :: %s"), *PreMsg, *RotString);
}

void AGridRunnerCharacter::BuildConnectorUp()
{
	BuildConnector(GetActorUpVector());
}

void AGridRunnerCharacter::BuildConnectorDown()
{
	BuildConnector(-GetActorUpVector());
}

void AGridRunnerCharacter::BuildConnectorLeft()
{
	BuildConnector(-GetActorRightVector());
}

void AGridRunnerCharacter::BuildConnectorRight()
{
	BuildConnector(GetActorRightVector());
}

void AGridRunnerCharacter::BuildConnectorStraight()
{
	BuildConnector(GetActorForwardVector());
}

void AGridRunnerCharacter::ChangeDirection()
{
	BuildConnector(-GetActorForwardVector());
}
