// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#include "DetailCustomizations/SampleBindingExtension.h"

#include "DetailLayoutBuilder.h"
#include "IPropertyAccessEditor.h"
#include "IPropertyUtilities.h"
#include "SampleBindingObject.h"

#define LOCTEXT_NAMESPACE "GlobalVariablesBindingExtension"


bool FSampleBindingExtension::IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const
{
	if (PropertyHandle.GetMetaData("Category").Equals(TEXT("TestBinding")))
	{
		return true;
	}
	return false;
}

void FSampleBindingExtension::ExtendWidgetRow(FDetailWidgetRow& InWidgetRow, const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> InPropertyHandle)
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		return;
	}

	IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");
	// Array of structs we can bind to.
	TArray<FBindingContextStruct> BindingContextStructs;

	TArray<UObject*> OuterObjects;
	InPropertyHandle->GetOuterObjects(OuterObjects);
	if (OuterObjects.Num() == 1)
	{
		const FText DisplayText = FText::FromString(TEXT("Parameters"));
		const FString CategoryStr = TEXT("TestCategory");
		const FText SectionText = FText::FromString(TEXT("Global"));

		const USampleBindingObject* SampleBindingObject = Cast<USampleBindingObject>(OuterObjects[0]);
		if (SampleBindingObject != nullptr && SampleBindingObject->Parameters.IsValid())
		{
			const UStruct* Struct = SampleBindingObject->Parameters.GetPropertyBagStruct();
			FBindingContextStruct& ContextStruct = BindingContextStructs.AddDefaulted_GetRef();
			ContextStruct.DisplayText = DisplayText;
			ContextStruct.Struct = const_cast<UStruct*>(Struct);
			ContextStruct.Category = CategoryStr;
			ContextStruct.Section = SectionText;
			ContextStruct.Color = FLinearColor::MakeRandomColor();
		}
	}

	// Wrap value widget
	{
		/*auto IsValueVisible = TAttribute<EVisibility>::Create([CachedBindingData]() -> EVisibility
		{
			return CachedBindingData->HasBinding(FStateTreeEditorPropertyBindings::ESearchMode::Exact) ? EVisibility::Collapsed : EVisibility::Visible;
		});*/

		TSharedPtr<SWidget> ValueWidget = InWidgetRow.ValueContent().Widget;
		InWidgetRow.ValueContent()
		[
			SNew(SBox)
			.Visibility(EVisibility::Collapsed)
			// .Visibility(IsValueVisible)
			[
				ValueWidget.ToSharedRef()
			]
		];
	}

	FPropertyBindingWidgetArgs Args;
	Args.Property = InPropertyHandle->GetProperty();

	Args.OnCanBindPropertyWithBindingChain = FOnCanBindPropertyWithBindingChain::CreateLambda([](FProperty* InProperty, TConstArrayView<FBindingChainElement> InBindingChain) {
		return true;
	});

	Args.OnCanBindToContextStructWithIndex = FOnCanBindToContextStructWithIndex::CreateLambda([](const UStruct* InStruct, int32 InStructIndex) {
		return true;
	});

	Args.OnCanAcceptPropertyOrChildrenWithBindingChain = FOnCanAcceptPropertyOrChildrenWithBindingChain::CreateLambda([](FProperty* InProperty, TConstArrayView<FBindingChainElement> InBindingChain) {
		return true;
	});

	Args.OnCanBindToClass = FOnCanBindToClass::CreateLambda([](UClass* InClass) {
		return true;
	});

	Args.OnAddBinding = FOnAddBinding::CreateLambda([&InDetailBuilder](FName InPropertyName, TConstArrayView<FBindingChainElement> InBindingChain) {
		InDetailBuilder.GetPropertyUtilities()->RequestForceRefresh();
	});

	Args.OnRemoveBinding = FOnRemoveBinding::CreateLambda([&InDetailBuilder](FName InPropertyName) {
		InDetailBuilder.GetPropertyUtilities()->RequestForceRefresh();
	});

	Args.OnCanRemoveBinding = FOnCanRemoveBinding::CreateLambda([](FName InPropertyName) {
		return true;
	});

	Args.CurrentBindingText = MakeAttributeLambda([]() {
		return FText::FromString("TestBindingText");
	});

	Args.CurrentBindingToolTipText = MakeAttributeLambda([]() {
		return FText::FromString("TestToolTipText");
	});

	Args.CurrentBindingImage = MakeAttributeLambda([]() -> const FSlateBrush* {
		return FAppStyle::GetBrush("Icons.Warning");
	});

	Args.CurrentBindingColor = MakeAttributeLambda([]() -> FLinearColor {
		return FLinearColor::Red;
	});

	Args.BindButtonStyle = &FAppStyle::Get().GetWidgetStyle<FButtonStyle>("HoverHintOnly");
	Args.bAllowNewBindings = false;
	Args.bAllowArrayElementBindings = false;
	Args.bAllowUObjectFunctions = false;

	// ResetToDefault
	/*{
		InWidgetRow.CustomResetToDefault = FResetToDefaultOverride::Create(
			MakeAttributeLambda([CachedBindingData, InPropertyHandle]()
			{
				return InPropertyHandle->CanResetToDefault() || CachedBindingData->HasBinding(FStateTreeEditorPropertyBindings::ESearchMode::Includes);
			}),
			FSimpleDelegate::CreateLambda([CachedBindingData, &InDetailBuilder, InPropertyHandle]()
				{
					if (CachedBindingData->HasBinding(FStateTreeEditorPropertyBindings::ESearchMode::Includes))
					{
						CachedBindingData->RemoveBinding(FStateTreeEditorPropertyBindings::ESearchMode::Includes);
						InDetailBuilder.GetPropertyUtilities()->RequestForceRefresh();
					}
					if (InPropertyHandle->CanResetToDefault())
					{
						InPropertyHandle->ResetToDefault();
					}
				}),
			false
			);
	}*/

	InWidgetRow.ExtensionContent()
	[
		PropertyAccessEditor.MakePropertyBindingWidget(BindingContextStructs, Args)
	];
}

#undef LOCTEXT_NAMESPACE