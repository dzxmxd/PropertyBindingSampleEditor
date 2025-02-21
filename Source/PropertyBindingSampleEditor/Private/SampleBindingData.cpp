#include "SampleBindingData.h"

bool FSampleBindingData::HasBinding() const
{
	if (PropertyBag == nullptr || PropertyBag->FindPropertyDescByName(PropertyDescName) == nullptr)
	{
		return false;
	}
	return true;
}

double FSampleBindingData::GetVectorValue()
{
	double Result = 0.114514;
	if (!PropertyBag || SourcePath.GetNumSegments() == 0)
	{
		return Result;
	}
	
	const FPropertyBagPropertyDesc* RootDesc = PropertyBag->FindPropertyDescByName(SourcePath.GetSegment(FirstValidSegmentIndex).GetName());
	if (!RootDesc)
	{
		return Result;
	}

	TValueOrError<FStructView, EPropertyBagResult> RootValueResult = PropertyBag->GetValueStruct(RootDesc->Name);
	if (!RootValueResult.IsValid())
	{
		return Result;
	}
	FStructView CurrentView = RootValueResult.GetValue();

	for (int32 i = FirstNestSegmentIndex; i < SourcePath.GetNumSegments(); ++i)
	{
		const FName SubPropertyName = SourcePath.GetSegment(i).GetName();
		const FProperty* SubProperty = CurrentView.GetScriptStruct()->FindPropertyByName(SubPropertyName);
		if (!SubProperty)
		{
			return Result;
		}
		
		if (const FStructProperty* StructProp = CastField<FStructProperty>(SubProperty))
		{
			CurrentView = FStructView(StructProp->Struct, static_cast<uint8*>(StructProp->ContainerPtrToValuePtr<void>(CurrentView.GetMemory())));
		}
		else
		{
			void* Value = SubProperty->ContainerPtrToValuePtr<void>(CurrentView.GetMemory());
			return *(static_cast<const double*>(Value));
		}
	}

	void* CurrentViewMemory = CurrentView.GetMemory();
	return *(static_cast<const double*>(CurrentViewMemory));
}

/*TValueOrError<FStructView, EPropertyBagResult> FSampleBindingData::GetStructView() const
{
	if (!PropertyBag || SourcePath.GetNumSegments() == 0)
	{
		return MakeError(EPropertyBagResult::TypeMismatch);
	}
		
	const FPropertyBagPropertyDesc* RootDesc = PropertyBag->FindPropertyDescByName(SourcePath.GetSegment(FirstValidBindingChainIndex).GetName());
	if (!RootDesc)
	{
		return MakeError(EPropertyBagResult::PropertyNotFound);
	}

	TValueOrError<FStructView, EPropertyBagResult> RootValueResult = PropertyBag->GetValueStruct(RootDesc->Name);
	if (!RootValueResult.IsValid())
	{
		return MakeError(RootValueResult.GetError());
	}
	FStructView CurrentView = RootValueResult.GetValue();

	for (int32 i = 0; i < SourcePath.GetNumSegments(); ++i)
	{
		// 下面的方案可以只存一个字符串
		// const FName SubPropertyName = SourcePath.GetSegment(i).GetName();
		// const FProperty* SubProperty = CurrentView.GetScriptStruct()->FindPropertyByName(SubPropertyName);
		const FProperty* SubProperty = SourcePath.GetSegment(i).GetField().Get<FProperty>();
		if (!SubProperty)
		{
			return MakeError(EPropertyBagResult::PropertyNotFound);
		}
			
		if (const FStructProperty* StructProp = CastField<FStructProperty>(SubProperty))
		{
			CurrentView = FStructView(StructProp->Struct, static_cast<uint8*>(StructProp->ContainerPtrToValuePtr<void>(CurrentView.GetMemory())));
		}
	}
	return MakeValue(CurrentView);
}*/