// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GridPointStruct.generated.h"

/**
 * 
 */

USTRUCT()
struct FGridPointStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float x;

	UPROPERTY()
	float y;

	UPROPERTY()
	float z;
};