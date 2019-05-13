#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>

#include "Parser.h"

struct Function_descr
{
    std::string ret_type;
    unsigned num_of_params;

    Function_descr(std::string r, unsigned n) : ret_type(r), num_of_params(n)
    {}

    Function_descr()
    {}
};

class Interpreter
{
    std::string filename;
    std::ofstream file;
    Parser& parser;
    Program_node* root;

    int action_type; // 0 - nie ma akcji, 1 - przypisanie do zmiennej, 2 - porownanie returna z funkcji
    std::string id_of_action; // nazwa zmiennej do ktorej jest przypisywane cos
    std::string current_fun; // id biezacej funkcji, zmieniane co przejscie z funkcji, po to by moc porownac return z typem zadeklarowanym
    std::map<std::string, std::string> symbols_array; // tablica symboli: zmienne w funkcji aktualnej jakiego sa typu
    std::map<std::string, std::string> emb_symbols_array; // tablica symboli: pola wbudowane jakiego sa typu
    std::map<std::string, Function_descr> functions_array; // tablica funkcji w programie
    std::map<std::string, std::string> emb_field_member; // ktore pole nalezy do ktorego typu wbudowanego
    std::map<std::string, std::string> emb_fun_ret; // co zwracaja funkcje wbudowane

public:
    Interpreter(std::string filename, Parser &p) : parser(p)
    {
        action_type = 0;
        file.open(filename.c_str());
        if(!file.is_open())
        {
            std::cout<< "Nie udalo sie otworzyc pliku!!!"<<std::endl;
            exit(1);
        }

        root = parser.PROGRAM();
        root->to_string("");

        prepare_arrays();
    }

    ~Interpreter()
    {
        file.close();
    }

    void prepare_arrays_for_fun(Function_node* fun)
    {
        functions_array[fun->id] = Function_descr(fun->ret_type, fun->parameters.size());
        current_fun = fun->id;

        // kazda funkcja ma swoja wlasna tablice symboli
        symbols_array.clear();

        // kazdy musi miec dostep do wbudowanych pol
        symbols_array = emb_symbols_array;
    }

    void error_message(std::string message)
    {
        std::cout<< message;
        file << " (\"" << message << "\" !!!^ ERROR ^!!!)";
        file.close();
        exit(0);
    }

    void interpret_and_evaluate(Base_node* node, std::string tabs)
    {
        switch(node->node_type)
        {
        case CONDITION_NODE:
            evaluate_cond((Condition_node*)node, tabs);
            break;
        case PRINT_NODE:
            evaluate_print((Print_node*)node, tabs);
            break;
        case LOOP_NODE:
            evaluate_loop((Loop_node*)node, tabs);
            break;
        case VALUE_NODE:
            evaluate_value((Value_node*) node, tabs);
            break;
        case DEFINITION_NODE:
            evaluate_definition((Definition_node*)node, tabs);
            break;
        case EMBEDDED_DECL_NODE:
            evaluate_emb_decl((Embedded_decl_node*)node);
            break;
        case FUNCTION_CALL_NODE:
            evaluate_fun_call((Function_call_node*)node, tabs);
            break;
        case RETURN_NODE:
            evaluate_return((Return_node*) node, tabs);
            break;
        case ID_NODE:
            evaluate_id((Id_node*)node, tabs);
            break;
        case EMBEDDED_ATTR_NODE:
            evaluate_emb_attr((Embedded_attr_node*)node, tabs);
            break;
        case EMBEDDED_FUN_NODE:
            evaluate_emb_fun((Embedded_fun_node*)node, tabs);
            break;
        case ASSIGN_NODE:
            evaluate_assign((Assign_node*)node, tabs);
            break;
        case EXPRESION_NODE:
            evaluate_expr((Expresion_node*)node);
            break;
        case NEGATION_NODE:
            evaluate_negation_node((Negation_node*)node);
            break;
        default:
            break;
        }
    }

    void evaluate_tree()
    {
        if(root->node_type != PROGRAM_NODE)
            error_message("Pierwszy wezel drzewa jest inny niz PROGRAM_NODE");

        file << "#include <iostream>\n";
        file << "#include \"Embedded_types.h\"\n";
        file << "\n\n";

        for(auto fun : root->functions)
            evaluate_function(fun);
    }

