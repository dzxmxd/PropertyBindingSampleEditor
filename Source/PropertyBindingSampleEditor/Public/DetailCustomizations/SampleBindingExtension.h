// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "IDetailPropertyExtensionHandler.h"

/**
 * c.f. FStateTreeBindingExtension
 * c.f. FAnimGraphNodeBindingExtension
 */
class FSampleBindingExtension final : public IDetailPropertyExtensionHandler
{
public:
	// IDetailPropertyExtensionHandler interface
	virtual bool IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const override;
	virtual void ExtendWidgetRow(FDetailWidgetRow& InWidgetRow, const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> PropertyHandle) override;
};