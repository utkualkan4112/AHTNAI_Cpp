// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_PredefinedOperators.generated.h"

class UAHTNAI_PlayerAction;
class ABaseGameState;
class UAHTNAI_MethodDecomposition;
class UAHTNAI_Term;
class UAHTNAI_Symboll;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_PredefinedOperators : public UObject
{
	GENERATED_BODY()
public:
	UAHTNAI_PredefinedOperators();

	static int32 DEBUG;

	//static PathFinding pf = new AStarPathFinding();
	
	class OperatorExecutor 
	{
	public:
		virtual UAHTNAI_Parameter* Execute(UAHTNAI_Term* T, UAHTNAI_MethodDecomposition* State, ABaseGameState* Gs, UAHTNAI_PlayerAction* Pa) = 0;
	};
private:
	static TMap<UAHTNAI_Symboll*, OperatorExecutor*> Operators;

public:
	static UAHTNAI_Parameter* Execute(UAHTNAI_MethodDecomposition* State, ABaseGameState* Gs, UAHTNAI_PlayerAction* Pa);
};
