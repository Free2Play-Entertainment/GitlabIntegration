// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GitlabIntegration.h"
#include "GitlabIntegrationStyle.h"
#include "GitlabIntegrationCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName GitlabIntegrationTabName("GitlabIntegration");

#define LOCTEXT_NAMESPACE "FGitlabIntegrationModule"

void FGitlabIntegrationModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGitlabIntegrationStyle::Initialize();
	FGitlabIntegrationStyle::ReloadTextures();

	FGitlabIntegrationCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGitlabIntegrationCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGitlabIntegrationModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGitlabIntegrationModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FGitlabIntegrationModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GitlabIntegrationTabName, FOnSpawnTab::CreateRaw(this, &FGitlabIntegrationModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGitlabIntegrationTabTitle", "GitlabIntegration"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGitlabIntegrationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGitlabIntegrationStyle::Shutdown();

	FGitlabIntegrationCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GitlabIntegrationTabName);
}

TSharedRef<SDockTab> FGitlabIntegrationModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FGitlabIntegrationModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("GitlabIntegration.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FGitlabIntegrationModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(GitlabIntegrationTabName);
}

void FGitlabIntegrationModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FGitlabIntegrationCommands::Get().OpenPluginWindow);
}

void FGitlabIntegrationModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FGitlabIntegrationCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGitlabIntegrationModule, GitlabIntegration)