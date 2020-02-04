// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GitlabIntegration.h"
#include "GitlabIntegrationStyle.h"
#include "GitlabIntegrationCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "GitlabIntegrationSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"

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

	RegisterSettings();
}

void FGitlabIntegrationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGitlabIntegrationStyle::Shutdown();

	FGitlabIntegrationCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GitlabIntegrationTabName);

	UnregisterSettings();
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

bool FGitlabIntegrationModule::HandleSettingsSaved()
{
	UGitlabIntegrationSettings* Settings = GetMutableDefault<UGitlabIntegrationSettings>();
	Settings->SaveConfig();
	return true;
}

void FGitlabIntegrationModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "GitLabIntegration",
			LOCTEXT("RuntimeGeneralSettingsName", "GitLab Integration"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Config for the Gitlab Integration Plugin"),
			GetMutableDefault<UGitlabIntegrationSettings>()
			);

		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FGitlabIntegrationModule::HandleSettingsSaved);
		}
	}
}

void FGitlabIntegrationModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGitlabIntegrationModule, GitlabIntegration)