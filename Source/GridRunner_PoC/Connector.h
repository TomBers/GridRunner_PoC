// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Connector.generated.h"

UCLASS()
class GRIDRUNNER_POC_API AConnector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConnector();

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;
};
