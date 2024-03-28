// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_ParameterSpecification.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_ParameterSpecification : public UObject
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;

	UPROPERTY()
	UClass* Type;

	UPROPERTY()
	UObject* DefaultValue;

	UPROPERTY()
	TArray<UObject*> PossibleValues;

	UPROPERTY()
	float MinValue;

	UPROPERTY()
	float MaxValue;
	
	void InitializeParameterSpecification(FString InName, UClass* InType, UObject* InDefaultValue)
	{
		Name = InName;
		Type = InType;
		DefaultValue = InDefaultValue;
	}

	void AddPossibleValue(UObject* Value)
	{
		PossibleValues.Add(Value);
	}

	void SetRange(float Min, float Max)
	{
		MinValue = Min;
		MaxValue = Max;
	}
};
