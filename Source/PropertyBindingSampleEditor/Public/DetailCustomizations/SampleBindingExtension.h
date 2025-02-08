// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "IDetailPropertyExtensionHandler.h"

/**
 * Most references
 * c.f. FBlueprintWidgetCustomization
 * c.f. FBlueprintMemberReferenceDetails::CustomizeHeader
 * Other references
 * c.f. FStateTreeBindingExtension
 * c.f. FAnimGraphNodeBindingExtension
 * c.f. UAnimGraphNodeBinding_Base
 * c.f. FSmartObjectDefinitionBindingExtension
 */
class FSampleBindingExtension final : public IDetailPropertyExtensionHandler
{
public:
	// IDetailPropertyExtensionHandler interface
	virtual bool IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const override;
	virtual void ExtendWidgetRow(FDetailWidgetRow& InWidgetRow, const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> PropertyHandle) override;
};