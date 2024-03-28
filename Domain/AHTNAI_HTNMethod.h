// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_MethodDecomposition.h"
#include "AHTNAI_Term.h"
#include "UObject/Object.h"
#include "AHTNAI_HTNMethod.generated.h"

class UAHTNAI_Symboll;
class UAHTNAI_LispElement;
class UAHTNAI_Bindings;
class UAHTNAI_MethodDecomposition;
class UAHTNAI_Term;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_HTNMethod : public UObject
{
	GENERATED_BODY()
private:
    FString Name;

    UPROPERTY()
    UAHTNAI_Term* Head;

    UPROPERTY()
    UAHTNAI_MethodDecomposition* Method;
public:
	
    void Initialize(FString N, UAHTNAI_Term* T, UAHTNAI_MethodDecomposition* M);
    
    FORCEINLINE FString GetName() { return Name; }
    
    FORCEINLINE  UAHTNAI_Term* GetHead() { return Head; }
    
    FORCEINLINE UAHTNAI_MethodDecomposition* GetDecomposition() { return Method; }
    
    UAHTNAI_HTNMethod* Clone();
    
    UAHTNAI_HTNMethod* CloneTrackingDescendants(TArray<UAHTNAI_MethodDecomposition*> DescendantsToTrack, TArray<UAHTNAI_MethodDecomposition*> NewDescendants);
        
    /*
    public HTNMethod clone(int renamingIndex) {
        HTNMethod c = new HTNMethod(name, head.clone(), method.clone());
        c.renameVariables(renamingIndex);
        return c;
    }
    */
    
    void RenameVariables(int32 RenamingIndex);
    
    void ApplyBindings(TArray<UAHTNAI_Bindings*> L);

    static UAHTNAI_HTNMethod* FromLispElement(UAHTNAI_LispElement* E);
    
    
    void ReplaceSingletonsByWildcards();
    
    TArray<UAHTNAI_Symboll*> FindSingletons();
    
    
    void CountVariableAppearances(TMap<UAHTNAI_Symboll*,int32>& Appearances);

    
    FORCEINLINE  FString ToString() { return "method("+Name+"): " + Head->ToString() + ", decomposition: " ; }     // Method will be added
    
    
    FORCEINLINE EExecutionType ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions, TArray<UAHTNAI_MethodDecomposition*> ChoicePoints)  { return Method->ExecutionCycle(Gs, Actions, ChoicePoints);}

    FORCEINLINE EExecutionType ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions, TArray<UAHTNAI_MethodDecomposition*> ChoicePoints, UAHTNAI_AdversarialChoicePoint* Previous_cp)  { return Method->ExecutionCycle(Gs, Actions, ChoicePoints, Previous_cp);}
};
