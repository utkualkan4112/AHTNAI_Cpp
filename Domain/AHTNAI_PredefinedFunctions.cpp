// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_PreDefinedFunctions.h"

#include "AHTNAI_Function.h"
#include "AHTNAI_IntegerConstant.h"
#include "AHTNAI_Symboll.h"
#include "AHTNAI_VectorConstant.h"
#include "BaseClasses/BaseGameState.h"


int32 UAHTNAI_PreDefinedFunctions::DEBUG = 0;

TMap<UAHTNAI_Symboll*, UAHTNAI_PreDefinedFunctions::FunctionEvaluator*> UAHTNAI_PreDefinedFunctions::Functions;

UAHTNAI_Parameter* UAHTNAI_PreDefinedFunctions::Evaluate(UAHTNAI_Function* f, ABaseGameState* gs)
{
	FunctionEvaluator* fe = Functions.FindRef(f->Functor);
    
	if (fe == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PredefinedFunctions::evaluate: undefined function."));
		return nullptr;
	}
	return fe->Evaluate(f, gs);
}

// Continuing the UPredefinedFunctions implementation from before:

// Define our specific FunctionEvaluators
class NeighborPositionEvaluator : public UAHTNAI_PreDefinedFunctions::FunctionEvaluator 
{
public:
    UAHTNAI_Parameter* Evaluate(UAHTNAI_Function* f, ABaseGameState* gs) override 
    {
        if (f->Parameters.Num() != 2) return nullptr;

        UAHTNAI_IntegerConstant* ic1 = dynamic_cast<UAHTNAI_IntegerConstant*>(f->Parameters[0]);
        UAHTNAI_IntegerConstant* ic2 = dynamic_cast<UAHTNAI_IntegerConstant*>(f->Parameters[1]);

        if (ic1 && ic2) 
        {
            FVector Pos = FVector(ic1->value, ic2->value, 0);
            UAHTNAI_VectorConstant* NewConstant =  NewObject<UAHTNAI_VectorConstant>();
            NewConstant->Initilaze(Pos);
            return NewConstant;
        }
        return nullptr;
    }
};

class AddEvaluator : public UAHTNAI_PreDefinedFunctions::FunctionEvaluator 
{
public:
    UAHTNAI_Parameter* Evaluate(UAHTNAI_Function* f, ABaseGameState* gs) override 
    {
        if (f->Parameters.Num() != 2) return nullptr;

        UAHTNAI_IntegerConstant* ic1 = dynamic_cast<UAHTNAI_IntegerConstant*>(f->Parameters[0]);
        UAHTNAI_IntegerConstant* ic2 = dynamic_cast<UAHTNAI_IntegerConstant*>(f->Parameters[1]);

        if (ic1 && ic2) 
        {
            UAHTNAI_IntegerConstant* NewConstant = NewObject<UAHTNAI_IntegerConstant>();
            NewConstant->Initilaze(ic1->value + ic2->value);
            return NewConstant;
        }
        return nullptr;
    }
};

// Constructor implementation, which fills the functions map:
UAHTNAI_PreDefinedFunctions::UAHTNAI_PreDefinedFunctions() 
{
    // Create symbols
    UAHTNAI_Symboll* NeighborPositionSymbol = NewObject<UAHTNAI_Symboll>();
    UAHTNAI_Symboll* AddSymbol = NewObject<UAHTNAI_Symboll>();
    NeighborPositionSymbol->InitializeWithString("neighbor-position");
    AddSymbol->InitializeWithString("+");

    // Populate the functions map
    Functions.Add(NeighborPositionSymbol, new NeighborPositionEvaluator());
    Functions.Add(AddSymbol, new AddEvaluator());
}
