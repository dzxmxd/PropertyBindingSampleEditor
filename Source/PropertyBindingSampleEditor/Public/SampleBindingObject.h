// Copyright https://github.com/dzxmxd/PropertyBindingSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/PropertyBag.h"

#include "SampleBindingObject.generated.h"

UCLASS()
class PROPERTYBINDINGSAMPLEEDITOR_API USampleBindingObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = TestParameters)
	FInstancedPropertyBag Parameters;

	UPROPERTY(EditAnywhere, Category = TestBinding)
	int32 TestInt32Value;

	UPROPERTY(EditAnywhere, Category = TestBinding)
	bool TestBoolValue;
};