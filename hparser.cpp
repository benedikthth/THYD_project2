
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
    auto list_mdn = method_declarations();
    match( decaf::token_type::ptRBrace );
    match( decaf::token_type::EOI );
    return new ProgramNode(name, list_vdn, list_mdn);
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
    match(decaf::token_type::kwStatic);
    ValueType return_type = this->method_return_type();
    string identifier = token_.lexeme;

    match(decaf::token_type::Identifier);
    match(decaf::token_type::ptLParen);

    auto parameters = this->parameters();

    match(decaf::token_type::ptRParen);
    match(decaf::token_type::ptLBrace);

    auto variable_declarations = this->variable_declarations();
    auto statements = this->statement_list();

    match(decaf::token_type::ptRBrace);

    return new MethodNode(return_type,
                          identifier,
                          parameters,
                          variable_declarations,
                          statements);
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
        match(decaf::token_type::ptComma);
        par_type = this->type();
        list_par->push_back(new ParameterNode(par_type, new VariableExprNode(token_.lexeme)));
        match(decaf::token_type::Identifier);
    }
    return list_par;
}

list<StmNode*>*
HParser::statement_list()
{
    auto list_stm = new list<StmNode*>();
    while(token_.type == decaf::token_type::Identifier
          || token_.type == decaf::token_type::kwIf
          || token_.type == decaf::token_type::kwFor
          || token_.type == decaf::token_type::kwReturn
          || token_.type == decaf::token_type::kwBreak
          || token_.type == decaf::token_type::kwContinue
          || token_.type == decaf::token_type::ptLBrace) {
        list_stm->push_back(this->statement());
    }
    return list_stm;
}

StmNode*
HParser::statement()
{
    if(token_.type == decaf::token_type::Identifier) {
        string identifier = token_.lexeme;
        match(decaf::token_type::Identifier);
        if(token_.type==decaf::token_type::ptLParen) {
            match(decaf::token_type::ptLParen);
            auto params = expr_list();
            match(decaf::token_type::ptSemicolon);
            return new MethodCallExprStmNode(identifier, params);
        }
        else {
            auto var = new VariableExprNode(identifier);
            if(token_.type==decaf::token_type::OpAssign) {
                match(decaf::token_type::OpAssign);
                auto assignment = expr();
                match(decaf::token_type::ptSemicolon);
                return new AssignStmNode(var, assignment);
            }
            if (token_.type==decaf::token_type::OpArtInc
                || token_.type==decaf::token_type::OpArtDec) {
                auto incr_decr = op_incr_decr(var);
                match(decaf::token_type::ptSemicolon);
                return incr_decr;
            }
        }
    }
    else if(token_.type == decaf::token_type::kwIf) {
        match(decaf::token_type::kwIf);
        match(decaf::token_type::ptLParen);
        auto expression = expr();
        match(decaf::token_type::ptRParen);
        auto stm_block = statement_block();
        auto opt_else = optional_else();
        return new IfStmNode(expression, stm_block, opt_else);
    }
    else if(token_.type == decaf::token_type::kwFor){
        match(decaf::token_type::kwFor);
        match(decaf::token_type::ptLParen);
        auto var = variable();
        match(decaf::token_type::OpAssign);
        auto assign_stm = new AssignStmNode(var, expr());
        match(decaf::token_type::ptSemicolon);
        auto cond = expr();
        match(decaf::token_type::ptSemicolon);
        auto inc_dec = op_incr_decr(variable());
        match(decaf::token_type::ptRParen);
        auto stm_block = statement_block();
        return new ForStmNode(assign_stm, cond, inc_dec, stm_block);
    }
    else if (token_.type == decaf::token_type::kwReturn){
        match(decaf::token_type::kwReturn);
        auto return_stm = new ReturnStmNode(optional_expr());
        match(decaf::token_type::ptSemicolon);
        return return_stm;
    }
    else if (token_.type == decaf::token_type::kwBreak) {
        match(decaf::token_type::kwBreak);
        auto break_stm = new BreakStmNode();
        match(decaf::token_type::ptSemicolon);
        return break_stm;
    }
    else if (token_.type == decaf::token_type::kwContinue) {
        match(decaf::token_type::kwContinue);
        auto cont_stm = new ContinueStmNode();
        match(decaf::token_type::ptSemicolon);
        return cont_stm;
    }
    else {
        return statement_block();
    }
}

