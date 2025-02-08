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
	{
		Parameters.AddProperty(TEXT("TestFloat"), EPropertyBagPropertyType::Float);
		Parameters.AddProperty(TEXT("TestInt32"), EPropertyBagPropertyType::Int32);
		Parameters.AddProperty(TEXT("TestBool"), EPropertyBagPropertyType::Bool);
	}

public:
	UPROPERTY(EditAnywhere, Category = TestParameters)
	FInstancedPropertyBag Parameters;

private:
	UPROPERTY(EditAnywhere)
	float TestFloatValue;
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
};