#pragma once

#include <iostream>
#include <string>

class CodingHelper
{
public:
	static CodingHelper* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new CodingHelper(); }

	void IncrementClearCounter();
	void IncrementAmountToClearCounter(int inAmountToAdd);
	void DecrementClearCounter();
	void DecrementAmountToClearCounter(int inAmountToRemove);

	void SetClearCounter(int inAmountToSet);
	bool CanQuit();

	void DisplayInfo(std::string inInfo);
	void DisplayPropertiesLeftToClear();

private:

	CodingHelper();

	static CodingHelper* m_Instance;

	int AmountOfPropertiesToClear;

	~CodingHelper();

};
