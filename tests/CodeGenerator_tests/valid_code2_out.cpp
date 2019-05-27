#include <iostream>
#include "Embedded_types.h"


void rec(User u)
{
	Algorithm a = Algorithm("Collaborative");
	Path p = a.getRecomendation(u);
	float rrank = u.rankRecomendation(p);
	if(rrank > 5.500000)
	{
		std::cout << "High rank of recommendation\n";
	}
	else
	{
		std::cout << "Low rank\n";
	}
}

int main()
{
	User u = User(1);
	rec(u)	Algorithm a = Algorithm("fast");
	a.lambda = 0.500000;
	while(a.lambda < 10.500000)
	{
		a.lambda = a.lambda + 0.500000;
		std::cout << "Parameter value: ";
		std::cout << a.lambda;
		std::cout << "\n";
	}

	return 1;
}

