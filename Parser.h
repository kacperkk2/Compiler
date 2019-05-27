#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <algorithm>

#define NEED_SEMI_COL 1

class Parser
{
    Lexer& lex;
    Token token; // ostatnio pobrany token z lexera
    Token second_token; // zbuforowany kolejny token
    bool is_second_token; // mowi czy jest zbufurowany kolejny token

    std::list<TOKEN_SY> type_set = {INT_SY, FLOAT_SY, STRING_SY, BOOL_SY, NORET_SY, USER_SY,
                                    ALGORITHM_SY, PATH_SY};
    std::list<TOKEN_SY> stmt_first_set = {IF_SY, WHILE_SY, INT_SY, FLOAT_SY, STRING_SY, BOOL_SY, NORET_SY,
                                        USER_SY, ALGORITHM_SY, PATH_SY, RETURN_SY, ID_SY, PRINT_SY};
    std::list<TOKEN_SY> expr_first_set = {NOT_SY, LPAREN_SY, MINUS_SY, ID_SY,STRING_CONTENT_SY,
                                        NUMBER_FLOAT_SY, NUMBER_INT_SY, TRUE_SY, FALSE_SY};
    std::list<TOKEN_SY> value_set = {STRING_CONTENT_SY, NUMBER_FLOAT_SY, NUMBER_INT_SY, TRUE_SY, FALSE_SY};
    std::list<TOKEN_SY> embedded_set = {USER_SY, ALGORITHM_SY, PATH_SY};
    std::list<TOKEN_SY> algo_attr_set = {pearsonGroupSize_SY, regParameter_SY, lambda_SY};
    std::list<TOKEN_SY> embedded_fun_set = {rankRecomendation_SY, getRecomendation_SY};

    std::list<TOKEN_SY> expr_operator_set = {OR_SY, AND_SY, MULT_SY, DIV_SY, PLUS_SY, MINUS_SY,
                    NOT_EQUAL_SY, EQUAL_SY, LESS_EQUAL_SY, LESS_SY, GREATER_SY, GREATER_EQUAL_SY};

    std::list<TOKEN_SY> expr_operator_high_set = {AND_SY, MULT_SY, DIV_SY, NOT_EQUAL_SY, EQUAL_SY, LESS_EQUAL_SY, LESS_SY,
                                    GREATER_SY, GREATER_EQUAL_SY};
    std::list<TOKEN_SY> expr_operator_low_set = {OR_SY, PLUS_SY, MINUS_SY};

public:
    Parser(Lexer &l) : lex(l)
    {
        token = lex.get_token(); // pobranie pierwszego tokenu
        is_second_token = false;
    }

    void accept(TOKEN_SY tok_sy)
    {
        if(token.get_token() == tok_sy)
        {
            if(is_second_token)
            {
                token = second_token;
                is_second_token = false;
            }
            else
                token = lex.get_token(); // moze trzeba sprawdzic czy te token nie jest errorem ????????
        }
        else
            print_error_message("Oczekiwany token: " + token_name(tok_sy));
    }

    void print_error_message(string message)
    {
        lex.error(token); // wypisze komunikat gdzie jest blad
        cout<<"Napotkano na token: "<< token_name(token.get_token()) <<endl;
        cout<< message <<endl;
        exit(0);
    }

    void accept(TOKEN_SY tok_sy, Base_node* node)
    {
        if(token.get_token() == tok_sy)
        {
            interprete_and_fill_node(node);

            if(is_second_token)
            {
                token = second_token;
                is_second_token = false;
            }
            else
                token = lex.get_token(); // moze trzeba sprawdzic czy te token nie jest errorem ????????
        }
        else
            print_error_message("Oczekiwany token: " + token_name(tok_sy));
    }

    void accept(std::list<TOKEN_SY> set_sy)
    {
        if(token_in_set(set_sy))
            accept(token.get_token()); // token jest w secie
        else
        {
            string message = "Oczekiwany token: ";
            //wypisuje tokeny mozliwe ktore sa w secie
            for (auto item : set_sy)
                message = message + token_name(item) + " ";

            print_error_message(message);
        }
    }

