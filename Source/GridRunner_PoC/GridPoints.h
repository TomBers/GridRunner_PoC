// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Connector.h"
#include "Enemy.h"
#include "PointActor.h"
#include "Engine/DataTable.h"
#include "GridPoints.generated.h"


UCLASS()
class GRIDRUNNER_POC_API AGridPoints : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridPoints();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveEnemies();
	void CreateEnemy(int Indx);
	void SetConnectorSpline(AActor* GeneratedActor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void TogglePointsVisible();

	USplineComponent* GetConnectorSpline();
	int GenerateRandomIndx();

	USplineComponent* BuildCharacterConnection(AActor* StartPoint, FVector Direction);
	void MaybeCreateNewNode(FVector Location, FVector Direction);
	USplineComponent* BuildEnemyConnection(AActor* StartPoint, FVector Direction);


private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APointActor> PointClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConnector> ConnectorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConnector> EnemyConnectorClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* GridData;

	
	TArray<APointActor*> SelectedPoints;
	TArray<APointActor*> Points;

	TArray<AConnector*> Connectors;

	TArray<AEnemy*> Enemies;

	FName SelectedTag = "GRABBED";

	USplineComponent* ConnectorSpline;

	void ClearPoints();
	void TurnOnPoint(APointActor* Point);
	void TurnOffPoint(APointActor* Point);
	void ConnectPoints(APointActor* StartPoint, APointActor* EndPoint);

	UPROPERTY(EditAnywhere)
	int NUM_ENEMY = 1;

	UPROPERTY(EditAnywhere)
	int NUM_X = 5;

	UPROPERTY(EditAnywhere)
	int NUM_Y = 5;

	UPROPERTY(EditAnywhere)
	int NUM_Z = 5;

	UPROPERTY(EditAnywhere)
	int GAP = 250;

	UPROPERTY(EditAnywhere)
	int Z_OFFSET = 50;

	UPROPERTY(EditAnywhere)
	bool PointsVisible = true;
};