    void evaluate_function(Function_node* fun)
    {
        prepare_arrays_for_fun(fun);

        // jesli funkcja jest mainem to musi byc noret
        if(fun->id == "main")
        {
            if(fun-> ret_type != "noret")
                error_message("Main functon must be noret");

            // main w cpp jest int i return 0;
            file << "int ";
        }
        else
        {
            // funkcja nie jest mainem i okreslam jej typ
            if(fun-> ret_type == "noret")
            {
                file << "void ";
                // sprawdzenie czy ktoras instrukcja biezacej funkcji to nie jest return (bo moze byc pare return)
                for(auto ret_stmt = fun->statements.begin(); ret_stmt != fun->statements.end(); ret_stmt++)
                {
                    if((*ret_stmt)->node_type == RETURN_NODE)
                        error_message("Funkcja noret nie moze niczego zwracac");
                }
            }
            else
                file << fun-> ret_type << " ";
        }


        // naglowek funkcji
        file << fun-> id << "(";
        for(auto param = fun->parameters.begin(); param != fun->parameters.end(); param++)
        {
            file << param->param_type << " " << param->param_id;
            if(symbols_array.count(param->param_id) != 0)
                error_message("<-- W definicji funkcji nazwa argumentu jest zajeta");

            // argumenty tez wchodza do puli zarezerwowanych id
            symbols_array[param->param_id] = param->param_type;

            if(param != fun-> parameters.end()-1)
                file << ", ";
        }
        file << ")\n";

        file << "{\n";

        for(auto stmt : fun->statements)
            interpret_and_evaluate(stmt, "\t");

        if(fun->id == "main")
            file << "\n\treturn 1;\n";

        file << "}\n\n";
    }

    void evaluate_cond(Condition_node* cond, std::string tabs)
    {
        file << tabs+"if(";
        evaluate_expr((Expresion_node*)cond->logic);
        file << ")\n";

        file << tabs+"{\n";
        for(auto ifs : cond->if_statements)
            interpret_and_evaluate(ifs, tabs+"\t");
        file << tabs+"}\n";

        if(cond->else_statements.size() > 0)
        {
            file << tabs+"else\n";
            file << tabs+"{\n";
            for(auto els : cond->else_statements)
                interpret_and_evaluate(els, tabs+"\t");
            file << tabs+"}\n";
        }
    }

    void evaluate_loop(Loop_node* loop, std::string tabs)
    {
        file << tabs+"while(";
        evaluate_expr((Expresion_node*)loop->logic);
        file << ")\n";

        file << tabs+"{\n";
        for(auto stmt : loop->statements)
            interpret_and_evaluate(stmt, tabs+"\t");
        file << tabs+"}\n";
    }

    void evaluate_negation_node(Negation_node* expr)
    {
        file << "!";
        if(expr->simply->node_type == EXPRESION_NODE)
        {
            file << "(";
            evaluate_expr((Expresion_node*)expr->simply);
            file << ")";
        }
        else
            interpret_and_evaluate(expr->simply, "");
    }

