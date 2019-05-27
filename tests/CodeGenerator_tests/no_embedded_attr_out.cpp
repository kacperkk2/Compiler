#include <iostream>
#include "Embedded_types.h"


void rec(User u)
{
	Algorithm a = Algorithm("Collaborative");
	Path p = a.getRecomendation(u);
	float rrank = a.rankRecomendation( ("<-- Zmienna nie ma takiego pola" !!!^ ERROR ^!!!)