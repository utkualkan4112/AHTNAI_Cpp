// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_Bindings.h"
#include "BaseClasses/BaseGameState.h"
#include "AHTNAI_Parameter.generated.h"

/**
 * 
 */


UCLASS()
class HITANDBOOM_API UAHTNAI_Parameter : public UObject
{
	GENERATED_BODY()
public:
    virtual FString ToString() const;
	
	virtual TOptional<TArray<UAHTNAI_Bindings*>> Match(int32 v);
	
	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s);

	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchVector(const FVector& v);
	
	virtual UAHTNAI_Parameter* CloneParameter();
	
	virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l);

	// applies all the bindings and evaluates in case it is a function:
	virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs);
};
