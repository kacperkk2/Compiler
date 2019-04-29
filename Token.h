#ifndef TOKEN_H
#define TOKEN_H

using namespace std;

class Token
{
protected:
    TOKEN_SY token;

    std::string str_value;
    int int_value;
    float float_value;

    // token position (error)
    Token_pos token_pos;

public:
    Token(TOKEN_SY token)
    {
        this->token = token;
    }

    Token(TOKEN_SY token, Token_pos token_pos)
    {
        this->token = token;
        this->token_pos = token_pos;
    }

    Token(TOKEN_SY token, int int_value, Token_pos token_pos)
    {
        this->token = token;
        this->int_value = int_value;
        this->token_pos = token_pos;
    }

    Token(TOKEN_SY token, std::string str_value, Token_pos token_pos)
    {
        this->token = token;
        this->str_value = str_value;
        this->token_pos = token_pos;
    }

    Token(TOKEN_SY token, float float_value, Token_pos token_pos)
    {
        this->token = token;
        this->float_value = float_value;
        this->token_pos = token_pos;
    }

    Token(){}

    TOKEN_SY get_token()
    {
        return token;
    }

    std::string get_str_value()
    {
        return str_value;
    }

    int get_int_value()
    {
        return int_value;
    }

    float get_float_value()
    {
        return float_value;
    }

    Token_pos get_token_pos()
    {
        return token_pos;
    }
};

class Token_Error : public Token
{
public:
    Token_Error(TOKEN_SY token, Token_pos token_pos)
    {
        this->token = token;
        this->token_pos = token_pos;
    }
};

void token_name(TOKEN_SY tok_sy)
{
    switch(tok_sy)
    {
    case NUMBER_INT_SY:
        cout << "NUMBER_INT_SY";
        break;
    case NUMBER_FLOAT_SY:
        cout << "NUMBER_FLOAT_SY";
        break;
    case ID_SY:
        cout << "ID_SY" ;
        break;
    case RETURN_SY:
        cout << "RETURN_SY" ;
        break;
    case LBRACE_SY:
        cout << "LBRACE_SY" ;
        break;
    case RBRACE_SY:
        cout << "RBRACE_SY" ;
        break;
    case LPAREN_SY:
        cout << "LPAREN_SY" ;
        break;
    case RPAREN_SY:
        cout << "RPAREN_SY" ;
        break;
    case SEMICOL_SY:
        cout << "SEMICOL_SY" ;
        break;
    case IF_SY:
        cout << "IF_SY" ;
        break;
    case ELSEIF_SY:
        cout << "ELSEIF_SY" ;
        break;
    case WHILE_SY:
        cout << "WHILE_SY" ;
        break;
    case ELSE_SY:
        cout << "ELSE_SY" ;
        break;
    case TRUE_SY:
        cout << "TRUE_SY" ;
        break;
    case FALSE_SY:
        cout << "FALSE_SY" ;
        break;
    case DOT_SY:
        cout << "DOT_SY" ;
        break;
    case COMMA_SY:
        cout << "COMMA_SY" ;
        break;
    case LESS_SY:
        cout << "LESS_SY" ;
        break;
    case LESS_EQUAL_SY:
        cout << "LESS_EQUAL_SY" ;
        break;
    case GREATER_SY:
        cout << "GREATER_SY" ;
        break;
    case GREATER_EQUAL_SY:
        cout << "GREATER_EQUAL_SY" ;
        break;
    case AND_SY:
        cout << "AND_SY" ;
        break;
    case OR_SY:
        cout << "OR_SY" ;
        break;
    case NOT_EQUAL_SY:
        cout << "NOT_EQUAL_SY" ;
        break;
    case EQUAL_SY:
        cout << "EQUAL_SY" ;
        break;
    case MULT_SY:
        cout << "MULT_SY" ;
        break;
    case DIV_SY:
        cout << "DIV_SY" ;
        break;
    case NOT_SY:
        cout << "NOT_SY" ;
        break;
    case ASSIGN_SY:
        cout << "ASSIGN_SY" ;
        break;
    case PLUS_SY:
        cout << "PLUS_SY" ;
        break;
    case MINUS_SY:
        cout << "MINUS_SY" ;
        break;
    case INT_SY:
        cout << "INT_SY" ;
        break;
    case FLOAT_SY:
        cout << "FLOAT_SY" ;
        break;
    case STRING_SY:
        cout << "STRING_SY" ;
        break;
    case BOOL_SY:
        cout << "BOOL_SY" ;
        break;
    case USER_SY:
        cout << "USER_SY" ;
        break;
    case ALGORITHM_SY:
        cout << "ALGORITHM_SY" ;
        break;
    case PATH_SY:
        cout << "PATH_SY" ;
        break;
    case END_SOURCE_SY:
        cout << "END_SOURCE_SY" ;
        break;
    case PRINT_SY:
        cout << "PRINT_SY" ;
        break;
    case INPUT_SY:
        cout << "INPUT_SY" ;
        break;
    case STRING_CONTENT_SY:
        cout << "STRING_CONTENT_SY";
        break;
    case NORET_SY:
        cout << "NORET_SY" ;
        break;
    case pearsonGroupSize_SY:
        cout << "pearsonGroupSize_SY" ;
        break;
    case regParameter_SY:
        cout << "regParameter_SY" ;
        break;
    case lambda_SY:
        cout << "lambda_SY" ;
        break;
    case rankRecomendation_SY:
        cout << "rankRecomendation_SY" ;
        break;
    case getRecomendation_SY:
        cout << "getRecomendation_SY" ;
        break;
    default:
        cout << "ERROR_SY" ;
        break;
    }
}

#endif