IncrDecrStmNode* HParser::op_incr_decr(VariableExprNode* var)
{
    if(token_.type == decaf::token_type::OpArtInc) {
        match(decaf::token_type::OpArtInc);
        return new IncrStmNode(var);
    }
    else {
        match(decaf::token_type::OpArtDec);
        return new DecrStmNode(var);
    }
}

list<ExprNode*>*
HParser::expr_list()
{
    if(is_expr()) {
        auto lst_expr = new list<ExprNode*>();
        lst_expr->push_back(expr());
        more_expr(lst_expr);
        return lst_expr;
    }
    else {
        return nullptr;
    }
}

void HParser::more_expr(list<ExprNode*>* lst_expr)
{
    while(token_.type == decaf::token_type::ptComma) {
        match(decaf::token_type::ptComma);
        lst_expr->push_back(expr());
    }
}

bool HParser::is_expr() {
    if(token_.type == decaf::token_type::OpLogAnd
       || token_.type == decaf::token_type::OpLogOr
       || token_.type == decaf::token_type::OpLogNot
       || token_.type == decaf::token_type::Number
       || token_.type == decaf::token_type::ptLParen
       || token_.type == decaf::token_type::Identifier
       || token_.type == decaf::token_type::OpRelEQ
       || token_.type == decaf::token_type::OpRelNEQ
       || token_.type == decaf::token_type::OpRelLT
       || token_.type == decaf::token_type::OpRelLTE
       || token_.type == decaf::token_type::OpRelGT
       || token_.type == decaf::token_type::OpRelGTE
       || token_.type == decaf::token_type::OpArtPlus
       || token_.type == decaf::token_type::OpArtMinus
       || token_.type == decaf::token_type::OpArtMult
       || token_.type == decaf::token_type::OpArtDiv
       || token_.type == decaf::token_type::OpArtModulus){
        return true;
       }
       else {
        return false;
       }
}

ExprNode* HParser::optional_expr()
{
    if(is_expr()) {
        return expr();
    }
    else {
        return nullptr;
    }
}

BlockStmNode* HParser::statement_block()
{
    match(decaf::token_type::ptRBrace);
    auto block_stm = new BlockStmNode(statement_list());
    match(decaf::token_type::ptLBrace);
    return block_stm;
}

BlockStmNode* HParser::optional_else()
{
    if(token_.type == decaf::token_type::kwElse) {
        match(decaf::token_type::kwElse);
        return statement_block();
    }
    else {
        return nullptr;
    }
}


ExprNode* HParser::expr()
{
    auto lhs = expr_and();
    return expr_o(lhs);
}

ExprNode* HParser::expr_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpLogOr) {
        match(decaf::token_type::OpLogOr);
        ExprNode* rhs = expr_and();
        ExprNode* node = new OrExprNode(lhs, rhs);
        return expr_o(node);
    }
    else {
        return lhs;
    }
}

ExprNode* HParser::expr_and()
{
    auto lhs = expr_eq();
    return expr_and_o(lhs);
}

ExprNode* HParser::expr_and_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpLogAnd) {
        match(decaf::token_type::OpLogAnd);
        ExprNode* rhs = expr_eq();
        ExprNode* node = new AndExprNode(lhs, rhs);
        return expr_and_o(node);
    }
    else {
        return lhs;
    }
}

ExprNode* HParser::expr_eq()
{
    auto lhs = expr_rel();
    return expr_eq_o(lhs);
}

ExprNode* HParser::expr_eq_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpRelEQ
    || token_.type == decaf::token_type::OpRelNEQ) {
        ExprNode* rhs = expr_rel();
        ExprNode* node = op_eq(lhs, rhs);
        return expr_eq_o(node);
    }
    else {
        return lhs;
    }
}

ExprNode* HParser::op_eq(ExprNode* lhs, ExprNode* rhs)
{
    if(token_.type == decaf::token_type::OpRelEQ) {
        match(decaf::token_type::OpRelEQ);
        return new EqExprNode(lhs, rhs);
    }
    else {
        match(decaf::token_type::OpRelNEQ);
        return new NeqExprNode(lhs, rhs);
    }
}

ExprNode* HParser::expr_rel()
{
    auto lhs = expr_add();
    return expr_rel_o(lhs);
}

ExprNode* HParser::expr_rel_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpRelGT
    || token_.type == decaf::token_type::OpRelGTE
    || token_.type == decaf::token_type::OpRelLT
    || token_.type == decaf::token_type::OpRelLTE) {
        ExprNode* rhs = expr_add();
        ExprNode* node = op_rel(lhs, rhs);
        return expr_rel_o(node);
    }
    else {
        return lhs;
    }
}

