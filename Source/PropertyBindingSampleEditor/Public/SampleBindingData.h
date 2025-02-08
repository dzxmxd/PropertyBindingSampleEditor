#pragma once
#include "StructUtils/PropertyBag.h"
#include "SampleBindingData.generated.h"

USTRUCT()
struct FSampleBindingData
{
	GENERATED_BODY()

public:
	const FInstancedPropertyBag* PropertyBag = nullptr;
	FName PropertyDescName;
	EPropertyBagPropertyType PropertyDescValueType = EPropertyBagPropertyType::None;

	FLinearColor Color = FLinearColor::White;
	const FSlateBrush* Image = nullptr;

	bool HasBinding() const;
	
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
				if constexpr (std::is_same_v<T, bool>)
					return PropertyBag->GetValueBool(PropertyName);
			break;
			case EPropertyBagPropertyType::Byte:
				if constexpr (std::is_same_v<T, uint8>)
					return PropertyBag->GetValueByte(PropertyName);
			break;
			case EPropertyBagPropertyType::Int32:
				if constexpr (std::is_same_v<T, int32>)
					return PropertyBag->GetValueInt32(PropertyName);
			break;
			case EPropertyBagPropertyType::Int64:
				if constexpr (std::is_same_v<T, int64>)
					return PropertyBag->GetValueInt64(PropertyName);
			break;
			case EPropertyBagPropertyType::Float:
				if constexpr (std::is_same_v<T, float>)
					return PropertyBag->GetValueFloat(PropertyName);
			break;
			case EPropertyBagPropertyType::Double:
				if constexpr (std::is_same_v<T, double>)
					return PropertyBag->GetValueDouble(PropertyName);
			break;
			case EPropertyBagPropertyType::Name:
				if constexpr (std::is_same_v<T, FName>)
					return PropertyBag->GetValueName(PropertyName);
			break;
			case EPropertyBagPropertyType::String:
				if constexpr (std::is_same_v<T, FString>)
					return PropertyBag->GetValueString(PropertyName);
			break;
			case EPropertyBagPropertyType::Text:
				if constexpr (std::is_same_v<T, FText>)
					return PropertyBag->GetValueText(PropertyName);
			break;
			default:
				break;
		}

		return MakeError(EPropertyBagResult::TypeMismatch);
	}

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
				if constexpr (std::is_same_v<T, bool>)
					return PropertyBag->SetValueBool(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Byte:
				if constexpr (std::is_same_v<T, uint8>)
					return PropertyBag->SetValueByte(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Int32:
				if constexpr (std::is_same_v<T, int32>)
					return PropertyBag->SetValueInt32(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Int64:
				if constexpr (std::is_same_v<T, int64>)
					return PropertyBag->SetValueInt64(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Float:
				if constexpr (std::is_same_v<T, float>)
					return PropertyBag->SetValueFloat(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Double:
				if constexpr (std::is_same_v<T, double>)
					return PropertyBag->SetValueDouble(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Name:
				if constexpr (std::is_same_v<T, FName>)
					return PropertyBag->SetValueName(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::String:
				if constexpr (std::is_same_v<T, FString>)
					return PropertyBag->SetValueString(PropertyName, NewValue);
			break;
			case EPropertyBagPropertyType::Text:
				if constexpr (std::is_same_v<T, FText>)
					return PropertyBag->SetValueText(PropertyName, NewValue);
			break;
			default:
				break;
		}

		UE_LOG(LogTemp, Error, TEXT("Type mismatch when writing property: %s"), *PropertyDescName.ToString());
		return false;
	}

public:
	/** 绑定的属性名称 */
	FName PropertyName;

	TWeakObjectPtr<UObject> WeakOwnerObject = nullptr;
	// FStateTreePropertyPath CachedSourcePath;
	// FStateTreePropertyPath TargetPath;
	TSharedPtr<const IPropertyHandle> PropertyHandle;
	// TArray<FStateTreeBindableStructDesc> AccessibleStructs;

	/* Default name of the source struct. */
	FText SourceStructName;

	/* Binding's display name text. Expects it's source struct name to be injected before use. */
	FText FormatableText;

	/* Binding's tooltip text. Expects it's source struct name to be injected before use. */
	FText FormatableTooltipText;
	
	bool bIsDataCached = false;

};