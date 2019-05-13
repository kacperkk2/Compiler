#ifndef NODES_H
#define NODES_H

#include <vector>


enum Node_type
{
    PROGRAM_NODE, FUNCTION_NODE, CONDITION_NODE, PRINT_NODE, LOOP_NODE,
    VALUE_NODE, DEFINITION_NODE, EMBEDDED_DECL_NODE, FUNCTION_CALL_NODE, RETURN_NODE,
    ID_NODE, EMBEDDED_ATTR_NODE, EMBEDDED_FUN_NODE, ASSIGN_NODE, EXPRESION_NODE, NEGATION_NODE
};

struct Parameter
{
    std::string param_type;
    std::string param_id;

    Parameter(std::string type, std::string id)
    {
        param_type = type;
        param_id = id;
    }
};

struct Base_node
{
    Node_type node_type;

    virtual void to_string(std::string tabs)=0;
};

struct Assign_node : Base_node
{
    std::string id;
    bool is_algo_attr; // czy jest odwolanie sie do wbudowanego pola
    std::string algo_attr;
    Base_node* value;

    Assign_node() {node_type = ASSIGN_NODE; is_algo_attr = false;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Assign_node {"<<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        if(is_algo_attr)
            std::cout<<tabs+" algo_attr: "<< algo_attr <<std::endl;
        value->to_string(tabs + "\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Embedded_fun_node : Base_node
{
    std::string id;
    std::string fun_name;
    std::string value;

    Embedded_fun_node() {node_type = EMBEDDED_FUN_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Embedded_fun_node {"<<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        std::cout<<tabs+" fun_name: "<< fun_name <<std::endl;
        std::cout<<tabs+" value: "<< value <<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Embedded_attr_node : Base_node
{
    std::string id;
    std::string attribute;

    Embedded_attr_node() {node_type = EMBEDDED_ATTR_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Embedded_attr_node {"<<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        std::cout<<tabs+" attribute: "<< attribute <<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Return_node : Base_node
{
    Base_node* expresion;

    Return_node() {node_type = RETURN_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Return_node {"<<std::endl;
        expresion->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Function_call_node : Base_node
{
    std::string id;
    std::vector<Base_node*> expresions; // tu sa kolejne parametry wywolania funkcji

    Function_call_node() {node_type = FUNCTION_CALL_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Function_call_node {"<<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        for (auto &exp : expresions)
            exp->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Embedded_decl_node : Base_node
{
    std::string embedded_type;
    std::string value;

    Embedded_decl_node() {node_type = EMBEDDED_DECL_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Embedded_decl_node {"<<std::endl;
        std::cout<<tabs+" embedded_type: "<< embedded_type <<std::endl;
        std::cout<<tabs+" value: "<< value <<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Definition_node : Base_node
{
    std::string type;
    std::string id;
    bool is_value; // czy jest definicja czy sama deklaracja
    Base_node* value;

    Definition_node() {node_type = DEFINITION_NODE; is_value = false;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Definition_node {"<<std::endl;
        std::cout<<tabs+" type: "<< type <<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        if(is_value)
            value->to_string(tabs + "\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Id_node : Base_node
{
    std::string id;

    Id_node() {node_type = ID_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Id_node {"<<std::endl;
        std::cout<<tabs+" id: "<< id <<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Value_node : Base_node
{
    std::string value;
    std::string type_value;
    bool minus_value;

    Value_node() {node_type = VALUE_NODE; minus_value = false;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Value_node {"<<std::endl;
        std::cout<<tabs+" minus_value: "<< minus_value <<std::endl;
        std::cout<<tabs+" value: "<< value <<std::endl;
        std::cout<<tabs+" type_value: "<< type_value <<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Negation_node : Base_node
{
    Base_node* simply;

    Negation_node() {node_type = NEGATION_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Negation_node {"<<std::endl;
        simply->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Expresion_node : Base_node
{
    std::string operation;
    Base_node* left;
    Base_node* right;

    Expresion_node() {node_type = EXPRESION_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Expresion_node {"<<std::endl;
        left->to_string(tabs+"\t");
        std::cout<<tabs+" operation: "<< operation <<std::endl;
        right->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Loop_node : Base_node
{
    Base_node* logic;
    std::vector<Base_node*> statements;

    Loop_node() {node_type = LOOP_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Loop_node {"<<std::endl;
        logic->to_string(tabs + "\t");
        for (auto &stm : statements)
            stm->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Print_node : Base_node
{
    std::string content;
    bool is_id_in_content;
    bool is_dot;
    std::string after_dot;

    Print_node() {node_type = PRINT_NODE; is_id_in_content = false; is_dot = false;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Print_node {"<<std::endl;
        std::cout<<tabs+" content: "<<content<<std::endl;
        std::cout<<tabs+" after_dot: "<<after_dot<<std::endl;
        std::cout<<tabs+" is_id_in_content: "<<is_id_in_content<<std::endl;
        std::cout<<tabs+" is_dot: "<<is_dot<<std::endl;
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Condition_node : Base_node
{
    Base_node* logic;
    std::vector<Base_node*> if_statements;
    std::vector<Base_node*> else_statements;

    Condition_node() {node_type = CONDITION_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Condition_node {"<<std::endl;
        logic->to_string(tabs + "\t");
        for (auto &ifs : if_statements)
            ifs->to_string(tabs+"\t");
        for (auto &els : else_statements)
            els->to_string(tabs+"\t");
        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Function_node : Base_node
{
    std::string id;
    std::string ret_type;
    std::vector<Parameter> parameters;
    std::vector<Base_node*> statements; // tu moze byc condition, print, assign ...

    // pomocnicze
    bool got_ret_and_id; // mowi czy ma juz zwracany typ i id funkcji
    std::string param_type; // schowek, jak dostane id parametru to pakuje calosc do parameters

    Function_node() {node_type = FUNCTION_NODE; got_ret_and_id = false;}

    void to_string(std::string tabs)
    {
        std::cout<<tabs+"Function_node {"<<std::endl;
        std::cout<<tabs+" ret_type: "<<ret_type<<std::endl;
        std::cout<<tabs+" id: "<<id<<std::endl;
        std::cout<<tabs+"Parameters: "<<std::endl;
        for (auto &par : parameters)
            std::cout<<tabs+" "<< par.param_type<<" "<<par.param_id<<std::endl;
        for (auto &stm : statements)
            stm->to_string(tabs+"\t");

        std::cout<<tabs+"}"<<std::endl;
    }
};

struct Program_node : Base_node
{
    std::vector<Function_node*> functions;

    Program_node() {node_type = PROGRAM_NODE;}

    void to_string(std::string tabs)
    {
        std::cout<<"Program_node {"<<std::endl;
        for (auto &fun : functions)
        {
            fun->to_string(tabs + "\t");
        }
        std::cout<<"}"<<std::endl;
    }
};


#endif
