// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPoints.h"

#include "GridPointStruct.h"


// Sets default values
AGridPoints::AGridPoints()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridPoints::BeginPlay()
{
	Super::BeginPlay();

	if(GridData)
	{
		TArray<FGridPointStruct *> Rows;
		GridData->GetAllRows(TEXT(""), Rows);
		for(FGridPointStruct* Row : Rows)
		{
			float rx = Row->x;
			float ry = Row->y;
			float rz = Row->z;
			FVector rloc = FVector(rx, ry, rz);
			Points.Push(Cast<APointActor>(GetWorld()->SpawnActor(PointClass, &rloc)));
			UE_LOG(LogTemp, Warning, TEXT("x %f, y %f, z %f"), rx, ry, rz);
		}
	}
	

	// for (int i = 0; i < NUM_X; i++)
	// {
	// 	for (int j = 0; j < NUM_Y; j++)
	// 	{
	// 		for (int k = 0; k < NUM_Z; k++)
	// 		{
	// 			int x = i * GAP;
	// 			int y = j * GAP;
	// 			int z = k * GAP + Z_OFFSET;
	// 			FVector loc = FVector(x, y, z);
	// 			//  TODO - add more defensive checks here incase of null's etc
	// 			Points.Push(Cast<APointActor>(GetWorld()->SpawnActor(PointClass, &loc)));
	// 		}
	// 	}
	// }

	// Generate an random initial connector
	if (ConnectorClass != nullptr)
	{
		int indx = 1;//GenerateRandomIndx();
		// UE_LOG(LogTemp, Warning, TEXT("Character spawned at %i"), indx);
		FVector sloc = Points[indx]->GetActorLocation();
		FVector eloc = Points[indx + 1]->GetActorLocation();
		FVector diff = eloc - sloc;
		
		FRotator rot = diff.Rotation();
		
		// FRotator rot = Points[indx]->GetActorRotation();

		// TODO - set Size to the distance between points
		AActor* ConnActor = GetWorld()->SpawnActor(ConnectorClass, &sloc, &rot);
		ConnActor->SetActorScale3D(FVector(diff.Length() / 100, 1.0, 1.0));
		SetConnectorSpline(ConnActor);
	}

	// Generate Enemy Connector
	for(int enemy = 0; enemy < NUM_ENEMY; enemy++)
	{
		CreateEnemy(GenerateRandomIndx());
	}
	
	// Setup Internal timer to move towards enemy every X secs
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGridPoints::MoveEnemies, 5.f, true);
}

void AGridPoints::MoveEnemies()
{
	for (AEnemy* Enemy : Enemies)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s"), *Enemy->GetName());
		auto [HasHit, Actor, Direction] = Enemy->MoveTowardCharacter();

		if(HasHit)
		{
			Enemy->SetConnectorSplineRef(BuildEnemyConnection(Actor, Direction));
		}
	}
}

void AGridPoints::CreateEnemy(int Indx)
{
	if (EnemyConnectorClass != nullptr && EnemyClass != nullptr)
	{
		// Create Connector
		FVector loc = Points[Indx]->GetActorLocation();
		FRotator rot = Points[Indx]->GetActorRotation();
		AActor* ConnActor = GetWorld()->SpawnActor(EnemyConnectorClass, &loc, &rot);

		AConnector* Connector = Cast<AConnector>(ConnActor);
		
		if(Connector)
		{
			AActor* EnemyActor = GetWorld()->SpawnActor(EnemyClass, &loc, &rot);

			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);

			if(Enemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("CreateEnemy"));
				Enemy->SetConnectorSplineRef(Connector->GetSpline());
				Enemies.Push(Enemy);
			}
		}
		
	}
}

void AGridPoints::SetConnectorSpline(AActor* GeneratedActor)
{
	AConnector* Connector = Cast<AConnector>(GeneratedActor);

	if (Connector)
	{
		Connectors.Push(Connector);
		ConnectorSpline = Connector->GetSpline();
	}
}

// Called every frame
void AGridPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridPoints::ClearPoints()
{
	for (APointActor* Point : SelectedPoints)
	{
		Point->Tags.Remove(SelectedTag);
	}
	SelectedPoints.Empty();
}

void AGridPoints::TurnOnPoint(APointActor* Point)
{
	SelectedPoints.Push(Point);
	Point->Tags.Add(SelectedTag);
}

void AGridPoints::TurnOffPoint(APointActor* Point)
{
	SelectedPoints.Remove(Point);
	Point->Tags.Remove(SelectedTag);
}


void AGridPoints::ConnectPoints(APointActor* StartPoint, APointActor* EndPoint)
{
	FVector sloc = StartPoint->GetActorLocation();
	FVector diff = EndPoint->GetActorLocation() - sloc;
	FRotator rot = diff.Rotation();

	GetWorld()->SpawnActor(ConnectorClass, &sloc, &rot);
}

USplineComponent* AGridPoints::BuildCharacterConnection(AActor* StartPoint, FVector Direction)
{
		FVector Location = StartPoint->GetActorLocation();
		FRotator Rot = Direction.Rotation();
		AActor* SpawnedActor = StartPoint->GetWorld()->SpawnActor(ConnectorClass, &Location, &Rot);
		SetConnectorSpline(SpawnedActor);

		AConnector* Connector = Cast<AConnector>(SpawnedActor);
	
		MaybeCreateNewNode(Location, Direction);
	
		return Connector->GetSpline();
}

void AGridPoints::MaybeCreateNewNode(FVector Location, FVector Direction)
{
	FVector LineStart = Location;
	FVector LineEnd = LineStart + Direction * GAP;

	FHitResult HitResult;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);

	bool HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			LineStart,
			LineEnd,
			FQuat::Identity,
			ECC_GameTraceChannel1,
			Sphere);

	if(!HasHit)
	{
		//  TODO - add more defensive checks here incase of null's etc
		Points.Push(Cast<APointActor>(GetWorld()->SpawnActor(PointClass, &LineEnd)));
	}
	
}

USplineComponent* AGridPoints::BuildEnemyConnection(AActor* StartPoint, FVector Direction)
{
	FVector Location = StartPoint->GetActorLocation();
	FRotator Rot = Direction.Rotation();
	AActor* SpawnedActor = StartPoint->GetWorld()->SpawnActor(EnemyConnectorClass, &Location, &Rot);

	MaybeCreateNewNode(Location, Direction);
	
	AConnector* Connector = Cast<AConnector>(SpawnedActor);
	return Connector->GetSpline();
}

void AGridPoints::TogglePointsVisible()
{
	if (PointsVisible)
	{
		for (APointActor* Point : Points)
		{
			Point->Tags.Add("HIDDEN");
		}
	}
	else
	{
		for (APointActor* Point : Points)
		{
			Point->Tags.Remove("HIDDEN");
		}
	}

	PointsVisible = !PointsVisible;
}

USplineComponent* AGridPoints::GetConnectorSpline()
{
	return ConnectorSpline;
}

int AGridPoints::GenerateRandomIndx()
{
	int StartingPositions[10] = {8, 20, 33, 44, 55, 66, 77, 88, 99, 111};
	int RandIdx = rand() % 10;
	// int max = NUM_X * NUM_Y * NUM_Z;
	// return rand() % max;
	return 2; //StartingPositions[RandIdx];
}
