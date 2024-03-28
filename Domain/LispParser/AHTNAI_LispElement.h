// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_LispElement.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_LispElement : public UObject
{
	GENERATED_BODY()
public:
	FString Element;
	TArray<UAHTNAI_LispElement*> Children;

	UAHTNAI_LispElement();

	FORCEINLINE void Initialize(FString e) { Element = e; }

	FORCEINLINE FString ToString() { return ToStringInt(0); }

	FString ToStringInt(int32 Tabs);
	
};
