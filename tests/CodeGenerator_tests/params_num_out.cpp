#include <iostream>
#include "Embedded_types.h"


void fun(string f, int s, float t)
{
	std::cout << f;
}

int main()
{
	string f = "sss";
	int s = 2;
	fun( ("<-- Istnieje funkcja o takiej nazwie ale innej liczbie parametrow wywolania" !!!^ ERROR ^!!!)