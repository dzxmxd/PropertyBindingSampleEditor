// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#include "PropertyBindingSampleEditor.h"
#include "PropertyBindingSampleEditorStyle.h"
#include "PropertyBindingSampleEditorCommands.h"
#include "SampleBindingObject.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "DetailCustomizations/SampleBindingExtension.h"

static const FName PropertyBindingSampleEditorTabName("PropertyBindingSampleEditor");

#define LOCTEXT_NAMESPACE "FPropertyBindingSampleEditorModule"

void FPropertyBindingSampleEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyBindingSampleEditorStyle::Initialize();
	FPropertyBindingSampleEditorStyle::ReloadTextures();

	FPropertyBindingSampleEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPropertyBindingSampleEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPropertyBindingSampleEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPropertyBindingSampleEditorModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PropertyBindingSampleEditorTabName, FOnSpawnTab::CreateRaw(this, &FPropertyBindingSampleEditorModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FPropertyBindingSampleEditorTabTitle", "PropertyBindingSampleEditor"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPropertyBindingSampleEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPropertyBindingSampleEditorStyle::Shutdown();

	FPropertyBindingSampleEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PropertyBindingSampleEditorTabName);
}

TSharedRef<SDockTab> FPropertyBindingSampleEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	SampleBindingObject = NewObject<USampleBindingObject>();
	SampleBindingObject->AddToRoot();

	// Details View
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.bShowPropertyMatrixButton = false;
	Args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyModule.CreateDetailView(Args);
	DetailsView->SetObject(SampleBindingObject);
	DetailsView->SetExtensionHandler(MakeShared<FSampleBindingExtension>());

	return SNew(SDockTab)
		.OnTabClosed_Raw(this, &FPropertyBindingSampleEditorModule::OnTabClosed)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				DetailsView.ToSharedRef()
			]
		];
}

void FPropertyBindingSampleEditorModule::OnTabClosed(TSharedRef<SDockTab> DockTab) const
{
	if (SampleBindingObject != nullptr)
	{
		SampleBindingObject->RemoveFromRoot();
	}
}

void FPropertyBindingSampleEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(PropertyBindingSampleEditorTabName);
}

void FPropertyBindingSampleEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FPropertyBindingSampleEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FPropertyBindingSampleEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPropertyBindingSampleEditorModule, PropertyBindingSampleEditor)