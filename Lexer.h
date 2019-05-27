#ifndef LEXER_H
#define LEXER_H

using namespace std;

class Lexer
{
    Source& src;

    // skladane id, string, numer
    string spell;

    Token_pos token_pos;

public:
    Lexer(Source &s) : src(s)
    {
        // od razu wkladam znak do bufora source
        src.load_next_ch();
    }

    TOKEN_SY check_if_keyword(string s)
    {
        if(s == "return") return RETURN_SY;
        else if(s == "if") return IF_SY;
        else if(s == "elseif") return ELSEIF_SY;
        else if(s == "else") return ELSE_SY;
        else if(s == "while") return WHILE_SY;
        else if(s == "true") return TRUE_SY;
        else if(s == "false") return FALSE_SY;
        else if(s == "int") return INT_SY;
        else if(s == "float") return FLOAT_SY;
        else if(s == "string") return STRING_SY;
        else if(s == "bool") return BOOL_SY;
        else if(s == "User") return USER_SY;
        else if(s == "Algorithm") return ALGORITHM_SY;
        else if(s == "Path") return PATH_SY;
        else if(s == "print") return PRINT_SY;
        else if(s == "input") return INPUT_SY;
        else if(s == "noret") return NORET_SY;
        else if(s == "pearsonGroupSize") return pearsonGroupSize_SY;
        else if(s == "regParameter") return regParameter_SY;
        else if(s == "lambda") return lambda_SY;
        else if(s == "rankRecomendation") return rankRecomendation_SY;
        else if(s == "getRecomendation") return getRecomendation_SY;
        else return ID_SY;
    }

    Token get_token()
    {
        do
        {
            // omijam kazdy znak bialy
            while(isspace(src.see_current_ch()))
                src.load_next_ch();

            //sprawdzam czy nie jestem na koncu pliku
            if(src.see_current_ch() == EOF)
                return Token(END_SOURCE_SY, token_pos); // daje tu poprzednia pozycje bo nie mam jeszcze biezacej, pozniej sie ona robi

            // omijam komentarz
            if(src.see_current_ch() == '@')
            {
                do
                {
                    src.load_next_ch();
                }
                while(src.see_current_ch() != '@' && src.see_current_ch() != EOF); // na wszelki wypadek nie domkniecia komentarza
                src.load_next_ch();
            }
        }
        while(isspace(src.see_current_ch()));

        // rejestruje pozycje zaczecia sie biezacego tokenu
        set_current_token_position();


        if(isalpha(src.see_current_ch()))
        {
            make_id_or_keyword();
            TOKEN_SY token = check_if_keyword(spell);
            return Token(token, spell, token_pos);
        }
        else if(isdigit(src.see_current_ch()))
        {
            make_number_int();

            if(src.see_current_ch() == '.')
            {
                Token t = make_number_float();
                return t;
            }

            int number = atoi(spell.c_str());
            return Token(NUMBER_INT_SY, number, token_pos);
        }

        switch(src.see_current_ch())
        {
        case EOF:
            return Token(END_SOURCE_SY, token_pos);
        case '{':
            src.load_next_ch();
            return Token(LBRACE_SY, token_pos);
        case '}':
            src.load_next_ch();
            return Token(RBRACE_SY, token_pos);
        case '(':
            src.load_next_ch();
            return Token(LPAREN_SY, token_pos);
        case ')':
            src.load_next_ch();
            return Token(RPAREN_SY, token_pos);
        case ';':
            src.load_next_ch();
            return Token(SEMICOL_SY, token_pos);
        case ',':
            src.load_next_ch();
            return Token(COMMA_SY, token_pos);
        case '.':
            src.load_next_ch();
            return Token(DOT_SY, token_pos);
        case '~':
            src.load_next_ch();
            return Token(NOT_SY, token_pos);
        case '!':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '=':
                src.load_next_ch();
                return Token(NOT_EQUAL_SY, token_pos);
            default:
                return Token_Error(ERROR_SY, token_pos);
            }
        case '=':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '=':
                src.load_next_ch();
                return Token(EQUAL_SY, token_pos);
            default:
                return Token(ASSIGN_SY, token_pos);
            }
        case '+':
            src.load_next_ch();
            return Token(PLUS_SY, token_pos);
        case '-':
            src.load_next_ch();
            return Token(MINUS_SY, token_pos);
        case '<':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '=':
                src.load_next_ch();
                return Token(LESS_EQUAL_SY, token_pos);
            default:
                return Token(LESS_SY, token_pos);
            }
        case '>':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '=':
                src.load_next_ch();
                return Token(GREATER_EQUAL_SY, token_pos);
            default:
                return Token(GREATER_SY, token_pos);
            }
        case '|':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '|':
                src.load_next_ch();
                return Token(OR_SY, token_pos);
            default:
                return Token_Error(ERROR_SY, token_pos);
            }
        case '&':
            src.load_next_ch();

            switch(src.see_current_ch())
            {
            case '&':
                src.load_next_ch();
                return Token(AND_SY, token_pos);
            default:
                return Token_Error(ERROR_SY, token_pos);
            }
        case '*':
            src.load_next_ch();
            return Token(MULT_SY, token_pos);
        case '/':
            src.load_next_ch();
            return Token(DIV_SY, token_pos);
        case '"':
        {
            src.load_next_ch();

            Token t = make_string_content();
            return t;
        }
        default:
            src.load_next_ch();
            return Token_Error(ERROR_SY, token_pos);
        }
    }

    void set_current_token_position()
    {
        token_pos.pos_in_line = src.get_pos_in_line();
        token_pos.current_line = src.get_current_line();
        token_pos.line_number = src.get_line_number();
    }

    Token make_number_float()
    {
        spell += src.see_current_ch();
        src.load_next_ch();

        if(!isdigit(src.see_current_ch()))
            return Token_Error(ERROR_SY, token_pos);

        while(isdigit(src.see_current_ch()))
        {
            spell += src.see_current_ch();
            src.load_next_ch();
        }
        float number = atof(spell.c_str());
        return Token(NUMBER_FLOAT_SY, number, token_pos);
    }

    void make_number_int()
    {
        spell = src.see_current_ch();
        src.load_next_ch();

        while(isdigit(src.see_current_ch()))
        {
            spell += src.see_current_ch();
            src.load_next_ch();
        }
    }

    void make_id_or_keyword()
    {
        spell = src.see_current_ch();
        src.load_next_ch();

        while(isalnum(src.see_current_ch()))
        {
            spell += src.see_current_ch();
            src.load_next_ch();
        }
    }

    Token make_string_content()
    {
        if(src.see_current_ch()=='"')
        {
            src.load_next_ch();
            return Token(STRING_CONTENT_SY, "", token_pos); // pusty string
        }

        spell = src.see_current_ch();
        src.load_next_ch();

        while(src.see_current_ch()!='"')
        {
            spell += src.see_current_ch();
            src.load_next_ch();
        }
        src.load_next_ch();

        return Token(STRING_CONTENT_SY, spell, token_pos);
    }

    void error(Token t)
    {
        cout<<"Blad w linii:"<<endl;
        cout<< t.get_token_pos().current_line <<endl;
        string error_point;
        for(int i=0; i < t.get_token_pos().pos_in_line; i++)
        {
            error_point += " ";
        }
        error_point += "^";
        cout<<error_point<<endl;
        cout<<"Numer linii: "<< t.get_token_pos().line_number<<endl;
        cout<<"Pozycja w linii: "<< t.get_token_pos().pos_in_line <<endl;
    }
};


#endif