    void accept(std::list<TOKEN_SY> set_sy, Base_node* node)
    {
        if(token_in_set(set_sy))
            accept(token.get_token(), node); // token jest w secie
        else
        {
            string message = "Oczekiwany token: ";
            //wypisuje tokeny mozliwe ktore sa w secie
            for (auto item : set_sy)
                message = message + token_name(item) + " ";

            print_error_message(message);
        }
    }

    void buffer_second_token()
    {
        second_token = lex.get_token();
        is_second_token = true;
    }

    // sprawdza czy biezacy token pobrany z lexera jest w danym secie
    bool token_in_set(std::list<TOKEN_SY> set_sy)
    {
        auto it = std::find(set_sy.begin(), set_sy.end(), token.get_token());
        if (it != set_sy.end())
            return true;
        return false;
    }

    void interprete_and_fill_node(Base_node* node)
    {
        switch(node->node_type)
        {
        case FUNCTION_NODE:
            {
                Function_node* f_node = (Function_node*) node;

                if(f_node-> got_ret_and_id) // zbieramy parametry
                {
                    if(token_in_set(type_set)) // mamy typ parametru
                        f_node-> param_type = token.get_str_value();
                    else // mamy id parametru
                        f_node-> parameters.push_back(Parameter(f_node-> param_type, token.get_str_value())); // typ id
                }
                else // zbieramy zwracany typ i id
                {
                    if(token_in_set(type_set)) // mamy return type
                        f_node-> ret_type = token.get_str_value();
                    else // mamy id funkcji
                    {
                        f_node-> id = token.get_str_value();
                        f_node-> got_ret_and_id = true; // teraz bedziemy chcieli parametry
                    }
                }
            break;
            }
        case PRINT_NODE:
            {
                Print_node* p_node = (Print_node*) node;
                if(token_in_set(value_set)) // mamy liczbe, napis, true, false
                {
                    switch(token.get_token())
                    {
                    case STRING_CONTENT_SY:
                        p_node-> content = "\""+token.get_str_value()+"\"";
                        break;
                    case NUMBER_INT_SY:
                        p_node-> content = to_string(token.get_int_value());
                        break;
                    case NUMBER_FLOAT_SY:
                        p_node-> content = to_string(token.get_float_value());
                        break;
                    case TRUE_SY:
                        p_node-> content = "true";
                        break;
                    case FALSE_SY:
                        p_node-> content = "false";
                        break;
                    default: break;
                    }
                }
                // mamy id przed ewentualna .
                else if(token.get_token() == ID_SY)
                {
                    p_node-> content = token.get_str_value();
                    p_node-> is_id_in_content = true;
                }
                // mamy atrybut po kropce
                else if(token_in_set(algo_attr_set))
                    p_node-> after_dot = token.get_str_value();
                // mamy kropke
                else
                    p_node-> is_dot = true;

            break;
            }
        case VALUE_NODE:
            {
                Value_node* v_node = (Value_node*) node;
                switch(token.get_token())
                    {
                    case MINUS_SY:
                        v_node-> minus_value = true;
                        break;
                    case STRING_CONTENT_SY:
                        v_node-> value = "\""+token.get_str_value()+"\"";
                        v_node->type_value = "string";
                        break;
                    case NUMBER_INT_SY:
                        v_node-> value = to_string(token.get_int_value());
                        v_node->type_value = "int";
                        break;
                    case NUMBER_FLOAT_SY:
                        v_node-> value = to_string(token.get_float_value());
                        v_node->type_value = "float";
                        break;
                    case TRUE_SY:
                        v_node-> value = "true";
                        v_node->type_value = "bool";
                        break;
                    case FALSE_SY:
                        v_node-> value = "false";
                        v_node->type_value = "bool";
                        break;
                    default: break;
                    }
            break;
            }
        case DEFINITION_NODE:
            {
                Definition_node* d_node = (Definition_node*) node;

                if(token_in_set(type_set))
                    d_node-> type = token.get_str_value();
                else
                {
                    switch(token.get_token())
                    {
                    case ASSIGN_SY:
                        d_node-> is_value = true;
                        break;
                    case ID_SY:
                        d_node-> id = token.get_str_value();
                        break;
                    default: break;
                    }
                }
                break;
            }
        case EMBEDDED_DECL_NODE:
            {
                Embedded_decl_node* e_node = (Embedded_decl_node*) node;

                if(token_in_set(embedded_set))
                {
                    switch(token.get_token())
                    {
                    case USER_SY:
                        e_node-> embedded_type = "User";
                        break;
                    case ALGORITHM_SY:
                        e_node-> embedded_type = "Algorithm";
                        break;
                    case PATH_SY:
                        e_node-> embedded_type = "Path";
                        break;
                    default: break;
                    }
                }
                else // wartosc w konstruktorze, w nawiasach
                {
                    switch(token.get_token())
                    {
                    case STRING_CONTENT_SY:
                        e_node-> value = "\""+token.get_str_value()+"\"";
                        break;
                    case NUMBER_INT_SY:
                        e_node-> value = to_string(token.get_int_value());
                        break;
                    default: break;
                    }
                }
                break;
            }
        case FUNCTION_CALL_NODE:
            {
                Function_call_node* f_node = (Function_call_node*) node;
                if(token.get_token() == ID_SY)
                    f_node-> id = token.get_str_value();
                break;
            }
        case ID_NODE:
            {
                Id_node* i_node = (Id_node*) node;
                if(token.get_token() == ID_SY)
                    i_node-> id = token.get_str_value();
                break;
            }
        case EMBEDDED_ATTR_NODE:
            {
                Embedded_attr_node* e_node = (Embedded_attr_node*) node;
                switch(token.get_token())
                {
                case pearsonGroupSize_SY:
                    e_node-> attribute = "pearsonGroupSize";
                    break;
                case regParameter_SY:
                    e_node-> attribute = "regParameter";
                    break;
                case lambda_SY:
                    e_node-> attribute = "lambda";
                    break;
                default: break;
                }
                break;
            }
        case EMBEDDED_FUN_NODE:
            {
                Embedded_fun_node* e_node = (Embedded_fun_node*) node;
                switch(token.get_token())
                {
                case rankRecomendation_SY:
                    e_node-> fun_name = "rankRecomendation";
                    break;
                case getRecomendation_SY:
                    e_node-> fun_name = "getRecomendation";
                    break;
                case ID_SY:
                    e_node-> value = token.get_str_value();
                    break;
                default: break;
                }
                break;
            }
        case ASSIGN_NODE:
            {
                Assign_node* a_node = (Assign_node*) node;
                switch(token.get_token())
                {
                case DOT_SY:
                    a_node-> is_algo_attr = true;
                    break;
                case pearsonGroupSize_SY:
                    a_node-> algo_attr = "pearsonGroupSize";
                    break;
                case regParameter_SY:
                    a_node-> algo_attr = "regParameter";
                    break;
                case lambda_SY:
                    a_node-> algo_attr = "lambda";
                    break;
                case ID_SY:
                    a_node-> id = token.get_str_value();
                    break;
                default: break;
                }
                break;
            }
        case EXPRESION_NODE:
            {
                Expresion_node* e_node = (Expresion_node*) node;
                switch(token.get_token())
                {
                case OR_SY:
                    e_node->operation = "||";
                    break;
                case AND_SY:
                    e_node->operation = "&&";
                    break;
                case MULT_SY:
                    e_node->operation = "*";
                    break;
                case DIV_SY:
                    e_node->operation = "/";
                    break;
                case PLUS_SY:
                    e_node->operation = "+";
                    break;
                case MINUS_SY:
                    e_node->operation = "-";
                    break;
                case NOT_EQUAL_SY:
                    e_node->operation = "!=";
                    break;
                case EQUAL_SY:
                    e_node->operation = "==";
                    break;
                case LESS_EQUAL_SY:
                    e_node->operation = "<=";
                    break;
                case LESS_SY:
                    e_node->operation = "<";
                    break;
                case GREATER_SY:
                    e_node->operation = ">";
                    break;
                case GREATER_EQUAL_SY:
                    e_node->operation = ">=";
                    break;
                default: break;
                }
            break;
            }

        default: break;
        }
    }

