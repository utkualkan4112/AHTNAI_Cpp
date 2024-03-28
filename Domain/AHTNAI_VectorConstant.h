// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Parameter.h"
#include "UObject/Object.h"
#include "AHTNAI_VectorConstant.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_VectorConstant : public UAHTNAI_Parameter
{
	GENERATED_BODY()
public:
	FVector Location;

	FORCEINLINE void Initilaze(FVector NewValue) { Location = NewValue; }

	FORCEINLINE FString ToString() const override { return "" + Location.ToString(); }

	FORCEINLINE virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s) override { return TOptional<TArray<UAHTNAI_Bindings*>>(); }

	FORCEINLINE virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs) override { return this; }

	FORCEINLINE virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l) override { return this; }

	FORCEINLINE virtual UAHTNAI_Parameter* CloneParameter() override { return this; }

	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchVector(const FVector& v) override;
	
	bool Equals(UObject* o);
};
