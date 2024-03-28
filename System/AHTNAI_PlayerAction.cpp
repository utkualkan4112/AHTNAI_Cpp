// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_PlayerAction.h"

UAHTNAI_PlayerAction::UAHTNAI_PlayerAction()
{
}

bool UAHTNAI_PlayerAction::Equals(UAHTNAI_PlayerAction* Other)
{
	return true;
}

bool UAHTNAI_PlayerAction::IsEmpty() const
{
	return true;
}

bool UAHTNAI_PlayerAction::HasNonNoneActions() const
{
	return true;
}

int32 UAHTNAI_PlayerAction::HasNumNoneActions() const
{
	return -1;
}

void UAHTNAI_PlayerAction::SetResourceUsage(UAHTNAI_ResourceUsage* NewResourceUsage)
{
}

void UAHTNAI_PlayerAction::AddUnitAction(UAHTNAI_Robot* Unit, UAHTNAI_RobotAction* Action)
{
}

void UAHTNAI_PlayerAction::RemoveUnitAction(UAHTNAI_Robot* Unit, UAHTNAI_RobotAction* Action)
{
}

UAHTNAI_PlayerAction* UAHTNAI_PlayerAction::Merge(UAHTNAI_PlayerAction* Other)
{
	return nullptr;
}

TArray<FRobotActionPair> UAHTNAI_PlayerAction::GetActions() const
{
	TArray<FRobotActionPair> trt;
	return trt;
}

UAHTNAI_RobotAction* UAHTNAI_PlayerAction::GetAction(UAHTNAI_Robot* Unit) const
{
	return nullptr;
}

TArray<UAHTNAI_PlayerAction*> UAHTNAI_PlayerAction::CartesianProduct(TArray<UAHTNAI_PlayerAction*> UnitActions,
	ABaseRobot* Unit, UAHTNAI_LightWeightGameState* State)
{
	TArray<UAHTNAI_PlayerAction*> trt;
	return trt;
}

bool UAHTNAI_PlayerAction::ConsistentWith(UAHTNAI_ResourceUsage* Usage, UAHTNAI_LightWeightGameState* State) const
{
	return true;
}

void UAHTNAI_PlayerAction::FillWithNones(UAHTNAI_LightWeightGameState* State, int32 PlayerID, int32 Duration)
{
}

bool UAHTNAI_PlayerAction::IntegrityCheck() const
{
	return true;
}

UAHTNAI_PlayerAction* UAHTNAI_PlayerAction::Clone()
{
	return nullptr;
}

void UAHTNAI_PlayerAction::Clear()
{
}

FString UAHTNAI_PlayerAction::ToString() const
{
	FString rtr;
	return rtr;
}
