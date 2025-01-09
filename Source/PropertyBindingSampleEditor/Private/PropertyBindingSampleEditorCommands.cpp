// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#include "PropertyBindingSampleEditorCommands.h"

#define LOCTEXT_NAMESPACE "FPropertyBindingSampleEditorModule"

void FPropertyBindingSampleEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PropertyBindingSampleEditor", "Bring up PropertyBindingSampleEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE