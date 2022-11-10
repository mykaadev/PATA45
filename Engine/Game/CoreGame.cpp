
#include "Core.h"
#include <iostream>


#undef main

int main()
{
	Core::GetInstance()->InitEngine();

	return 0;
}