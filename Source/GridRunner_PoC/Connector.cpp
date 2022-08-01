// Fill out your copyright notice in the Description page of Project Settings.


#include "Connector.h"

// Sets default values
AConnector::AConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Spline and SplineMesh

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene");
	if(SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}
	

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SplineComponent->SetupAttachment(SceneComponent);
	} 
	

}

USplineComponent* AConnector::GetSpline()
{
	return SplineComponent;
}

// Called when the game starts or when spawned
void AConnector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

