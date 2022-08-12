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

std::tuple<AActor*, FVector> AEnemy::MoveTowardCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveTowardCharacter"));

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	

	const FVector Start = GetActorLocation();
	DrawDebugLine(GetWorld(), Start, Character->GetActorLocation(), FColor::Green, true, -1, 0, 4);
	
	// Do the HitTest as with the Player character to get the Actor, then need to calculate which direction is closest!!

	// Find closest direction to character and build a connector there
	// Find Forward facing node
	// Step 1 - build connector in random direction
	// Step 2 - From node calculate distance between potential connector endpoint and character - build the connector to the one with the smallest distance??
	
	return {Character->GetParentActor(), FVector(0,0,0)};
}

void AEnemy::SetConnectorSplineRef(USplineComponent* Spline)
{
	EnemyConnector = Spline;
}
