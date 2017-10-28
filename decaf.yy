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



%left OpLogOr


%left OpLogAnd


%left OpRelEQ
%left OpRelNEQ


%nonassoc OpRelGT
%nonassoc OpRelGTE
%nonassoc OpRelLT
%nonassoc OpRelLTE


%left OpArtPlus
%left OpArtMinus


%left OpArtModulus
%left OpArtDiv
%left OpArtMult


%left OpLogNot



%type <std::list<VariableDeclarationNode*>*> variable_declarations
%type <ValueType> type
%type <std::list<VariableExprNode*>*> variable_list
%type <VariableExprNode*> variable
%type <std::list<MethodNode*>*> method_declarations
%type <MethodNode*> method_declaration
%type <std::list<ParameterNode *>*> parameters
%type <std::list<ParameterNode *>*> parameter_list

%type <std::list<StmNode*>*> statement_list
%type <StmNode*> statement
%type <std::list<ExprNode *>*> expression_list
%type <ExprNode *> expression
%type <BlockStmNode *> statement_block
%type <BlockStmNode *> optional_else
%type <ExprNode *> optional_expression
%type <IncrDecrStmNode *> incr_decr_var
%type <std::list<ExprNode *>*> more_expressions
%%

////////////////////////////////////////////////////////////////////////////////////

program: kwClass Identifier ptLBrace
             variable_declarations method_declarations
         ptRBrace
         { driver.set_AST( new ProgramNode( $2, $4, $5 ) ); }


parameters: parameter_list
            {$$ = $1; }
            | {$$ = new list<ParameterNode *>(); }

parameter_list: type Identifier
                {$$ = new List<ParameterNode *>(); $$->push_back(new ParameterNode($1, $2) ); }
              | parameter_list ptComma type Identifier
                {$$ = $1; $$->push_back($3, $4); }


variable_declarations: variable_declarations type variable_list ptSemicolon
                      { $$ = $1; $$->push_back( new VariableDeclarationNode($2,$3) ); }
                    | { $$ = new std::list<VariableDeclarationNode*>(); }


method_declarations: method_declarations method_declaration
                      {$$ = $1; $$->push_back( $2 );}
                    | method_declaration
                    { $$ = new std::list<MethodNode *>(); $$->push_back($1); }


method_declaration: kwStatic type Identifier ptLParen parameters ptRParen ptLBrace variable_declarations statement_list ptRBrace
                    {$$ = new MethodNode($2, $3, $5, $8, $9);}


type: kwInt  { $$ = ValueType::IntVal; }
    | kwReal { $$ = ValueType::RealVal; }



variable_list: variable
               { $$ = new std::list<VariableExprNode*>(); $$->push_back( $1 ); }
             | variable_list ptComma variable
               { $$ = $1; $$->push_back( $3 ); }

statement_list: statement_list statement
                { $$ = $1; $$->push_back($2);}
              | { $$ = new std::list<StmNode*>(); }



//????
expression_list: expression more_expressions
                 {$$ = $2; $$->push_back($1); }
                 | {$$ = new std::list<ExprNode *>(); }


more_expressions: ptComma expression more_expressions
                  {$$ = $3; $$->push_back($2); }
                  | {$$ = new std::list<ExprNode *>();  }


statement_block: ptLBrace statement_list ptRBrace
                 { $$ = new BlockStmNode($2); }


optional_else: kwElse statement_block
               { $$ = $2; }
             | { $$ = nullptr; }


optional_expression: expression
                     {$$ = new $1; }
                   | {$$ = nullptr; }


incr_decr_var: variable OpArtInc
               { $$ = new IncrStmtNode($1); }
             | variable OpArtDec
               { $$ = new DecrStmNode($1); }


statement: variable OpAssign expression ptSemicolon
           { $$ = new AssignStmNode($1, $3); }
         | Identifier ptLParen expression_list ptRParen ptSemicolon
           {$$ = new MethodCallExprStmNode($1, $3)}
         | kwIf ptLParen expression ptRParen statement_block optional_else
           {$$ new IfStmNode($3, $5, $6); }
         | kwFor ptLParen variable OpAssign expression ptSemicolon expression ptSemicolon incr_decr_var ptRParen statement_block
           {$$ = new ForStmNode( AssignStmNode($3, $5), $7, $9, $11 ); }
         | kwReturn optional_expression ptSemicolon
           {$$ = new ReturnStmNode($2); }
         | kwBreak ptSemicolon
           {$$ = new BreakStmNode(); }
         | kwContinue ptSemicolon
           {$$ = new ContinueStmNode(); }
         | incr_decr_var ptSemicolon
           {$$ = $1; }
         | statement_block
           {$$ = $1; }



expression: expression OpLogOr expression
            {$$ = new OrExprNode($1, $3); }
          | expression OpLogAnd expression
            {$$ = new AndExprNode($1, $3); }
          | expression OpRelEQ expression
            {$$ = new EqExprNode($1, $3); }
          | expression OpRelNEQ expression
            {$$ = new NeqExprNode($1, $3); }
          | expression OpRelLT expression
            {$$ = new LtExprNode($1, $3); }
          | expression OpRelLTE expression
            {$$ = new LteExprNode($1, $3); }
          | expression OpRelGT expression
            {$$ = new GtExprNode($1, $3); }
          | expression OpRelGTE expression
            {$$ = new GteExprNode($1, $3); }
          | expression OpArtPlus expression
            {$$ = new PlusExprNode($1, $3); }
          | expression OpArtMinus expression
            {$$ = new MinusExprNode($1, $3); }
          | expression OpArtMult expression
            {$$ = new MultiplyExprNode($1, $3); }
          | expression OpArtDiv expression
            {$$ = new DivideExprNode($1, $3); }
          | expression OpArtModulus expression
            {$$ = new ModulusExprNode($1, $3); }
          | OpArtPlus expression %prec OpLogNot
            {$$ = new PlusExprNode($2); }
          | OpArtMinus expression %prec OpLogNot
            {$$ = new MinusExprNode($2); }
          | OpLogNot expression
            {$$ = new NotExprNode($2); }
          | variable
            {$$ = new VariableExprNode($1); }
          | Identifier ptLParen expression_list ptRParen
            {$$ = new MethodCallExprStmNode($1, $3); }
          | Number
            {$$ = new NumberExprNode($1); }
          | ptLBrace expression ptRBrace
            {$$ = $2; }





variable:  Identifier  { $$ = new VariableExprNode($1); }

%%

////////////////////////////////////////////////////////////////////////////////////


void yy::parser_decaf::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}
