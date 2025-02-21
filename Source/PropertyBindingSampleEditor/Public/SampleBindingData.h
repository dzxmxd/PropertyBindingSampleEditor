#pragma once

#include "PropertyPathHelpers.h"
#include "StructUtils/PropertyBag.h"
#include "SampleBindingData.generated.h"

USTRUCT()
struct FSampleBindingData
{
	GENERATED_BODY()
public:
	static constexpr int32 FirstValidSegmentIndex = 0;
	static constexpr int32 FirstNestSegmentIndex = 1;

	const FInstancedPropertyBag* PropertyBag = nullptr;
	FName PropertyDescName;
	EPropertyBagPropertyType PropertyDescValueType = EPropertyBagPropertyType::None;
	FCachedPropertyPath SourcePath;
	FLinearColor Color = FLinearColor::White;
	const FSlateBrush* Image = nullptr;

	bool HasBinding() const;

	double GetVectorValue();


	template <typename T>
	TValueOrError<T, EPropertyBagResult> GetNestedPropertyValue() const
	{
		if (!PropertyBag || SourcePath.GetNumSegments() == 0)
		{
			return MakeError(EPropertyBagResult::TypeMismatch);
		}
		
		const FPropertyBagPropertyDesc* RootDesc = PropertyBag->FindPropertyDescByName(SourcePath.GetSegment(FirstValidSegmentIndex).GetName());
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

		for (int32 i = FirstNestSegmentIndex; i < SourcePath.GetNumSegments(); ++i)
		{
			const FName SubPropertyName = SourcePath.GetSegment(i).GetName();
			const FProperty* SubProperty = CurrentView.GetScriptStruct()->FindPropertyByName(SubPropertyName);
			if (!SubProperty)
			{
				return MakeError(EPropertyBagResult::PropertyNotFound);
			}
			
			if (const FStructProperty* StructProp = CastField<FStructProperty>(SubProperty))
			{
				CurrentView = FStructView(StructProp->Struct, static_cast<uint8*>(StructProp->ContainerPtrToValuePtr<void>(CurrentView.GetMemory())));
			}
			else
			{
				void* Value = SubProperty->ContainerPtrToValuePtr<void>(CurrentView.GetMemory());
				return MakeValue(*(static_cast<const T*>(Value)));
			}
		}

		void* CurrentViewMemory = CurrentView.GetMemory();
		return MakeValue(*(static_cast<T*>(CurrentViewMemory)));
	}
	
	template <typename T>
	operator T() const
	{
		TValueOrError<T, EPropertyBagResult> Result = GetValue<T>();
		if (Result.HasError())
		{
			UE_LOG(LogTemp, Error, TEXT("Type mismatch when reading property: %s"), *PropertyDescName.ToString());
			return T();
		}
		return Result.GetValue();
	}

	template <typename T>
	FSampleBindingData& operator=(const T& NewValue)
	{
		SetValue(NewValue);
		return *this;
	}

	template<typename T>
	TValueOrError<T, EPropertyBagResult> GetValue() const
	{
		if (!HasBinding())
		{
			return MakeError(EPropertyBagResult::TypeMismatch);
		}

		const FName PropertyName = PropertyDescName;
		switch (PropertyDescValueType)
		{
			case EPropertyBagPropertyType::Bool:
			{
				if constexpr (std::is_same_v<T, bool>)
				{
					return PropertyBag->GetValueBool(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Byte:
			{
				if constexpr (std::is_same_v<T, uint8>)
				{
					return PropertyBag->GetValueByte(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Int32:
			{
				if constexpr (std::is_same_v<T, int32>)
				{
					return PropertyBag->GetValueInt32(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Int64:
			{
				if constexpr (std::is_same_v<T, int64>)
				{
					return PropertyBag->GetValueInt64(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Float:
			{
				if constexpr (std::is_same_v<T, float>)
				{
					return PropertyBag->GetValueFloat(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Double:
			{
				if constexpr (std::is_same_v<T, double>)
				{
					return PropertyBag->GetValueDouble(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Name:
			{
				if constexpr (std::is_same_v<T, FName>)
				{
					return PropertyBag->GetValueName(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::String:
			{
				if constexpr (std::is_same_v<T, FString>)
				{
					return PropertyBag->GetValueString(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Text:
			{
				if constexpr (std::is_same_v<T, FText>)
				{
					return PropertyBag->GetValueText(PropertyName);
				}
				break;
			}
			case EPropertyBagPropertyType::Struct:
			{
				return GetNestedPropertyValue<T>();
				break;
			}
			default:
			{
				break;
			}
		}

		return MakeError(EPropertyBagResult::TypeMismatch);
	}

	/* Not ready */
	template<typename T>
	bool SetValue(const T& NewValue)
	{
		if (!HasBinding())
		{
			return false;
		}

		const FName PropertyName = PropertyDescName;
		switch (PropertyDescValueType)
		{
			case EPropertyBagPropertyType::Bool:
			{
				if constexpr (std::is_same_v<T, bool>)
				{
					return PropertyBag->SetValueBool(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Byte:
			{
				if constexpr (std::is_same_v<T, uint8>)
				{
					return PropertyBag->SetValueByte(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Int32:
			{
				if constexpr (std::is_same_v<T, int32>)
				{
					return PropertyBag->SetValueInt32(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Int64:
			{
				if constexpr (std::is_same_v<T, int64>)
				{
					return PropertyBag->SetValueInt64(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Float:
			{
				if constexpr (std::is_same_v<T, float>)
				{
					return PropertyBag->SetValueFloat(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Double:
			{
				if constexpr (std::is_same_v<T, double>)
				{
					return PropertyBag->SetValueDouble(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Name:
			{
				if constexpr (std::is_same_v<T, FName>)
				{
					return PropertyBag->SetValueName(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::String:
			{
				if constexpr (std::is_same_v<T, FString>)
				{
					return PropertyBag->SetValueString(PropertyName, NewValue);
				}
				break;
			}
			case EPropertyBagPropertyType::Text:
			{
				if constexpr (std::is_same_v<T, FText>)
				{
					return PropertyBag->SetValueText(PropertyName, NewValue);
				}
				break;
			}
			default:
			{
				break;
			}
		}

		UE_LOG(LogTemp, Error, TEXT("Type mismatch when writing property: %s"), *PropertyDescName.ToString());
		return false;
	}
};