    // PROGRAM = {FUNCTION}
    Program_node* PROGRAM()
    {
        if(token.get_token() == END_SOURCE_SY)
            cout<<"Plik jest pusty"<<endl;

        Program_node* p = new Program_node();

        while(token_in_set(type_set))
            p->functions.push_back(FUNCTION());

        if(token.get_token() != END_SOURCE_SY)
            print_error_message("Plik moze skladac sie TYLKO z funkcji");

        return p;
    }

    // FUNCTION = RET_TYPE ID "(" {TYPE ID} ")" "{" {STATEMENT} "}"
    Function_node* FUNCTION()
    {
        Function_node* f_node = new Function_node();

        accept(type_set, f_node); // akceptuj kazdy token z setu, else error
        accept(ID_SY, f_node);
        accept(LPAREN_SY);

        if(token.get_token() != RPAREN_SY) // jesli nie ma ) to oczekuje parametrow, chroni przed wpisaniem glupot w ()
        {
            accept(type_set, f_node);
            accept(ID_SY, f_node);
            while(token.get_token() == COMMA_SY)
            {
                accept(COMMA_SY);
                accept(type_set, f_node); // akceptuj kazdy token z setu, else error
                accept(ID_SY, f_node);
            }
        }

        accept(RPAREN_SY);
        accept(LBRACE_SY);
        while(token_in_set(stmt_first_set))
            f_node-> statements.push_back(STATEMENT());

        accept(RBRACE_SY);

        return f_node;
    }

