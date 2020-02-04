// Copyright by Free2Play-Entertainment (2015-2020)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GitlabIntegrationSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class GITLABINTEGRATION_API UGitlabIntegrationSettings : public UObject
{
	GENERATED_BODY()
	
public:

	UGitlabIntegrationSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, config)
	FString APIKey;

	UPROPERTY(EditAnywhere, config)
	int32 ProjectID;
};
