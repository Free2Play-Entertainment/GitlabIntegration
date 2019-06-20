// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GitlabIntegrationCommands.h"

#define LOCTEXT_NAMESPACE "FGitlabIntegrationModule"

void FGitlabIntegrationCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GitlabIntegration", "Bring up GitlabIntegration window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
