// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class GRIDRUNNER_POC_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite)
	USplineComponent* EnemyConnector;

	std::tuple<AActor*, FVector> MoveTowardCharacter();

	void SetConnectorSplineRef(USplineComponent* Spline);
	
};