    void evaluate_fun_call(Function_call_node* fun_call, std::string tabs)
    {
        file << tabs+fun_call-> id << "(";

        if(functions_array.count(fun_call->id) == 0)
            error_message("<-- Funkcja o takiej nazwie nie byla zdefiniowana");

        if(fun_call->expresions.size() != functions_array[fun_call->id].num_of_params)
            error_message("<-- Istnieje funkcja o takiej nazwie ale innej liczbie parametrow wywolania");

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], functions_array[fun_call->id].ret_type))
                    error_message("<-- Zmienna przed = nie jest typu zwracanego przez funkcje");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, functions_array[fun_call->id].ret_type))
                    error_message("<-- Typ zwracany przez funkcje nie zgodny z typem w return");
            }
        }

        // zeby nie porownywalo typow parametrow funkcji z ew przypisaniem do zmiennej wyniku tej funkcji, na czas wypisania wywolania funkcji zakrywam action
        int last_action = action_type;
        action_type = 0;

        for(auto param = fun_call->expresions.begin(); param != fun_call->expresions.end(); param++)
        {
            interpret_and_evaluate(*param, "");

            if(param != fun_call->expresions.end()-1)
                file << ", ";
        }
        file << ")";

        action_type = last_action;
    }

    void evaluate_emb_fun(Embedded_fun_node* emb_fun, std::string tabs)
    {
        file << tabs+emb_fun->id;

        // sprawdzam czy zmienna przed kropka byla zdefiniowana
        if(symbols_array.count(emb_fun->id) == 0)
            error_message("<-- Zmienna nie byla zdefiniowana");

        file << "." << emb_fun->fun_name << "(";

        // sprawdzam czy zmienna przed kropka jest obiektem tego typu wbudowanego co funkcja
        if(symbols_array[emb_fun->id] != emb_field_member[emb_fun->fun_name])
            error_message("<-- Zmienna nie ma takiego pola");

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], emb_fun_ret[emb_fun->fun_name]))
                    error_message("<-- Zmienna przed = nie jest typu zwracanego przez funkcje");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, emb_fun_ret[emb_fun->fun_name]))
                    error_message("<-- Typ zwracany przez funkcje nie zgodny z typem w return");
            }
        }

        file << emb_fun->value << ")";

        // sprawdzam czy zmienna bedaca argumentem byla zdefiniowana
        if(symbols_array.count(emb_fun->value) == 0)
            error_message("<-- Zmienna nie byla zdefiniowana");

        // sprawdzam czy typ argumentu w funkcji zgadza sie z tym z typu wbudowanego
        if(emb_fun->fun_name == "rankRecomendation") // rankRecomendation(Path)
        {
            if(symbols_array[emb_fun->value] != "Path")
                error_message("<-- Typ argumentu nie zgadza sie z typem argumentu funkcji typu wbudowanego");
        }
        else // getRecomendation(User)
        {
            if(symbols_array[emb_fun->value] != "User")
                error_message("<-- Typ argumentu nie zgadza sie z typem argumentu funkcji typu wbudowanego");
        }
    }

    void evaluate_emb_decl(Embedded_decl_node* decl)
    {
        file << decl->embedded_type << "(" << decl->value << ")";

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], decl->embedded_type))
                    error_message("<-- Niezgodnosc typow");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, decl->embedded_type))
                    error_message("<-- Niezgodnosc zwracanego typu przez funkcje");
            }
        }
    }

    void evaluate_emb_attr(Embedded_attr_node* emb_attr, std::string tabs)
    {
        file << tabs+emb_attr->id << ".";

        // sprawdzam czy zmienna przed kropka byla zdefiniowana
        if(symbols_array.count(emb_attr->id) == 0)
            error_message("<-- Zmienna nie byla zdefiniowana");

        // sprawdzam czy zmienna przed kropka jest tego typu co typ wbudowany majacy ten atrybut
        if(symbols_array[emb_attr->id] != emb_field_member[emb_attr->attribute])
            error_message("<-- Zmienna nie jest tego typu co zwracany typ lub zmienna nie ma takiego pola");

        file << emb_attr->attribute;

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], symbols_array[emb_attr->attribute]))
                    error_message("<-- Niezgodnosc typow");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, symbols_array[emb_attr->attribute]))
                    error_message("<-- Niezgodnosc zwracanego typu przez funkcje");
            }
        }
    }

    void evaluate_print(Print_node* print, std::string tabs)
    {
        file << tabs+"std::cout << ";

        file << print->content;

        if(print->is_id_in_content)
        {
            if(print->is_dot)
            {
                file << "." << print->after_dot;

                if(emb_symbols_array.count(print->after_dot) == 0)
                    error_message("<-- Pole nie ma takiego atrybutu");
            }
            else
            {
                if(symbols_array.count(print->content) == 0)
                    error_message("<-- Zmienna o tej nazwie nie jest zdefiniowana");
            }
        }

        file << ";\n";
    }

    void evaluate_return(Return_node* ret, std::string tabs)
    {
        file << tabs+"return ";
        action_type = 2;
        id_of_action = current_fun;
        interpret_and_evaluate(ret->expresion, "");
        action_type = 0;
        file << ";\n";
    }

    void evaluate_expr(Expresion_node* expr)
    {
        if(expr->left->node_type == EXPRESION_NODE)
        {
            file << "(";
            interpret_and_evaluate(expr->left, "");
            file << ")";
        }
        else
            interpret_and_evaluate(expr->left, "");

        file << " " << expr->operation << " ";

        if(expr->right->node_type == EXPRESION_NODE)
        {
            file << "(";
            interpret_and_evaluate(expr->right, "");
            file << ")";
        }
        else
            interpret_and_evaluate(expr->right, "");
    }

    void evaluate_id(Id_node* idn, std::string tabs)
    {
        file << tabs+idn->id;

        if(symbols_array.count(idn->id) == 0)
            error_message("<-- Zmienna o tej nazwie nie jest zdefiniowana");

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], symbols_array[idn->id]))
                    error_message("<-- Niezgodnosc typow");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, symbols_array[idn->id]))
                    error_message("<-- Niezgodnosc zwracanego typu przez funkcje");
            }
        }
    }

    void evaluate_value(Value_node* val, std::string tabs)
    {
        if(val->minus_value)
        {
            file << tabs+"-";
            file << val->value;
        }
        else
            file << tabs+val->value;

        if(action_type > 0)
        {
            if(action_type == 1)
            {
                if(!compatible_types(symbols_array[id_of_action], val->type_value))
                    error_message("<-- Niezgodnosc typow");
            }
            else
            {
                if(!compatible_types(functions_array[id_of_action].ret_type, val->type_value))
                    error_message("<-- Niezgodnosc zwracanego typu przez funkcje");
            }
        }
    }

    void evaluate_assign(Assign_node* assi, std::string tabs)
    {
        if(assi->is_algo_attr)
        {
            file << tabs+assi->id << "." << assi->algo_attr << " = ";
            id_of_action = assi->algo_attr;

            if(symbols_array[assi->id] != emb_field_member[assi->algo_attr])
                error_message("<-- Zmienna nie ma takiego pola");
        }
        else
        {
            file << tabs+assi->id << " = ";
            id_of_action = assi->id;
        }

        if(symbols_array.count(assi->id) == 0)
            error_message("<-- Zmienna nie byla zdefiniowana");

        action_type = 1;
        interpret_and_evaluate(assi->value, "");
        action_type = 0;
        file << ";\n";
    }

    void evaluate_definition(Definition_node* def, std::string tabs)
    {
        file << tabs+def->type << " " << def->id;

        if(symbols_array.count(def->id) != 0)
            error_message("<-- Redefinicja zmiennej - nazwa jest zajeta");

        symbols_array[def->id] = def->type;

        if(def->is_value)
        {
            file << " = ";
            action_type = 1;
            id_of_action = def->id;
            interpret_and_evaluate(def->value, "");
            action_type = 0;
        }
        file << ";\n";
    }

    bool compatible_types(std::string first, std::string second)
    {
        if(first == "string")
            return second == "string" ? true : false;

        else if(first == "bool")
            return second == "bool" ? true : false;

        else if(first == "Algorithm")
            return second == "Algorithm" ? true : false;

        else if(first == "User")
            return second == "User" ? true : false;

        else if(first == "Path")
            return second == "Path" ? true : false;

        // pierwszy typ to int lub float
        else
            return (second == "int" || second == "float") ? true : false;
    }

    void prepare_arrays()
    {
        // inicjalizacja tablicy symboli typow wbudowanych
        emb_symbols_array["id"] = "int";
        emb_symbols_array["grade"] = "float";
        emb_symbols_array["country"] = "string";

        emb_symbols_array["name"] = "string";
        emb_symbols_array["pearsonGroupSize"] = "int";
        emb_symbols_array["regParameter"] = "float";
        emb_symbols_array["lambda"] = "float";

        emb_symbols_array["id"] = "int";
        emb_symbols_array["country"] = "string";
        emb_symbols_array["difficulty"] = "float";
        emb_symbols_array["length"] = "float";

        // inicjalizacja przynaleznosci pol do danych typow
        emb_field_member["id"] = "User";
        emb_field_member["grade"] = "User";
        emb_field_member["country"] = "User";

        emb_field_member["name"] = "Algorithm";
        emb_field_member["pearsonGroupSize"] = "Algorithm";
        emb_field_member["regParameter"] = "Algorithm";
        emb_field_member["lambda"] = "Algorithm";

        emb_field_member["id"] = "Path";
        emb_field_member["country"] = "Path";
        emb_field_member["difficulty"] = "Path";
        emb_field_member["length"] = "Path";

        emb_field_member["rankRecomendation"] = "User";
        emb_field_member["getRecomendation"] = "Algorithm";

        // co zwracaja funkcje
        emb_fun_ret["rankRecomendation"] = "float";
        emb_fun_ret["getRecomendation"] = "Path";
    }
};


#endif

