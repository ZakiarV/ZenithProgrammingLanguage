use zenith_token::Token;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum FactorNode {
    IntLiteral(Token),
    Identifier(Token),
    ParenExpr(Token, Box<ExprNode>, Token),
}
impl FactorNode {
    pub fn new_int_literal(token: Token) -> Self {
        FactorNode::IntLiteral(token)
    }
    pub fn new_identifier(token: Token) -> Self {
        FactorNode::Identifier(token)
    }
    pub fn new_paren_expr(left_paren: Token, expr: ExprNode, right_paren: Token) -> Self {
        FactorNode::ParenExpr(left_paren, Box::new(expr), right_paren)
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TermNode {
    Factor(FactorNode),
    TermBinOp(Box<TermNode>, Token, Box<FactorNode>),
}
impl TermNode {
    pub fn new_factor(factor: FactorNode) -> Self {
        TermNode::Factor(factor)
    }
    pub fn new_term_bin_op(left: TermNode, op: Token, right: FactorNode) -> Self {
        TermNode::TermBinOp(Box::new(left), op, Box::new(right))
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ExprNode {
    Term(TermNode),
    ExprBinOp(Box<ExprNode>, Token, Box<TermNode>),
}
impl ExprNode {
    pub fn new_term(term: TermNode) -> Self {
        ExprNode::Term(term)
    }
    pub fn new_expr_bin_op(left: ExprNode, op: Token, right: TermNode) -> Self {
        ExprNode::ExprBinOp(Box::new(left), op, Box::new(right))
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ASTNode {
    Factor(FactorNode),
    Term(TermNode),
    Expr(ExprNode),
}
impl ASTNode {
    pub fn new_factor(factor: FactorNode) -> Self {
        ASTNode::Factor(factor)
    }
    pub fn new_term(term: TermNode) -> Self {
        ASTNode::Term(term)
    }
    pub fn new_expr(expr: ExprNode) -> Self {
        ASTNode::Expr(expr)
    }

    pub fn to_index(&self) -> usize {
        match self {
            ASTNode::Factor(_) => 0,
            ASTNode::Term(_) => 1,
            ASTNode::Expr(_) => 2,
        }
    }
}
