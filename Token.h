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

string token_name(TOKEN_SY tok_sy)
{
    switch(tok_sy)
    {
    case NUMBER_INT_SY:
        return "NUMBER_INT_SY";
    case NUMBER_FLOAT_SY:
        return "NUMBER_FLOAT_SY";
    case ID_SY:
        return "ID_SY" ;
    case RETURN_SY:
        return "RETURN_SY" ;
    case LBRACE_SY:
        return "LBRACE_SY" ;
    case RBRACE_SY:
        return "RBRACE_SY" ;
    case LPAREN_SY:
        return "LPAREN_SY" ;
    case RPAREN_SY:
        return "RPAREN_SY" ;
    case SEMICOL_SY:
        return "SEMICOL_SY" ;
    case IF_SY:
        return "IF_SY" ;
    case ELSEIF_SY:
        return "ELSEIF_SY" ;
    case WHILE_SY:
        return "WHILE_SY" ;
    case ELSE_SY:
        return "ELSE_SY" ;
    case TRUE_SY:
        return "TRUE_SY" ;
    case FALSE_SY:
        return "FALSE_SY" ;
    case DOT_SY:
        return "DOT_SY" ;
    case COMMA_SY:
        return "COMMA_SY" ;
    case LESS_SY:
        return "LESS_SY" ;
    case LESS_EQUAL_SY:
        return "LESS_EQUAL_SY" ;
    case GREATER_SY:
        return "GREATER_SY" ;
    case GREATER_EQUAL_SY:
        return "GREATER_EQUAL_SY" ;
    case AND_SY:
        return "AND_SY" ;
    case OR_SY:
        return "OR_SY" ;
    case NOT_EQUAL_SY:
        return "NOT_EQUAL_SY" ;
    case EQUAL_SY:
        return "EQUAL_SY" ;
    case MULT_SY:
        return "MULT_SY" ;
    case DIV_SY:
        return "DIV_SY" ;
    case NOT_SY:
        return "NOT_SY" ;
    case ASSIGN_SY:
        return "ASSIGN_SY" ;
    case PLUS_SY:
        return "PLUS_SY" ;
    case MINUS_SY:
        return "MINUS_SY" ;
    case INT_SY:
        return "INT_SY" ;
    case FLOAT_SY:
        return "FLOAT_SY" ;
    case STRING_SY:
        return "STRING_SY" ;
    case BOOL_SY:
        return "BOOL_SY" ;
    case USER_SY:
        return "USER_SY" ;
    case ALGORITHM_SY:
        return "ALGORITHM_SY" ;
    case PATH_SY:
        return "PATH_SY" ;
    case END_SOURCE_SY:
        return "END_SOURCE_SY" ;
    case PRINT_SY:
        return "PRINT_SY" ;
    case INPUT_SY:
        return "INPUT_SY" ;
    case STRING_CONTENT_SY:
        return "STRING_CONTENT_SY";
    case NORET_SY:
        return "NORET_SY" ;
    case pearsonGroupSize_SY:
        return "pearsonGroupSize_SY" ;
    case regParameter_SY:
        return "regParameter_SY" ;
    case lambda_SY:
        return "lambda_SY" ;
    case rankRecomendation_SY:
        return "rankRecomendation_SY" ;
    case getRecomendation_SY:
        return "getRecomendation_SY" ;
    default:
        return "ERROR_SY" ;
    }
}

#endif
