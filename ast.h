#ifndef DECAFPARSER_AST_H
#define DECAFPARSER_AST_H

#include <iostream>
#include <string>
#include <list>

/////////////////////////////////////////////////////////////////////////////////

enum class ValueType { VoidVal, IntVal, RealVal };

inline std::string tostr( ValueType t ) {
    return ((t==ValueType::VoidVal) ? "void" : ((t==ValueType::IntVal) ? "int" : "real"));
}

class Node {
public:
    virtual const std::string str( ) const = 0;
    virtual ~Node() = default;
};

inline std::string tostr( const Node* node ) {
    return (node == nullptr ? "(null)" : node->str() );
}


/////////////////////////////////////////////////////////////////////////////////

class ExprNode : public Node { };


class NumberExprNode : public ExprNode
{
public:
    explicit NumberExprNode( const std::string value ) : value_(value) { }

    virtual const std::string str( ) const override {
        return std::string("(NUM ") + value_ + ')';
    }

protected:
    std::string value_;
};


class AndExprNode : public ExprNode
{
public:
    AndExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(&& ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class OrExprNode : public ExprNode
{
public:
    OrExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(|| ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class NotExprNode : public ExprNode
{
public:
    NotExprNode( ExprNode *rhs ) : rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(! ") + tostr(rhs_) + ')';
    }

protected:
    ExprNode *rhs_;
};


class EqExprNode : public ExprNode
{
public:
    EqExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(== ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';

    }

protected:
    ExprNode *lhs_, *rhs_;
};


class NeqExprNode : public ExprNode
{
public:
    NeqExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(!= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class LtExprNode : public ExprNode
{
public:
    LtExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(< ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class LteExprNode : public ExprNode
{
public:
    LteExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(<= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class GtExprNode : public ExprNode
{
public:
    GtExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(> ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class GteExprNode : public ExprNode
{
public:
    GteExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(>= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class MultiplyExprNode : public ExprNode
{
public:
    MultiplyExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(* ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};

class DivideExprNode : public ExprNode
{
public:
    DivideExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(/ ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class ModulusExprNode : public ExprNode
{
public:
    ModulusExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        return std::string("(% ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class PlusExprNode : public ExprNode
{
public:
    PlusExprNode( ExprNode *rhs ) : lhs_(nullptr), rhs_(rhs) {}
    PlusExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        std::string s = std::string("(+ ");
        if ( lhs_ != nullptr ) {
            s += tostr(lhs_) + ' ';
        }
        s += tostr(rhs_) + ')';
        return s;
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class MinusExprNode : public ExprNode
{
public:

    MinusExprNode( ExprNode *rhs ) : lhs_(nullptr), rhs_(rhs) {}
    MinusExprNode( ExprNode *lhs, ExprNode *rhs ) : lhs_(lhs), rhs_(rhs) {}

    virtual const std::string str( ) const override {
        std::string s = std::string("(- ");
        if ( lhs_ != nullptr ) {
            s += tostr(lhs_) + ' ';
        }
        s += tostr(rhs_) + ')';
        return s;
    }

protected:
    ExprNode *lhs_, *rhs_;
};


class VariableExprNode : public ExprNode {
public:
    explicit VariableExprNode( const std::string& id ) : id_(id) {}

    virtual const std::string str( ) const override {
        return std::string("(VAR ") + id_ + ')';
    }

protected:
    std::string id_;
};

/////////////////////////////////////////////////////////////////////////////////

class VariableDeclarationNode : public Node
{
public:
    VariableDeclarationNode( ValueType type, std::list<VariableExprNode*>* vars )
            : type_(type), vars_(vars) {}

    virtual const std::string str( ) const override {
        std::string s = std::string("(DECLARE ") + tostr(type_);
        for ( auto node : *vars_ ) {
            s +=  ' ' + tostr(node);
        }
        s += ')';
        return s;
    }

protected:
    ValueType type_;
    const std::list<VariableExprNode*>* vars_;
};


class ParameterNode : public Node {
public:
    ParameterNode( ValueType type, VariableExprNode* var )
            : type_(type), var_(var) {}

    virtual const std::string str( ) const override {
        return std::string("(PARAM ") + tostr(type_) + tostr(var_) + ")";
    }

protected:
    ValueType type_;
    VariableExprNode *var_;
};

/////////////////////////////////////////////////////////////////////////////////

class StmNode : public Node { };

class MethodCallExprStmNode: public ExprNode, public StmNode
{
public:

    MethodCallExprStmNode( std::string id, std::list<ExprNode*>* expr_list )
            : id_(id), expr_list_(expr_list) {
    }

    virtual const std::string str( ) const override {
        std::string s( "(CALL " + id_ );
        for ( auto e : *expr_list_ ) {
            s += " " + tostr(e);
        }
        s += ')';
        return s;
    }

protected:
    std::string id_;
    std::list<ExprNode*> *expr_list_;
};


class AssignStmNode: public StmNode
{
public:

    AssignStmNode( VariableExprNode *lvar, ExprNode *expr )
            : lvar_(lvar), expr_(expr) {
    }

    virtual const std::string str( ) const override {
        return std::string("(= ") + tostr(lvar_) + ' ' + tostr(expr_) + ')';
    }

protected:
    VariableExprNode *lvar_;
    ExprNode *expr_;
};


class IncrDecrStmNode : public StmNode {};


class IncrStmNode: public IncrDecrStmNode
{
public:

    IncrStmNode( VariableExprNode *var ) : var_(var) {}

    virtual const std::string str( ) const override {
        return std::string("(++ ") + tostr(var_) + ')';
    }

protected:
    VariableExprNode *var_;
};


class DecrStmNode: public IncrDecrStmNode
{
public:

    DecrStmNode( VariableExprNode *var ) : var_(var) {}

    virtual const std::string str( ) const override {
        return std::string("(-- ") + tostr(var_) + ')';
    }

protected:
    VariableExprNode *var_;
};


class ReturnStmNode: public StmNode
{
public:

    ReturnStmNode( )
            : expr_(nullptr) {
    }

    ReturnStmNode( ExprNode *expr )
            : expr_(expr) {
    }

    virtual const std::string str( ) const override {
        std::string s("(RET ");
        if ( expr_ != nullptr ) {
            s += tostr(expr_);
        }
        s += ')';
        return s;
    }

protected:
    ExprNode *expr_;
};


class BreakStmNode: public StmNode
{
public:

    BreakStmNode(  ) { }

    virtual const std::string str( ) const override {
        return std::string( "(BREAK)" );
    }
};


class ContinueStmNode: public StmNode
{
public:

    ContinueStmNode(  ) { }

    virtual const std::string str( ) const override {
        return std::string( "(CONTINUE)" );
    }
};


class BlockStmNode: public StmNode
{
public:

    BlockStmNode( std::list<StmNode*>* stms ) : stms_(stms) {}

    virtual const std::string str( ) const override {
        std::string s( "(BLOCK" );
        for ( auto stm : *stms_ ) {
            s += " " + tostr(stm);
        }
        s += ")";
        return s;
    };

protected:
    std::list<StmNode*>* stms_;
};


class IfStmNode: public StmNode
{
public:

    IfStmNode( ExprNode* expr, BlockStmNode* stm_if, BlockStmNode* stm_else )
            : expr_(expr), stm_if_(stm_if), stm_else_(stm_else) {}

    virtual const std::string str( ) const override {
        std::string s( "(IF " );
        s += tostr(expr_) + tostr(stm_if_);
        if ( stm_else_ != nullptr ) {
            s += tostr(stm_else_);
        }
        s += ')';
        return s;
    };

protected:
    ExprNode *expr_;
    BlockStmNode *stm_if_, *stm_else_;
};


class ForStmNode: public StmNode
{
public:

    ForStmNode( AssignStmNode* assign, ExprNode* expr, IncrDecrStmNode* inc_dec,
                BlockStmNode* stms_ )
            : assign_(assign), expr_(expr), inc_dec_(inc_dec), stms_(stms_) {}

    virtual const std::string str( ) const override {
        return "(FOR " + tostr(assign_) + tostr(expr_) + tostr(inc_dec_) + tostr(stms_) + ')';
    };

protected:
    AssignStmNode *assign_;
    ExprNode *expr_;
    IncrDecrStmNode *inc_dec_;
    BlockStmNode *stms_;
};

/////////////////////////////////////////////////////////////////////////////////

class MethodNode : public Node {
public:
    MethodNode( ValueType return_type,
                std::string id,
                std::list<ParameterNode*> *params,
                std::list<VariableDeclarationNode*> *vars_decl,
                std::list<StmNode*>* stms )
            : return_type_(return_type), id_(id), params_(params), vars_decl_(vars_decl), stms_(stms) {}

    virtual const std::string str( ) const override {
        std::string s = "(METHOD " + tostr(return_type_) + ' ' + id_ + ' ';
        for (auto p : *params_) {
            s += tostr( p );
        }
        for (auto vds : *vars_decl_) {
            s += tostr( vds );
        }
        for (auto stm : *stms_) {
            s += tostr( stm );
        }
        s += ')';
        return s;
    }

protected:
    ValueType return_type_;
    std::string id_;
    std::list<ParameterNode*> *params_;
    std::list<VariableDeclarationNode*> *vars_decl_;
    std::list<StmNode*> *stms_;
};


class ProgramNode : public Node {
public:
    ProgramNode( std::string id,
                std::list<VariableDeclarationNode*> *var_decls,
                std::list<MethodNode*> *method_decls )
            : id_(id), var_decls_(var_decls), method_decls_(method_decls) {}

    virtual const std::string str( ) const override {
        std::string s = "(CLASS " + id_;
        if ( var_decls_ != nullptr ) {
            for (auto v : *var_decls_) {
                s += " " + tostr( v );
            }
        }
        if ( method_decls_ != nullptr ) {
            for (auto m : *method_decls_) {
                s += " " + tostr( m );
            }
        }
        s += ')';
        return s;
    }

protected:
    std::string id_;
    std::list<VariableDeclarationNode*> *var_decls_;
    std::list<MethodNode*> *method_decls_;
};

#endif //DECAFPARSER_AST_H
