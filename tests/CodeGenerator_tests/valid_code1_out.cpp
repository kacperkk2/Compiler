#include <iostream>
#include "Embedded_types.h"


int ffunction(int value, int fvalue)
{
	return 3;
}

float function2(int fvalue)
{
	return 2.990000;
}

int main()
{
	User u = User(1);
	int value = 1;
	float fvalue = 2;
	Algorithm a = Algorithm("fast");
	a.lambda = 1.000000;
	a.pearsonGroupSize = ffunction(value, fvalue) + (function2(fvalue) + 4.800000);
	float param = 0.500000;
	while(param < 10.500000)
	{
		param = param + 0.500000;
		std::cout << "Parameter value: ";
		std::cout << param;
		std::cout << "\n";
	}
	Path p = a.getRecomendation(u);
	float arank = u.rankRecomendation(p);
	if(arank > a.regParameter)
	{
		int value = 3;
		std::cout << "Algorithm typed path correctly";
	}
	else
	{
		std::cout << "stringg string1";
	}

	return 1;
}

