// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#include "SampleBindingData.h"

bool FSampleBindingData::HasBinding() const
{
	if (PropertyBag == nullptr || !PropertyBag->IsValid() || PropertyBag->FindPropertyDescByName(PropertyDescName) == nullptr)
	{
		return false;
	}
	return true;
}

void FSampleBindingData::RemoveBinding()
{
	PropertyBag = nullptr;
	PropertyDescName = NAME_None;
	PropertyDescValueType = EPropertyBagPropertyType::None;
	SourcePath = FCachedPropertyPath();
	Color = FLinearColor::White;
	Image = nullptr;
}

FText FSampleBindingData::GetBindingText() const
{
	return FText::FromString(SourcePath.ToString());
}