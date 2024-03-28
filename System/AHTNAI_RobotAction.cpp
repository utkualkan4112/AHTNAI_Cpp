// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_RobotAction.h"
#include "AdvancedAI/AdvancedAIController.h"
#include "AHTNAI_ResourceUsage.h"
#include "AdvancedAI/AHTNAI/BTTask_BaseClass.h"
#include "BaseClasses/BaseItem.h"
#include "BaseClasses/BaseRobot.h"
#include "BehaviorTree/BTTaskNode.h"

UAHTNAI_RobotAction::UAHTNAI_RobotAction()
{
	UObject* BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/PursueTask.uasset.PursueTask'"));
	UBlueprint* BPClassAsset = Cast<UBlueprint>(BPObject);
	UClass* MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			ChasingTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
	BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/FleeTask.uasset.FleeTask'"));
	BPClassAsset = Cast<UBlueprint>(BPObject);
	MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			EscapeTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
	BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/FirstSkillTask.uasset.FirstSkillTask'"));
	BPClassAsset = Cast<UBlueprint>(BPObject);
	MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			FirstSkillTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
	BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/SecondSkillTask.uasset.SecondSkillTask'"));
	BPClassAsset = Cast<UBlueprint>(BPObject);
	MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			SecondSkillTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
	BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/FourthSkillTask.uasset.FourthSkillTask'"));
	BPClassAsset = Cast<UBlueprint>(BPObject);
	MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			FourthSkillTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
	BPObject = StaticLoadObject(UBTTask_BaseClass::StaticClass(), nullptr, TEXT("Blueprint'C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/Blueprints/AI/Tasks/FifthSkillTask.uasset.FifthSkillTask'"));
	BPClassAsset = Cast<UBlueprint>(BPObject);
	MyTaskBPClass = nullptr;
	if(BPClassAsset)
	{
		MyTaskBPClass = BPClassAsset->GeneratedClass;
		if(MyTaskBPClass)
		{
			FifthSkillTask = NewObject<UBTTask_BaseClass>(GetTransientPackage(), MyTaskBPClass);
		}
	}
}

void UAHTNAI_RobotAction::Initialize(EUnitAction type, int32 parameter, ABaseRobot* RobotType, FVector direction,
                                     UAHTNAI_RobotAction* Other, AAdvancedAIController* Controller)
{
	if(Other)
	{
		Type = Other->Type;
		Parameter = Other->Parameter;
		Robot = Other->Robot;
		Direction = Other->Direction;
		AIController = Other->AIController;
	}
	else
	{
		if(type != EUnitAction::Neutral)
		{
			Type = type;
		}
		if(parameter != 0)
		{
			Parameter = parameter;
		}
		if(RobotType)
		{
			Robot = RobotType;
		}
		if(direction != FVector::ZeroVector)
		{
			Direction = direction;
		}
		if(Controller)
		{
			AIController = Controller;
		}
	}
	
	// KÜÇÜK BİR PATATES <3 
}

bool UAHTNAI_RobotAction::Equals(UObject* O)
{
	
	if (!Cast<UAHTNAI_RobotAction>(O)) {
		return false;
	}
	UAHTNAI_RobotAction* a = NewObject<UAHTNAI_RobotAction>();
	a->Initialize(EUnitAction::Neutral, 0, nullptr, FVector::ZeroVector, Cast<UAHTNAI_RobotAction>(O), nullptr);

	if(Type != a->Type) return false;
	if(Type == EUnitAction::BuyItem || Type == EUnitAction::Chasing || Type == EUnitAction::Escape || Type == EUnitAction::Move) return a->Parameter == Parameter;
	else if (Type == EUnitAction::AttackLocation) return a->Direction == Direction;
	else return a->Direction == Direction && a->Robot->RobotID == Robot->RobotID && a->Parameter == Parameter;
}

UAHTNAI_ResourceUsage* UAHTNAI_RobotAction::ResourceUsage(ABaseGameState* Gs)
{
	if (R_Usage != nullptr) {
		return R_Usage;
	}

	R_Usage = NewObject<UAHTNAI_ResourceUsage>();

	switch (Type) {
	case EUnitAction::Move: {
			FVector Pos = Robot->GetActorLocation();
			R_Usage->GetPositionsUsed().Add(Pos + Direction);
	}
		break;
	case EUnitAction::BuyItem: {
			R_Usage->SetResourcesUsed(DesiredItem->ItemData.EnergyCost); 
	}
		break;
	default:
		return nullptr;
	}
	

	return R_Usage;
}

void UAHTNAI_RobotAction::Execute()
{
	
	switch (Type)
	{
	case EUnitAction::Move:
		{
			if(AIController)
			{
				FAIMoveRequest NewRequest;
				NewRequest.SetUsePathfinding(true);
				NewRequest.SetGoalLocation(Direction);
				NewRequest.SetAcceptanceRadius(0.5);
				AIController->MoveTo(NewRequest);
			}
			break;
		}
	case EUnitAction::Chasing:
		{
			ChasingTask->SetEnemy(Target);
			ChasingTask->Execute(AIController, Robot);
			break;
		}
	case EUnitAction::Escape:
		{
			EscapeTask->SetFleeDistance(Parameter);
			EscapeTask->Execute(AIController, Robot);
			break;
		}
	case EUnitAction::AttackTarget:
		{
			/*
			switch (AttackType)
			{
			case EUnitAbilityType::FirstSkill:
				{
					//TO-DO change skill type with strategy
				}
			}*/
			FirstSkillTask->SetEnemy(Target);
			FirstSkillTask->Execute(AIController, Robot);
		}
	default:
		{
			break;
			return;
		}
		
	}
}

