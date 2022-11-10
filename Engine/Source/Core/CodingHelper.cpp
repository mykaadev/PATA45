#include "CodingHelper.h"


CodingHelper* CodingHelper::m_Instance = nullptr;


CodingHelper::CodingHelper()
{
	AmountOfPropertiesToClear = 0;
	IncrementClearCounter();
}

CodingHelper::~CodingHelper()
{
	delete m_Instance;
	DecrementClearCounter();
}

void CodingHelper::IncrementClearCounter()
{
	++AmountOfPropertiesToClear;
}

void CodingHelper::IncrementAmountToClearCounter(int inAmountToAdd)
{
	AmountOfPropertiesToClear += inAmountToAdd;
}

void CodingHelper::DecrementClearCounter()
{
	--AmountOfPropertiesToClear;
}

void CodingHelper::DecrementAmountToClearCounter(int inAmountToRemove)
{
	AmountOfPropertiesToClear -= inAmountToRemove;

}

void CodingHelper::SetClearCounter(int inAmountToSet)
{
	AmountOfPropertiesToClear = inAmountToSet;
}

bool CodingHelper::CanQuit()
{
	if (AmountOfPropertiesToClear != 0)
	{
		return true;
	}

	return false;
}

void CodingHelper::DisplayInfo(std::string inInfo)
{
	std::cout << "INFO: " << inInfo << std::endl;
}


void CodingHelper::DisplayPropertiesLeftToClear()
{
	std::cout << "ERROR: " << "PROPERTIES LEFT TO CLEAR: " << AmountOfPropertiesToClear << std::endl;
}
