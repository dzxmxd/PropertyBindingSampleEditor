// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "SampleBindingData.h"
#include "StructUtils/PropertyBag.h"
#include "SampleBindingObject.generated.h"

UCLASS()
class PROPERTYBINDINGSAMPLEEDITOR_API USampleBindingObject : public UObject
{
	GENERATED_BODY()

	USampleBindingObject()
		: TestFloatValue(0.114)
		, TestInt32Value(514)
		, TestBoolValue(true)
		, TestVectorValue(FVector(11.22, 22.33, 33.44))
	{
		Parameters.AddProperty(TEXT("TestFloat"), EPropertyBagPropertyType::Double);
		Parameters.SetValueFloat(TEXT("TestFloat"), 123.123);

		Parameters.AddProperty(TEXT("TestInt32"), EPropertyBagPropertyType::Int32);
		Parameters.SetValueInt32(TEXT("TestInt32"), 123123);

		Parameters.AddProperty(TEXT("TestBool"), EPropertyBagPropertyType::Bool);
		Parameters.SetValueBool(TEXT("TestBool"), true);

		Parameters.AddProperty(TEXT("TestVector"), EPropertyBagPropertyType::Struct, TBaseStructure<FVector>::Get());
		Parameters.SetValueStruct(TEXT("TestVector"), FVector(1.11, 22.2, 3.33));
	}

public:
	UPROPERTY(EditAnywhere, Category = TestParameters)
	FInstancedPropertyBag Parameters;

private:
	UPROPERTY(EditAnywhere)
	double TestFloatValue;
	UPROPERTY()
	FSampleBindingData TestFloatValueBindingData;

	UPROPERTY(EditAnywhere)
	int32 TestInt32Value;
	UPROPERTY()
	FSampleBindingData TestInt32ValueBindingData;

	UPROPERTY(EditAnywhere)
	bool TestBoolValue;
	UPROPERTY()
	FSampleBindingData TestBoolValueBindingData;

	UPROPERTY(EditAnywhere)
	FVector TestVectorValue;
	UPROPERTY()
	FSampleBindingData TestVectorValueBindingData;
};