// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

std::tuple<bool, AActor*, FVector> AEnemy::MoveTowardCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveTowardCharacter"));

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	DrawDebugLine(GetWorld(), GetActorLocation(), Character->GetActorLocation(), FColor::Green, true, -1, 0, 4);
	
	
	FHitResult HitResult;
	
	const FVector Start = GetActorLocation() + GetActorForwardVector();
	const FVector End = Start + GetActorForwardVector() * HitTestExtensionValue;


	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);

	bool HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_GameTraceChannel1,
			Sphere);
	
	return {HasHit, HitResult.GetActor(), GetClosestDirection(Character->GetActorLocation())};
}

void AEnemy::SetConnectorSplineRef(USplineComponent* Spline)
{
	EnemyConnector = Spline;
}

FVector AEnemy::GetClosestDirection(FVector CharacterLoc)
{
	FVector MyLoc = GetActorLocation();
	TArray<FVector> Directions = TArray({GetActorForwardVector(), -GetActorForwardVector(), GetActorUpVector(), -GetActorUpVector(), GetActorRightVector(), -GetActorRightVector()});
	float MinDist = 5000000;
	int MinIdx = 0;
	// TODO - Get the distance between the Character location and each direction, go in the shortest
	for(int i = 0; i < Directions.Num(); i++)
	{
		FVector Start = MyLoc + Directions[i] * HitTestExtensionValue;
		FVector End = CharacterLoc;
		float Dist = FVector::Dist(Start, End);
		if(Dist <= MinDist)
		{
			MinDist = Dist;
			MinIdx = i;
		}
	}
	
	return Directions[MinIdx];
}
