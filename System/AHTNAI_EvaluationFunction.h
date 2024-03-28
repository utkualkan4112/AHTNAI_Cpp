// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_EvaluationFunction.generated.h"

class ABaseGameState;
/**
 * 
 */
UCLASS(Abstract)
class HITANDBOOM_API UAHTNAI_EvaluationFunction : public UObject
{
	GENERATED_BODY()
public:
	static constexpr float VICTORY = 10000.0f;
	
	virtual float Evaluate(int32 MaxPlayer, int32 MinPlayer, ABaseGameState* Gs) PURE_VIRTUAL(UAHTNAI_EvaluationFunction::Evaluate, return 0.0f;);
    
	virtual float UpperBound(ABaseGameState* Gs) PURE_VIRTUAL(UAHTNAI_EvaluationFunction::UpperBound, return 0.0f;);

	virtual FString ToString()
	{
		return GetName();
	}
};
