#include "SampleBindingData.h"

bool FSampleBindingData::HasBinding() const
{
	if (PropertyBag == nullptr || PropertyBag->FindPropertyDescByName(PropertyDescName) == nullptr)
	{
		return false;
	}
	return true;
}