    // STATEMENT = CONDITION | PRINT | LOOP | ASSIGN | DEFINITION | RETURN | FUNCTION_CALL
    Base_node* STATEMENT()
    {
        Base_node* statement;
        if(token.get_token() == IF_SY)
            statement = CONDITION();
        else if(token.get_token() == PRINT_SY)
            statement = PRINT();
        else if(token.get_token() == WHILE_SY)
            statement = LOOP();
        else if(token_in_set(type_set))
            statement = DEFINITION();
        else if(token.get_token() == RETURN_SY)
            statement = RETURN();
        else if(token.get_token() == ID_SY)
        {
            // potrzebny jeszcze nastepny token zeby sie dowiedziec czy przypisanie czy func
            buffer_second_token();

            if(second_token.get_token() == LPAREN_SY)
                statement = FUNCTION_CALL(NEED_SEMI_COL);
            else //if(token.get_token() == ASSIGN_SY)
                statement = ASSIGN(); // assign moze byc tez tej postaci id.ALGO_ATTR = ..., a nie tylko id = ...
        }
        else
            print_error_message("Brak wyrazenia wpisujacego sie w semantyke STATEMENT");

        return statement;
    }

    // CONDITION = IF [ELSE]
    Condition_node* CONDITION()
    {
        Condition_node* c_node = new Condition_node();

        accept(IF_SY);
        accept(LPAREN_SY);
        c_node-> logic = EXPRESION();
        accept(RPAREN_SY);
        accept(LBRACE_SY);
        while(token_in_set(stmt_first_set))
            c_node-> if_statements.push_back(STATEMENT());
        accept(RBRACE_SY);

        if(token.get_token() == ELSE_SY)
        {
            accept(ELSE_SY);
            accept(LBRACE_SY);
            while(token_in_set(stmt_first_set))
                c_node-> else_statements.push_back(STATEMENT());
            accept(RBRACE_SY);
        }

        return c_node;
    }

