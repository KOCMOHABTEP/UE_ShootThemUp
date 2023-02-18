// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAICharacter.h"

#include "STUAIController.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTUAIController::StaticClass();
}
