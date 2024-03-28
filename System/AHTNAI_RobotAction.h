// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_RobotAction.generated.h"

class UBTTask_BaseClass;
class UBTTaskNode;
class AAdvancedAIController;
class UAHTNAI_ResourceUsage;
class ABaseRobot;
class ABaseItem;
/**
 * 
 */
UENUM(BlueprintType)
enum class EUnitAction : uint8
{
	Neutral,
	Stealth,
	Chasing,
	AttackTarget,
	AttackLocation,
	Move,
	Escape,
	BuyItem
};

UENUM(BlueprintType)
enum class EUnitAbilityType : uint8
{
	FirstSkill,
	SecondSkill,
	FourthSkill,
	FifthSkill
};



UCLASS()
class HITANDBOOM_API UAHTNAI_RobotAction : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UBTTask_BaseClass* ChasingTask;

	UPROPERTY()
	UBTTask_BaseClass* StealthTask;

	UPROPERTY()
	UBTTask_BaseClass* EscapeTask;

	UPROPERTY()
	UBTTask_BaseClass* BuyItemTask;

	UPROPERTY()
	UBTTask_BaseClass* FirstSkillTask;

	UPROPERTY()
	UBTTask_BaseClass* SecondSkillTask;

	UPROPERTY()
	UBTTask_BaseClass* FourthSkillTask;

	UPROPERTY()
	UBTTask_BaseClass* FifthSkillTask;
	
    TArray<FString> ActionName = {"neutral",
        "stealth",
        "chasing",
        "attack_target",
        "attack_location",
        "move",
        "escape",
        "buy_item"};

    EUnitAction Type = EUnitAction::Neutral;
	
	EUnitAbilityType AttackType = EUnitAbilityType::FirstSkill;

    FVector Direction = FVector::ZeroVector;

	int32 Parameter = 0;

	UPROPERTY()
    ABaseRobot* Robot;

	UPROPERTY()
	ABaseRobot* Target;

	UPROPERTY()
    UAHTNAI_ResourceUsage* R_Usage;
	UPROPERTY()
	AAdvancedAIController* AIController;

	UPROPERTY()
	ABaseItem* DesiredItem;

	UAHTNAI_RobotAction();

	void Initialize(EUnitAction type, int32 parameter, ABaseRobot* RobotType, FVector direction, UAHTNAI_RobotAction* Other, AAdvancedAIController* Controller);

	bool Equals(UObject* O);

	UAHTNAI_ResourceUsage* ResourceUsage(ABaseGameState* Gs);

	void Execute();
	
};
