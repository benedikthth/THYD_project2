
#include "hparser.h"

using namespace std;

int HParser::parse()
{
    set_AST( program() );
    return 0;
}

ProgramNode*
HParser::program() {
    match( decaf::token_type::kwClass );
    string name = token_.lexeme;
    match( decaf::token_type::Identifier );
    match( decaf::token_type::ptLBrace );
    auto list_vdn = variable_declarations();
    match( decaf::token_type::ptRBrace );
    match( decaf::token_type::EOI );
    return new ProgramNode(name, list_vdn, nullptr);
}


list<VariableDeclarationNode*>*
HParser::variable_declarations()
{
    auto list_vdn = new list<VariableDeclarationNode*>();
    while ( token_.type == decaf::token_type::kwInt ||
            token_.type == decaf::token_type::kwReal ) {
        ValueType type = this->type();
        auto list_v = variable_list();
        list_vdn->push_back( new VariableDeclarationNode( type, list_v ) );
    }
    return list_vdn;
}


ValueType HParser::type()
{
    ValueType valuetype = ValueType::VoidVal;
    if ( token_.type == decaf::token_type::kwInt ) {
        match( decaf::token_type::kwInt );
        valuetype = ValueType::IntVal;
    }
    else if ( token_.type == decaf::token_type::kwReal ) {
        match( decaf::token_type::kwReal );
        valuetype = ValueType::RealVal;
    }
    else {
       error( decaf::token_type::kwInt );
    }
    return valuetype;
}


list<VariableExprNode*>*
HParser::variable_list()
{
    auto list_v = new list<VariableExprNode*>();
    list_v->push_back( variable() );
    while ( token_.type == decaf::token_type::ptComma ) {
        match( decaf::token_type::ptComma );
        list_v->push_back( variable() );
    }
    match( decaf::token_type::ptSemicolon );
    return list_v;
}


VariableExprNode*
HParser::variable()
{
    auto node = new VariableExprNode(token_.lexeme);
    match( decaf::token_type::Identifier );
    return node;
}

list<MethodNode*>*
HParser::method_declarations()
{
    auto list_mdn = new list<MethodNode*>();
    list_mdn->push_back(method_declaration());
    while(token_.type == decaf::token_type::kwStatic) {
        list_mdn->push_back(method_declaration());
    }
    return list_mdn;
}

MethodNode* HParser::method_declaration()
{
    if( token_.type == decaf::token_type::kwStatic) {
        match(decaf::token_type::kwStatic);
    }
    ValueType return_type = this -> method_return_type();
}

ValueType HParser::method_return_type()
{
    ValueType valuetype = ValueType::VoidVal;
    if ( token_.type == decaf::token_type::kwVoid) {
        match( decaf::token_type::kwVoid);
        valuetype = ValueType::VoidVal;
    }
    else {
        valuetype = this -> type();
    }
    return valuetype;
}

list<ParameterNode*>*
HParser::parameters()
{
    auto list_par = new list<ParameterNode*>();
    if ( token_.type == decaf::token_type::kwInt ||
        token_.type == decaf::token_type::kwReal ) {
        parameter_list(list_par);
    }
    return list_par;
}

list<ParameterNode*>*
HParser::parameter_list(list<ParameterNode*>* list_par)
{
    ValueType par_type = this->type();
    list_par->push_back(new ParameterNode(par_type, new VariableExprNode(token_.lexeme)));
    match(decaf::token_type::Identifier);
    while(token_.type == decaf::token_type::ptComma) {
        par_type = this->type();
        list_par->push_back(new ParameterNode(par_type, new VariableExprNode(token_.lexeme)));
        match(decaf::token_type::Identifier);
    }
    return list_par;
}