ExprNode* HParser::op_rel(ExprNode* lhs, ExprNode* rhs)
{
    if(token_.type == decaf::token_type::OpRelGT) {
        match(decaf::token_type::OpRelGT);
        return new GtExprNode(lhs, rhs);
    }
    else if (token_.type == decaf::token_type::OpRelGTE){
        match(decaf::token_type::OpRelGTE);
        return new GteExprNode(lhs, rhs);
    }
    else if (token_.type == decaf::token_type::OpRelLT){
        match(decaf::token_type::OpRelLT);
        return new LtExprNode(lhs, rhs);
    }
    else if (token_.type == decaf::token_type::OpRelLTE){
        match(decaf::token_type::OpRelLTE);
        return new LteExprNode(lhs, rhs);
    }
}

ExprNode* HParser::expr_add()
{
    auto lhs = expr_mult();
    return expr_add_o(lhs);
}

ExprNode* HParser::expr_add_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpArtPlus
    || token_.type == decaf::token_type::OpArtMinus) {
        ExprNode* rhs = expr_mult();
        ExprNode* node = op_add(lhs, rhs);
        return expr_add_o(node);
    }
    else {
        return lhs;
    }
}

ExprNode* HParser::op_add(ExprNode* lhs, ExprNode* rhs)
{
    if(token_.type == decaf::token_type::OpArtPlus) {
        match(decaf::token_type::OpArtPlus);
        return new PlusExprNode(lhs, rhs);
    }
    else {
        match(decaf::token_type::OpArtMinus);
        return new MinusExprNode(lhs, rhs);
    }
}

ExprNode* HParser::expr_mult()
{
    if(token_.type == decaf::token_type::OpArtPlus
    || token_.type == decaf::token_type::OpArtMinus) {
        auto lhs = expr_add();
        return expr_mult_o(lhs);
    }
    else {
        return expr_mult_o(nullptr);
    }
}

ExprNode* HParser::expr_mult_o(ExprNode* lhs)
{
    if(token_.type == decaf::token_type::OpArtMult
    || token_.type == decaf::token_type::OpArtDiv
    || token_.type == decaf::token_type::OpArtModulus) {
        ExprNode* rhs = expr_unary();
        ExprNode* node = op_mult(lhs, rhs);
        return expr_mult_o(node);
    }
    else {
        return expr_unary();
    }
}

ExprNode* HParser::op_mult(ExprNode* lhs, ExprNode* rhs)
{
    if(token_.type == decaf::token_type::OpArtMult) {
        match(decaf::token_type::OpArtMult);
        return new MultiplyExprNode(lhs, rhs);
    }
    else if (token_.type == decaf::token_type::OpArtDiv){
        match(decaf::token_type::OpArtDiv);
        return new DivideExprNode(lhs, rhs);
    }
    else {
        match(decaf::token_type::OpArtModulus);
        return new ModulusExprNode(lhs, rhs);
    }
}

ExprNode* HParser::expr_unary()
{

    if(token_.type == decaf::token_type::OpArtPlus
    || token_.type == decaf::token_type::OpArtMinus
    || token_.type == decaf::token_type::OpLogNot) {
        //TODO: Implement unary operators
    }
    else {
        return factor();
    }
}

ExprNode* HParser::op_unary(ExprNode* rhs)
{
    if(token_.type == decaf::token_type::OpArtPlus) {
        match(decaf::token_type::OpArtPlus);
        return new PlusExprNode(rhs);
    }
    else if(token_.type == decaf::token_type::OpArtMinus) {
        match(decaf::token_type::OpArtMinus);
        return new MinusExprNode(rhs);
    }
    else {
        match(decaf::token_type::OpLogNot);
        return new NotExprNode(rhs);
    }
}

ExprNode* HParser::factor()
{
    if(token_.type == decaf::token_type::Number) {
        string number = token_.lexeme;
        match(decaf::token_type::Number);
        return new NumberExprNode(number);
    }
    else if(token_.type == decaf::token_type::ptLParen){
        match(decaf::token_type::ptLParen);
        auto node = expr();
        match(decaf::token_type::ptRParen);
        return node;
    }
    else if(token_.type == decaf::token_type::Identifier) {
        string identifier = token_.lexeme;
        match(decaf::token_type::Identifier);
        if(token_.type == decaf::token_type::ptLParen) {
            match(decaf::token_type::ptLParen);
            auto lst_expr = expr_list();
            match(decaf::token_type::ptRParen);
            return new MethodCallExprStmNode(identifier, lst_expr);
        }
    }
}
