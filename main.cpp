#include <iostream>
#include <ctype.h>
#include <string>

#include "Structures.h"
#include "Nodes.h"
#include "Token.h"
#include "Source.h"
#include "Lexer.h"
#include "Parser.h"
#include "CodeGenerator.h"

using namespace std;

void interprete_token(Token&);

int main()
{
    string filename = "code.txt";
    string output_file = "output/out.cpp";

    Source src(filename);
    Lexer l(src);
    Parser p(l);
    CodeGenerator cg(output_file, p);

    cg.evaluate_tree();

    return 0;
}