    // LOGIC = EXPRESION ( "||" | "&&" | "!=" | "==" | "<=" | "<" | ">" | ">=" ) EXPRESION | "~" EXPRESION
    // ARITHMETIC = EXPRESION ( "+" | "-" | "*" | "/") EXPRESION
    // EXPRESION = LOGIC | ARITHMETIC | SIMPLY
    // SIMPLY = VALUE | ID | FUNCTION_CALL | ALGO_ATTR_EMB_FUN
    Base_node* SIMPLY()
    {
        Base_node* simply;

        if(token_in_set(value_set) || token.get_token() == MINUS_SY)
            simply = VALUE();

        else if(token.get_token() == ID_SY)
        {
            // potrzebny jeszcze nastepny token zeby sie dowiedziec czy id. czy func
            buffer_second_token();

            if(second_token.get_token() == LPAREN_SY)
                simply = FUNCTION_CALL(0);

            else if(second_token.get_token() == DOT_SY)
                simply = ALGO_ATTR_EMB_FUN();

            else
                simply = ID(); // jesli nic nie dopasowano to to bedzie samo id
        }
        else
            print_error_message("Brak wyrazenia wpisujacego sie w semantyke SIMPLY");

        return simply;
    }

    // ALGO_ATTR_EMB_FUN = ID "." (ALGO_ATTR | EMBEDDED_FUNCTION)
    Base_node* ALGO_ATTR_EMB_FUN()
    {
        Id_node* i_node = new Id_node();

        accept(ID_SY, i_node); // to Id_node
        accept(DOT_SY);

        // moze byc id.ALGO_ATTR; i id.id(..), wiec trzeba zobaczyc co jest dalej
        Base_node* algo_attr_emb_fun;

        if(token_in_set(algo_attr_set)) // id.ALGO_ATTR
        {
            Embedded_attr_node* ea_node = new Embedded_attr_node();
            ea_node-> id = i_node-> id; // przepisuje z pomocniczego
            accept(algo_attr_set, ea_node); // wrzucam atrybut do node

            algo_attr_emb_fun = ea_node;
        }
        else // id.FUN(..)
        {
            Embedded_fun_node* ef_node = new Embedded_fun_node();
            ef_node-> id = i_node-> id; // przepisuje z pomocniczego
            accept(embedded_fun_set, ef_node); // dorzucam nazwe funkcji wbudowanej
            accept(LPAREN_SY);
            accept(ID_SY, ef_node); // dorzuc do node nazwe zmiennej z () konstruktora
            accept(RPAREN_SY);

            algo_attr_emb_fun = ef_node;
        }

        free(i_node);
        return algo_attr_emb_fun;
    }

    Id_node* ID()
    {
        Id_node* i_node = new Id_node();
        accept(ID_SY, i_node);
        return i_node;
    }

    Value_node* VALUE()
    {
        Value_node* v_node = new Value_node();
        if(token.get_token() == MINUS_SY)
            accept(MINUS_SY, v_node);
        accept(value_set, v_node);
        return v_node;
    }

    Base_node* build_paren()
    {
        accept(LPAREN_SY);

        bool was_neg = false;
        Negation_node* wrapper = new Negation_node();

        if(token.get_token() == NOT_SY)
        {
            accept(NOT_SY);
            was_neg = true;
        }

        Base_node* expr;
        if(token.get_token() == LPAREN_SY)
        {
            Base_node* paren = build_paren();
            Expresion_node* expresion = build_node(paren);
            accept(RPAREN_SY);
            return expresion;
        }

        if(was_neg)
            wrapper -> simply = SIMPLY();
        else
            expr = SIMPLY();

        // jesli od razu za simply jest ) to znaczy ze jest np (2)
        if(token.get_token() == RPAREN_SY)
        {
            accept(RPAREN_SY);
            return was_neg ? wrapper : expr;
        }
        else
        {
            // zrobi az od )
            Expresion_node* e_node = was_neg ? build_node(wrapper) : build_node(expr);
            accept(RPAREN_SY);
            return e_node;
        }
    }

