// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_PreDefinedFunctions.generated.h"

/**
 * 
 */
class UAHTNAI_Parameter;
class UAHTNAI_Function;
class ABaseGameState;
class UAHTNAI_Symboll;
UCLASS()
class HITANDBOOM_API UAHTNAI_PreDefinedFunctions : public UObject
{
    GENERATED_BODY()

public:
	UAHTNAI_PreDefinedFunctions();

	static int32 DEBUG;

	class FunctionEvaluator 
	{
	public:
		virtual UAHTNAI_Parameter* Evaluate(UAHTNAI_Function* f, ABaseGameState* gs) = 0;
	};

private:
	static TMap<UAHTNAI_Symboll*, FunctionEvaluator*> Functions;

public:
	static UAHTNAI_Parameter* Evaluate(UAHTNAI_Function* f, ABaseGameState* gs);
};

