// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPoints.h"
#include "GameFramework/Character.h"
#include "GridRunnerCharacter.generated.h"

UCLASS()
class GRIDRUNNER_POC_API AGridRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGridRunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildConnector(FVector Direction);
	void PrintRotation(FRotator Rot, FString PreMsg);
	void BuildConnectorUp();
	void BuildConnectorDown();
	void BuildConnectorLeft();
	void BuildConnectorRight();
	void BuildConnectorStraight();
	void ChangeDirection();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TogglePointsVisible();

	UPROPERTY(BlueprintReadWrite)
	USplineComponent* ConnectorSplineRef;

	UPROPERTY(EditAnywhere)
	float StartHitOffset = 0.f;

	UPROPERTY(EditAnywhere)
	float EndHitOffset = 100.f;

	UPROPERTY(EditAnywhere)
	float X_OFFSET = 0.f;

	UPROPERTY(EditAnywhere)
	float Y_OFFSET = 0.f;

	UPROPERTY(EditAnywhere)
	float Z_OFFSET = 0.f;

private:
	// TODO - find a way of not tieing this to a particular instance, it crashed as it was pointing to the GridPoints of another level
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<AGridPoints> GridPoints;
	
};
