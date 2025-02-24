// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#include "DetailCustomizations/SampleBindingExtension.h"

#include "IPropertyAccessEditor.h"
#include "PropertyPathHelpers.h"
#include "SampleBindingObject.h"

#define LOCTEXT_NAMESPACE "GlobalVariablesBindingExtension"


bool FSampleBindingExtension::IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const
{
	if (!PropertyHandle.GetProperty())
	{
		return false;
	}
	if (FindFProperty<FProperty>(InObjectClass, FName(*(PropertyHandle.GetProperty()->GetName() + TEXT("BindingData")))))
	{
		return true;
	}
	return false;
}

void FSampleBindingExtension::ExtendWidgetRow(FDetailWidgetRow& InWidgetRow, const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		return;
	}

	IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");

	TArray<UObject*> OuterObjects;
	PropertyHandle->GetOuterObjects(OuterObjects);
	if (OuterObjects.Num() != 1)
	{
		return;
	}

	USampleBindingObject* SampleBindingObject = Cast<USampleBindingObject>(OuterObjects[0]);
	if (SampleBindingObject == nullptr || !SampleBindingObject->Parameters.IsValid())
	{
		return;
	}

	// TODO: Changed this to get real InstancedPropertyBag params.
	FInstancedPropertyBag& PropertyBag = SampleBindingObject->Parameters;

	if (!PropertyHandle->GetProperty())
	{
		return;
	}
	const FProperty* BindingDataProperty = FindFProperty<FProperty>(InObjectClass, FName(*(PropertyHandle->GetProperty()->GetName() + TEXT("BindingData"))));
	if (BindingDataProperty == nullptr)
	{
		return;
	}
	void* BindingDataAddress = BindingDataProperty->ContainerPtrToValuePtr<void>(SampleBindingObject);
	if (BindingDataAddress == nullptr)
	{
		return;
	}
	if (CastField<FStructProperty>(BindingDataProperty) == nullptr)
	{
		return;
	}
	FSampleBindingData* SampleBindingData = static_cast<FSampleBindingData*>(BindingDataAddress);
	if (SampleBindingData == nullptr)
	{
		return;
	}

	const FText DisplayText = FText::FromString(TEXT("Parameters"));
	const FString CategoryStr = TEXT("TestCategory");
	const FText SectionText = FText::FromString(TEXT("Global"));
	TArray<FBindingContextStruct> BindingContextStructs;
	FBindingContextStruct& ContextStruct = BindingContextStructs.AddDefaulted_GetRef();
	ContextStruct.DisplayText = DisplayText;
	const UStruct* Struct = PropertyBag.GetPropertyBagStruct();
	ContextStruct.Struct = const_cast<UStruct*>(Struct);
	// ContextStruct.Category = CategoryStr;
	ContextStruct.Section = SectionText;
	ContextStruct.Color = FLinearColor::MakeRandomColor();

	FPropertyBindingWidgetArgs Args;
	Args.OnGenerateBindingName = FOnGenerateBindingName::CreateLambda([]() -> FString { return TEXT("NewBinding"); });
	Args.OnCanBindPropertyWithBindingChain = FOnCanBindPropertyWithBindingChain::CreateLambda([PropertyHandle](FProperty* InProperty, TConstArrayView<FBindingChainElement> InBindingChain)
	{
		if (InProperty == nullptr || InBindingChain.IsEmpty())
		{
			return true;
		}
		IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");
		const FProperty* BindingProperty = PropertyHandle->GetProperty();
		const bool Result = BindingProperty && PropertyAccessEditor.GetPropertyCompatibility(InProperty, BindingProperty) == EPropertyAccessCompatibility::Compatible;
		return Result;
	});
	Args.OnCanBindToClass = FOnCanBindToClass::CreateLambda([](UClass* InClass) { return true; });
	Args.OnAddBinding = FOnAddBinding::CreateLambda([SampleBindingData, PropertyBag, PropertyHandle](const FName& InPropertyName, const TArray<FBindingChainElement>& InBindingChain)
	{
		TArray<FFieldVariant> FieldChain;
		Algo::Transform(InBindingChain, FieldChain, [](const FBindingChainElement& InElement) {
			return InElement.Field;
		});
		FCachedPropertyPath CachedPropertyPath;
		TArray<FString> PropertyChain;
		for (FFieldVariant Field : FieldChain)
		{
			if (const FProperty* CurProperty = Field.Get<FProperty>())
			{
				FName MemberName = CurProperty->GetFName();
				PropertyChain.Add(MemberName.ToString());
			}
		}
		if (PropertyChain.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("%s:%d Binding chain is empty."), *FString(__FUNCTION__), __LINE__);
			return;
		}
		SampleBindingData->SourcePath = FCachedPropertyPath(PropertyChain);
		SampleBindingData->PropertyBag = &PropertyBag;
		const FPropertyBagPropertyDesc* PropertyDesc = PropertyBag.FindPropertyDescByName(
			SampleBindingData->SourcePath.GetSegment(FSampleBindingData::FirstValidSegmentIndex).GetName());
		SampleBindingData->PropertyDescName = PropertyDesc->Name;
		SampleBindingData->PropertyDescValueType = PropertyDesc->ValueType;

		// Image
		static FName PropertyIcon(TEXT("Kismet.Tabs.Variables"));
		SampleBindingData->Image = FAppStyle::GetBrush(PropertyIcon);

		// Color
		const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
		check(Schema);
		FEdGraphPinType PinType;
		Schema->ConvertPropertyToPinType(PropertyHandle->GetProperty(), PinType);
		SampleBindingData->Color = Schema->GetPinTypeColor(PinType);
	});
	Args.OnRemoveBinding = FOnRemoveBinding::CreateLambda([SampleBindingData](FName InPropertyName)
	{
		SampleBindingData->PropertyBag = nullptr;
		SampleBindingData->PropertyDescName = NAME_None;
		SampleBindingData->PropertyDescValueType = EPropertyBagPropertyType::None;
		SampleBindingData->Color = FLinearColor::White;
		SampleBindingData->Image = nullptr;
	});
	Args.OnCanRemoveBinding = FOnCanRemoveBinding::CreateLambda([](FName InPropertyName)
	{
		return true;
	});
	Args.OnHasAnyBindings = FOnHasAnyBindings::CreateLambda([SampleBindingData]()
	{
		return SampleBindingData->HasBinding();
	});
	Args.CurrentBindingText = MakeAttributeLambda([SampleBindingData]()
	{
		return FText::FromName(SampleBindingData->PropertyDescName);
	});
	Args.CurrentBindingToolTipText = MakeAttributeLambda([SampleBindingData]()
	{
		return FText::FromName(SampleBindingData->PropertyDescName);
	});
	Args.CurrentBindingImage = MakeAttributeLambda([SampleBindingData]() -> const FSlateBrush*
	{
		return SampleBindingData->Image;
	});
	Args.CurrentBindingColor = MakeAttributeLambda([SampleBindingData]() -> FLinearColor
	{
		return SampleBindingData->Color;
	});

	// Configure binding options
	Args.bGeneratePureBindings = true;          // Allow pure bindings
	Args.bAllowFunctionBindings = false;        // Disallow function bindings
	Args.bAllowFunctionLibraryBindings = false; // Disallow function library bindings
	Args.bAllowPropertyBindings = true;         // Allow property bindings
	Args.bAllowNewBindings = false;             // Disallow creating new bindings
	Args.bAllowArrayElementBindings = false;    // Disallow array element bindings
	Args.bAllowUObjectFunctions = false;        // Disallow UObject functions
	Args.bAllowStructFunctions = false;         // Disallow struct functions
	Args.bAllowStructMemberBindings = true;     // Allow struct member bindings

	TAttribute<bool> bOriginValueContentWidgetEnabled = TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda([SampleBindingData]() -> bool
	{
		if (SampleBindingData != nullptr)
		{
			return !SampleBindingData->HasBinding();
		}
		return true;
	}));
	InWidgetRow.ValueContent().Widget->SetEnabled(bOriginValueContentWidgetEnabled);
	// Customize the header row in the Details Panel to include the PropertyAccessEditor widget
	InWidgetRow.ExtensionContent()
	[
		PropertyAccessEditor.MakePropertyBindingWidget(BindingContextStructs, Args)
	];

}

#undef LOCTEXT_NAMESPACE