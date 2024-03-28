// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_Bindings.generated.h"

class UAHTNAI_Variable;
class UAHTNAI_Parameter;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_Bindings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UAHTNAI_Variable* V;
	UPROPERTY()
	UAHTNAI_Parameter* P;

	void InitBindings(UAHTNAI_Variable* a_V, UAHTNAI_Parameter* a_P);

	bool Equals(UObject* o);

	FString ToString();
	
};