    Base_node* build_high()
    {
        bool was_neg = false;
        Negation_node* wrapper = new Negation_node();

        if(token.get_token() == NOT_SY)
        {
            accept(NOT_SY);
            was_neg = true;
        }

        Base_node* expr;
        if(token.get_token() == LPAREN_SY)
        {
            if(was_neg)
                wrapper -> simply = build_paren();
            else
                expr = build_paren();
        }
        else
        {
            if(was_neg)
                wrapper -> simply = SIMPLY();
            else
                expr = SIMPLY();
        }

        if(token_in_set(expr_operator_high_set))
        {
            Expresion_node* expr_node = new Expresion_node();
            expr_node->left = was_neg ? wrapper : expr;
            accept(expr_operator_high_set, expr_node);
            expr_node->right = build_high();
            return expr_node;
        }
        else // jesli za wyrazeniem prostym nie ma operatora * to jest to + lub koniec wyrazenia calego, jak plus to kolejne wywolanie pozniej
            return was_neg ? wrapper : expr;
    }

    Base_node* build_low()
    {
        bool was_neg = false;
        Negation_node* wrapper = new Negation_node();

        if(token.get_token() == NOT_SY)
        {
            accept(NOT_SY);
            was_neg = true;
        }

        Base_node* expr;
        if(token.get_token() == LPAREN_SY)
        {
            if(was_neg)
                wrapper -> simply = build_paren();
            else
                expr = build_paren();
        }
        else
        {
            if(was_neg)
                wrapper -> simply = SIMPLY();
            else
                expr = SIMPLY();
        }

        Expresion_node* expr_node = nullptr;
        if(token_in_set(expr_operator_low_set))
        {
            expr_node = new Expresion_node();
            expr_node->left = was_neg ? wrapper : expr;
            accept(expr_operator_low_set, expr_node);
            expr_node->right = build_low();
        }
        else if(token_in_set(expr_operator_high_set))
        {
            expr_node = new Expresion_node();
            expr_node->left = was_neg ? wrapper : expr;
            accept(expr_operator_high_set, expr_node);
            expr_node->right = build_high();
        }
        else // jesli za wyrazeniem prostym nie ma operatora to jest to koniec wyrazenia calego
            return was_neg ? wrapper : expr;

        return expr_node;
    }

    Expresion_node* build_node(Base_node* node)
    {
        Expresion_node* e_node = new Expresion_node();

        e_node->left = node;
        if(token_in_set(expr_operator_high_set)) // jesli operator jest w grupie wyzszych priorytetow to nie moge wejsc w funkcje z nizszymi
        {
            accept(expr_operator_high_set, e_node);
            e_node->right = build_high();
        }
        else if(token_in_set(expr_operator_low_set))
        {
            accept(expr_operator_low_set, e_node);
            e_node->right = build_low();
        }

        return e_node;
    }

    // LOOP = "while" "(" LOGIC ")" "{" {STATEMENT} "}"
    Loop_node* LOOP()
    {
        Loop_node* l_node = new Loop_node();

        accept(WHILE_SY);
        accept(LPAREN_SY);
        l_node->logic = EXPRESION();
        accept(RPAREN_SY);
        accept(LBRACE_SY);
        while(token_in_set(stmt_first_set))
            l_node->statements.push_back(STATEMENT());
        accept(RBRACE_SY);

        return l_node;
    }

    // ASSIGN = ID ["." ALGO_ATTR] "=" (EXPRESION | EMBEDDED_DECL) ";"
    Assign_node* ASSIGN()
    {
        Assign_node* a_node = new Assign_node();

        accept(ID_SY, a_node);

        if(token.get_token() == DOT_SY)
        {
            accept(DOT_SY, a_node);
            accept(algo_attr_set, a_node);
        }

        accept(ASSIGN_SY);

        if(token_in_set(embedded_set))
            a_node-> value = EMBEDDED_DECL();
        else
            a_node-> value = EXPRESION();

        accept(SEMICOL_SY);

        return a_node;
    }

    // RETURN = "return" EXPRESION ";"
    Return_node* RETURN()
    {
        Return_node* r_node = new Return_node();

        accept(RETURN_SY);
        r_node->expresion = EXPRESION();
        accept(SEMICOL_SY);

        return r_node;
    }

