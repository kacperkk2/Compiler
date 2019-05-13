#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <sstream>

enum TOKEN_SY
{
    ID_SY, RETURN_SY,
    // wartosci
    NUMBER_INT_SY, NUMBER_FLOAT_SY, STRING_CONTENT_SY,
    // nawiasy, srednik
    LBRACE_SY, RBRACE_SY, LPAREN_SY, RPAREN_SY, SEMICOL_SY,
    // operacje
    IF_SY, ELSEIF_SY, ELSE_SY, WHILE_SY,
    // true false
    TRUE_SY, FALSE_SY,
    //operatory
    DOT_SY, COMMA_SY,
    LESS_SY, LESS_EQUAL_SY, GREATER_SY, GREATER_EQUAL_SY, OR_SY, AND_SY,
    NOT_EQUAL_SY, EQUAL_SY, MULT_SY, DIV_SY, NOT_SY, ASSIGN_SY,
    PLUS_SY, MINUS_SY,
    // typy
    INT_SY, FLOAT_SY, STRING_SY, BOOL_SY, NORET_SY,
    USER_SY, ALGORITHM_SY, PATH_SY,
    // dozwolone pola algorithm
    pearsonGroupSize_SY, regParameter_SY, lambda_SY,
    // dozwolone metody wbudowane
    rankRecomendation_SY, getRecomendation_SY,
    // blad lub koniec
    ERROR_SY, END_SOURCE_SY,
    // in out
    PRINT_SY, INPUT_SY
};

struct Token_pos
{
    std::string current_line;
    int pos_in_line;
    int line_number;
};

template<typename T>
std::string to_string(const T& obj)
{
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

#endif
