#include "MonGameInstance.h"

UMonGameInstance::UMonGameInstance()
{
	TotalScore = 0; 
}

void UMonGameInstance::AddScore(int32 Points)
{
	TotalScore += Points;
}

void UMonGameInstance::setMeilleurScore()
{
	if (MeilleureScore < TotalScore)
		MeilleureScore = TotalScore;
	
}

