#include <iostream>
#include <ctype.h>
#include <string>

#include "Structures.h"
#include "Nodes.h"
#include "Token.h"
#include "Source.h"
#include "Lexer.h"
#include "Parser.h"

using namespace std;

void interprete_token(Token&);
void print_tree(Base_node*);

int main()
{
    string filename = "plik.txt";

    Source src(filename);
    Lexer l(src);
    Parser parser(l);

    Program_node* root = parser.PROGRAM();

    cout<<endl<<"DRZEWKO: ----------"<<endl;

    root->to_string("");
    //print_tree(root);

    /*cout<<root->functions.size()<<endl;
    cout<<root->functions[0]->ret_type<<endl;
    cout<<root->functions[0]->id<<endl;

    cout<<root->functions[0]->parameters.size()<<endl;
    cout<<root->functions[0]->parameters[0].param_type<<endl;
    cout<<root->functions[0]->parameters[0].param_id<<endl;

    cout<<root->functions[0]->statements.size()<<endl;

    Print_node* p_node = (Print_node*)root->functions[0]->statements[0];
    cout<<p_node->content<<endl;
    cout<<p_node->is_id_in_content<<endl;*/

    return 0;
}

void interprete_token(Token& A)
{
    /*Token A;

    while(A.get_token() != END_SOURCE_SY)
    {
        A = l.get_token();
        interprete_token(A);

        int x;
        cin>>x;
    }*/

    switch(A.get_token())
    {
    case NUMBER_INT_SY:
        cout << "NUMBER_INT_SY" << "|" << A.get_int_value() << "|" << endl;
        break;
    case NUMBER_FLOAT_SY:
        cout << "NUMBER_FLOAT_SY" << "|" << A.get_float_value() << "|" << endl;
        break;
    case ID_SY:
        cout << "ID_SY" << "|" << A.get_str_value() << "|" << endl;
        break;
    case RETURN_SY:
        cout << "RETURN_SY" << endl;
        break;
    case LBRACE_SY:
        cout << "LBRACE_SY" << endl;
        break;
    case RBRACE_SY:
        cout << "RBRACE_SY" << endl;
        break;
    case LPAREN_SY:
        cout << "LPAREN_SY" << endl;
        break;
    case RPAREN_SY:
        cout << "RPAREN_SY" << endl;
        break;
    case SEMICOL_SY:
        cout << "SEMICOL_SY" << endl;
        break;
    case IF_SY:
        cout << "IF_SY" << endl;
        break;
    case ELSEIF_SY:
        cout << "ELSEIF_SY" << endl;
        break;
    case WHILE_SY:
        cout << "WHILE_SY" << endl;
        break;
    case ELSE_SY:
        cout << "ELSE_SY" << endl;
        break;
    case TRUE_SY:
        cout << "TRUE_SY" << endl;
        break;
    case FALSE_SY:
        cout << "FALSE_SY" << endl;
        break;
    case DOT_SY:
        cout << "DOT_SY" << endl;
        break;
    case COMMA_SY:
        cout << "COMMA_SY" << endl;
        break;
    case LESS_SY:
        cout << "LESS_SY" << endl;
        break;
    case LESS_EQUAL_SY:
        cout << "LESS_EQUAL_SY" << endl;
        break;
    case GREATER_SY:
        cout << "GREATER_SY" << endl;
        break;
    case GREATER_EQUAL_SY:
        cout << "GREATER_EQUAL_SY" << endl;
        break;
    case AND_SY:
        cout << "AND_SY" << endl;
        break;
    case NOT_EQUAL_SY:
        cout << "NOT_EQUAL_SY" << endl;
        break;
    case EQUAL_SY:
        cout << "EQUAL_SY" << endl;
        break;
    case MULT_SY:
        cout << "MULT_SY" << endl;
        break;
    case DIV_SY:
        cout << "DIV_SY" << endl;
        break;
    case NOT_SY:
        cout << "NOT_SY" << endl;
        break;
    case ASSIGN_SY:
        cout << "ASSIGN_SY" << endl;
        break;
    case PLUS_SY:
        cout << "PLUS_SY" << endl;
        break;
    case MINUS_SY:
        cout << "MINUS_SY" << endl;
        break;
    case INT_SY:
        cout << "INT_SY" << endl;
        break;
    case FLOAT_SY:
        cout << "FLOAT_SY" << endl;
        break;
    case STRING_SY:
        cout << "STRING_SY" << endl;
        break;
    case BOOL_SY:
        cout << "BOOL_SY" << endl;
        break;
    case USER_SY:
        cout << "USER_SY" << endl;
        break;
    case ALGORITHM_SY:
        cout << "ALGORITHM_SY" << endl;
        break;
    case PATH_SY:
        cout << "PATH_SY" << endl;
        break;
    case END_SOURCE_SY:
        cout << "END_SOURCE_SY" << endl;
        break;
    case PRINT_SY:
        cout << "PRINT_SY" << endl;
        break;
    case INPUT_SY:
        cout << "INPUT_SY" << endl;
        break;
    case STRING_CONTENT_SY:
        cout << "STRING_CONTENT_SY" << "|" << A.get_str_value() << "|" << endl;
        break;
    case NORET_SY:
        cout << "NORET_SY" << endl;
        break;
    default:
        cout << "ERROR_SY" << endl;
        cout<< A.get_token_pos().current_line <<endl;
        string error_point;
        for(int i=0; i < A.get_token_pos().pos_in_line; i++)
        {
            error_point += " ";
        }
        error_point += "^";
        cout<<error_point<<endl;
        cout<<"Numer linii: "<< A.get_token_pos().line_number<<endl;
        cout<<"Pozycja w linii: "<< A.get_token_pos().pos_in_line <<endl;

        break;
    }
}

