// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "Framework/Commands/Commands.h"
#include "PropertyBindingSampleEditorStyle.h"

class FPropertyBindingSampleEditorCommands : public TCommands<FPropertyBindingSampleEditorCommands>
{
public:
	FPropertyBindingSampleEditorCommands()
		: TCommands<FPropertyBindingSampleEditorCommands>(TEXT("PropertyBindingSampleEditor"), NSLOCTEXT("Contexts", "PropertyBindingSampleEditor", "PropertyBindingSampleEditor Plugin"), NAME_None, FPropertyBindingSampleEditorStyle::GetStyleSetName()) {}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};