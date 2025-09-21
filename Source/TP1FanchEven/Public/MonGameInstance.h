#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MonGameInstance.generated.h"

UCLASS()
class TP1FANCHEVEN_API UMonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMonGameInstance();
	
	UPROPERTY(BlueprintReadWrite, Category="Score")
	int32 TotalScore;
	int32 MeilleureScore;

	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category="Score")
	int getScore(){return TotalScore;};
	
	UFUNCTION(BlueprintCallable, Category="Score")
	int getMeilleurScore(){return MeilleureScore;};

	UFUNCTION(BlueprintCallable, Category="Score")
	void setMeilleurScore();

	UFUNCTION(BlueprintCallable, Category="Score")
	void resetScore(){TotalScore=0;};
};