    // ID "(" {EXPRESION} ")" ";" <- ale srednik w zaleznosci ktora funkcja wywolala np jak assign wywola to srednika byc nie moze bo od assign jest, tak samo if nie moze byc
    Function_call_node* FUNCTION_CALL(int need_semi_col)
    {
        Function_call_node* f_node = new Function_call_node();

        accept(ID_SY, f_node);
        accept(LPAREN_SY);

        if(token_in_set(expr_first_set))
        {
            f_node->expresions.push_back(EXPRESION());
            while(token.get_token() == COMMA_SY)
            {
                accept(COMMA_SY);
                f_node->expresions.push_back(EXPRESION());
            }
        }

        accept(RPAREN_SY);
        if(need_semi_col) // byc moze trzeba bedzie poprawic, patrze czy jest w ifie czy ma ) za wywolaniem, chyba nie najlepsza metoda
            accept(SEMICOL_SY);

        return f_node;
    }

    // TYPE ID ["=" EXPRESION | EMBEDDED_DECL] ";"
    Definition_node* DEFINITION()
    {
        Definition_node* d_node = new Definition_node();

        accept(type_set, d_node);
        accept(ID_SY, d_node);
        if(token.get_token() == ASSIGN_SY)
        {
            accept(ASSIGN_SY, d_node);// set is definition = true
            if(token_in_set(embedded_set))
                d_node-> value = EMBEDDED_DECL();
            else
                d_node-> value = EXPRESION();
        }
        accept(SEMICOL_SY);

        return d_node;
    }

    Base_node* EXPRESION()
    {
        bool was_neg = false;
        Negation_node* wrapper = new Negation_node();

        if(token.get_token() == NOT_SY)
        {
            accept(NOT_SY);
            was_neg = true;
        }

        Base_node* expr;
        if(token.get_token() == LPAREN_SY)
        {
            if(was_neg)
                wrapper -> simply = build_paren();
            else
                expr = build_paren();
        }
        else
        {
            if(was_neg)
                wrapper -> simply = SIMPLY();
            else
                expr = SIMPLY();
        }

        if(token_in_set(expr_operator_set))
        {
            Expresion_node* e_node = was_neg ? build_node(wrapper) : build_node(expr);

            while(token_in_set(expr_operator_set))
                e_node = build_node(e_node);

            return e_node;
        }

        return was_neg ? wrapper : expr;
    }

    // EMBEDDED_DECL = EMBEDDED "(" VALUE ")"
    Embedded_decl_node* EMBEDDED_DECL()
    {
        Embedded_decl_node* e_node = new Embedded_decl_node();

        TOKEN_SY emb_type = token.get_token();
        accept(embedded_set, e_node);
        accept(LPAREN_SY);

        // dozwolone tylko inicjalizowanie stringiem lub intem
        if(token.get_token() == STRING_CONTENT_SY)
        {
            if(emb_type == ALGORITHM_SY)
                accept(STRING_CONTENT_SY, e_node);
            else
                print_error_message("Inicjalizowanie typu Algorithm mozliwe tylko stringiem");
        }
        else
        {
            if(emb_type == USER_SY)
                accept(NUMBER_INT_SY, e_node);
            else
                print_error_message("Inicjalizowanie typu User mozliwe tylko intem");
        }
        accept(RPAREN_SY);
        // bez srednika bo gwarantuje to funkcja wywolujaca

        return e_node;
    }

    // PRINT = "print" "(" ( ID | VALUE) ")" ";"
    Print_node* PRINT()
    {
        Print_node* p_node = new Print_node();

        accept(PRINT_SY);
        accept(LPAREN_SY);

        if(token_in_set(value_set))
            accept(value_set, p_node);
        else
        {
            accept(ID_SY, p_node);
            if(token.get_token() == DOT_SY)
            {
                accept(DOT_SY, p_node);
                accept(algo_attr_set, p_node);
            }
        }

        accept(RPAREN_SY);
        accept(SEMICOL_SY);

        return p_node;
    }
};


#endif
