// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics: %s"), *PhysicsHandle->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}


void UGrabber::Grab()
{
	UPrimitiveComponent* HoldingObject = GetPhysicsHandle()->GetGrabbedComponent();

	if (HoldingObject == nullptr)
	{
		PickUpObject();
	}
	else
	{
		GetPhysicsHandle()->ReleaseComponent();
	}
}

void UGrabber::PickUpObject() const
{
	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		HitComp->WakeAllRigidBodies();

		HitResult.GetActor()->Tags.Add("GRABBED");

		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(
			HitComp,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Grabber Hit: %s"), *HitActor->GetName());

		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, true, 5);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, true, 5);
	}
}

void UGrabber::DrawDebug(FVector Start, FVector End) const
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
}

void UGrabber::GrabberRelease()
{
	UE_LOG(LogTemp, Warning, TEXT("Release Func"));
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	// Pickup object
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
}
