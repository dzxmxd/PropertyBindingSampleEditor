// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "Modules/ModuleManager.h"

class USampleBindingObject;
class FToolBarBuilder;
class FMenuBuilder;

class FPropertyBindingSampleEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnTabClosed(TSharedRef<SDockTab> DockTab) const;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class IDetailsView> DetailsView;
	USampleBindingObject* SampleBindingObject = nullptr;
};