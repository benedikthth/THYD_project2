////////////////////////////////////////////////////////////////////////////////////

%skeleton "lalr1.cc"
%require "3.0.4"
%defines
%define parser_class_name {parser_decaf}

%define api.token.constructor
%define api.value.type variant

%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires
{
#include <string>
#include "ast.h"
class Parser;
}

%parse-param { Parser& driver }
%locations
%code
{
#include "parser.h"
}

%token EOI 0

%token kwClass
%token kwStatic
%token kwVoid
%token kwInt
%token kwReal
%token kwReturn
%token kwBreak
%token kwContinue
%token kwIf
%token kwElse
%token kwFor

%token ptLBrace
%token ptRBrace
%token ptComma
%token ptSemicolon
%token ptLParen
%token ptRParen

%token OpAssign
%token OpArtPlus
%token OpArtMinus
%token OpArtMult
%token OpArtDiv
%token OpArtModulus
%token OpArtInc
%token OpArtDec
%token OpRelEQ
%token OpRelNEQ
%token OpRelLT
%token OpRelLTE
%token OpRelGT
%token OpRelGTE
%token OpLogAnd
%token OpLogOr
%token OpLogNot

%token <std::string> Identifier
%token <std::string> Number
%token <std::string> ErrUnknown


%type <std::list<VariableDeclarationNode*>*> variable_declarations
%type <ValueType> type
%type <std::list<VariableExprNode*>*> variable_list
%type <VariableExprNode*> variable

%%

////////////////////////////////////////////////////////////////////////////////////

program: kwClass Identifier ptLBrace
             variable_declarations
         ptRBrace
         { driver.set_AST( new ProgramNode( $2, $4, nullptr ) ); }

variable_declarations: variable_declarations type variable_list ptSemicolon
                      { $$ = $1; $$->push_back( new VariableDeclarationNode($2,$3) ); }
                    | { $$ = new std::list<VariableDeclarationNode*>(); }

type: kwInt  { $$ = ValueType::IntVal; }
    | kwReal { $$ = ValueType::RealVal; }

variable_list: variable
               { $$ = new std::list<VariableExprNode*>(); $$->push_back( $1 ); }
             | variable_list ptComma variable
               { $$ = $1; $$->push_back( $3 ); }

variable:  Identifier  { $$ = new VariableExprNode($1); }

%%

////////////////////////////////////////////////////////////////////////////////////


void yy::parser_decaf::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}
