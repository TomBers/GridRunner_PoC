// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionsActor.h"
#include "Math/Vector.h"


// Sets default values
ADirectionsActor::ADirectionsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADirectionsActor::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebug(GetActorUpVector(), FColor::Magenta);
	DrawDebug(-GetActorUpVector(), FColor::Magenta);
	// DrawDebug(FRotator(-90,0,0), FColor::Magenta);
	//
	DrawDebug(GetActorRightVector(), FColor::Blue);
	DrawDebug(-GetActorRightVector(), FColor::Blue);
	// DrawDebug(FRotator(0,-90,0), FColor::Blue);
	//
	// DrawDebug(FRotator(0,0,90), FColor::Green);
	// DrawDebug(FRotator(0,0,-90), FColor::Green);

	DrawDebug(GetActorForwardVector(), FColor::Red);
	
}

// Called every frame
void ADirectionsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirectionsActor::DrawDebug(FVector Direction, FColor Col)
{
	FVector StartLine = GetActorLocation();

	FVector EndLine = StartLine + Direction * 100;

	DrawDebugDirectionalArrow(GetWorld(), StartLine, EndLine, 30.f, Col, true, -1.f, 0, 1.5f);
}

