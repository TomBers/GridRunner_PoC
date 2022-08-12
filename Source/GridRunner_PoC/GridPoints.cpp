// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPoints.h"


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

	for (int i = 0; i < NUM_X; i++)
	{
		for (int j = 0; j < NUM_Y; j++)
		{
			for (int k = 0; k < NUM_Z; k++)
			{
				int x = i * GAP;
				int y = j * GAP;
				int z = k * GAP + Z_OFFSET;
				FVector loc = FVector(x, y, z);
				//  TODO - add more defensive checks here incase of null's etc
				Points.Push(Cast<APointActor>(GetWorld()->SpawnActor(PointClass, &loc)));
			}
		}
	}

	// Generate an random initial connector
	if (ConnectorClass != nullptr)
	{
		// TODO - generate a random start position??
		// Should use function to gen??
		int indx = 8;
		FVector loc = Points[indx]->GetActorLocation();
		FRotator rot = Points[indx]->GetActorRotation();
		AActor* ConnActor = GetWorld()->SpawnActor(ConnectorClass, &loc, &rot);
		SetConnectorSpline(ConnActor);
	}

	// Generate Enemy Connector
	// for(int enemy = 0; enemy < NUM_ENEMY; enemy++)
	// {
		CreateEnemy(3);
	//}
}

void AGridPoints::CreateEnemy(int Indx)
{
	if (ConnectorClass != nullptr && EnemyClass != nullptr)
	{
		// Create Connector
		FVector loc = Points[Indx]->GetActorLocation();
		FRotator rot = Points[Indx]->GetActorRotation();
		AActor* ConnActor = GetWorld()->SpawnActor(ConnectorClass, &loc, &rot);

		AConnector* Connector = Cast<AConnector>(ConnActor);
		
		if(Connector)
		{
			AActor* EnemyActor = GetWorld()->SpawnActor(EnemyClass, &loc, &rot);

			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);

			if(Enemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("CreateEnemy"));
				Enemy->SetConnectorSplineRef(Connector->GetSpline());
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

void AGridPoints::BuildConnection(AActor* StartPoint, FVector Direction)
{
		FVector Location = StartPoint->GetActorLocation();
		FRotator Rot = Direction.Rotation();
		AActor* SpawnedActor = StartPoint->GetWorld()->SpawnActor(ConnectorClass, &Location, &Rot);
		SetConnectorSpline(SpawnedActor);
	
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
