#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub enum NonTerminal {
    Expr,
    Term,
    Factor,
}
impl NonTerminal {
    pub fn to_index(&self) -> usize {
        match self {
            NonTerminal::Expr => 0,
            NonTerminal::Term => 1,
            NonTerminal::Factor => 2,
        }
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub enum Terminal {
    IntLiteral = 0,
    Identifier = 1,
    Slash = 2,
    Star = 3,
    Plus = 4,
    Minus = 5,
    LParen = 6,
    RParen = 7,
    Eof = 8,
}
impl Terminal {
    pub fn to_index(&self) -> usize {
        match self {
            Terminal::IntLiteral => 0,
            Terminal::Identifier => 1,
            Terminal::Slash => 2,
            Terminal::Star => 3,
            Terminal::Plus => 4,
            Terminal::Minus => 5,
            Terminal::LParen => 6,
            Terminal::RParen => 7,
            Terminal::Eof => 8, // Adjusted index for Eof
        }
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub enum Actions {
    Shift(usize),
    Reduce(usize),
    Accept,
    Error(u64), // Error code for error handling
}


pub const RULE_COUNT: usize = 6; // Number of grammar rules
pub const ACTION_TABLE_COLS: usize = 9; // Number of terminals
pub const ACTION_TABLE_ROWS: usize = 12; // Number of states
pub const GOTO_TABLE_COLS: usize = 3; // Number of non-terminals
pub const GOTO_TABLE_ROWS: usize = ACTION_TABLE_ROWS; // Number of states

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Rule {
    pub lhs: NonTerminal,
    pub rhs_len: usize,
}


pub const RULES: [Rule; RULE_COUNT] = [
    Rule { lhs: NonTerminal::Expr, rhs_len: 3 }, // Expr -> Expr +,- Term
    Rule { lhs: NonTerminal::Expr, rhs_len: 1 }, // Expr -> Term
    Rule { lhs: NonTerminal::Term, rhs_len: 3 }, // Term -> Term *,/ Factor
    Rule { lhs: NonTerminal::Term, rhs_len: 1 }, // Term -> Factor
    Rule { lhs: NonTerminal::Factor, rhs_len: 3 }, // Factor -> ( Expr )
    Rule { lhs: NonTerminal::Factor, rhs_len: 1 }, // Factor -> Number,Identifier
];
// The grammar for the expression parser is as follows:
// Start =:: expr
// expr =:: term | expr +,- term
// term =:: factor | term *,/ factor
// factor =:: integer_literal | identifier | ( expr )


/*

*/

pub const ACTION_TABLE: [[Actions; ACTION_TABLE_COLS]; ACTION_TABLE_ROWS] = [
    // State 0
    [
        Actions::Shift(1), // IntLiteral
        Actions::Shift(1), // Identifier
        Actions::Error(1), // Slash
        Actions::Error(2), // Star
        Actions::Error(3), // Plus
        Actions::Error(4), // Minus
        Actions::Error(7), // LParen FORGET ABOUT THIS
        Actions::Error(5), // RParen
        Actions::Error(6), // Eof
    ],
    // State 1 (If expr)
    [
        Actions::Error(7), // IntLiteral
        Actions::Error(8), // Identifier
        Actions::Error(7), // Slash
        Actions::Error(6), // Star
        Actions::Shift(2), // Plus
        Actions::Shift(2), // Minus
        Actions::Error(9), // LParen
        Actions::Shift(5), // RParen
        Actions::Accept,   // Eof
    ],
    // State 2
    [
        Actions::Error(11), // IntLiteral
        Actions::Error(12), // Identifier
        Actions::Reduce(4),  // Slash
        Actions::Reduce(4),  // Star
        Actions::Shift(1), // Plus
        Actions::Shift(1), // Minus
        Actions::Error(13), // LParen
        Actions::Reduce(4), // RParen
        Actions::Reduce(4),  // Eof
    ],
    // State 3
    [
        Actions::Error(15), // IntLiteral
        Actions::Error(16), // Identifier
        Actions::Shift(4),  // Slash
        Actions::Shift(4),  // Star
        Actions::Reduce(2), // Plus
        Actions::Reduce(2), // Minus
        Actions::Error(17), // LParen
        Actions::Reduce(5), // RParen
        Actions::Reduce(2),    // Eof
    ],
    // State 4
    [
        Actions::Shift(5), // IntLiteral
        Actions::Error(20), // Identifier
        Actions::Error(585),  // Slash
        Actions::Shift(5),  // Star
        Actions::Reduce(2), // Plus
        Actions::Reduce(2), // Minus
        Actions::Error(21), // LParen
        Actions::Reduce(2), // RParen
        Actions::Reduce(2),    // Eof
    ],
    // State 5
    [
        Actions::Shift(6), // IntLiteral
        Actions::Shift(7), // Identifier
        Actions::Error(23), // Slash
        Actions::Error(24), // Star
        Actions::Error(25), // Plus
        Actions::Error(26), // Minus
        Actions::Shift(6),  // LParen FIX THIS LATER
        Actions::Error(27), // RParen
        Actions::Error(28), // Eof
    ],
    // State 6
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
    // State 7
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
    // State 8
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
    // State 9
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
    // State 10
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
    // State 11
    [
        Actions::Shift(5), // IntLiteral
        Actions::Shift(6), // Identifier
        Actions::Error(29), // Slash
        Actions::Error(30), // Star
        Actions::Error(31), // Plus
        Actions::Error(32), // Minus
        Actions::Shift(7),  // LParen
        Actions::Error(33), // RParen
        Actions::Error(34), // Eof
    ],
];


pub const GOTO_TABLE: [[Option<usize>; GOTO_TABLE_COLS]; GOTO_TABLE_ROWS] = [
    // State 0
    [Some(1), Some(2), Some(3)], // Expr, Term, Factor
    // State 1
    [None, None, None], // Expr, Term, Factor
    // State 2
    [None, None, None], // Expr, Term, Factor
    // State 3
    [None, None, None], // Expr, Term, Factor
    // State 4
    [None, None, None], // Expr, Term, Factor
    // State 5
    [None, None, None], // Expr, Term, Factor
    // State 6
    [None, None, None], // Expr, Term, Factor
    // State 7
    [None, None, None], // Expr, Term, Factor
    // State 8
    [None, None, None], // Expr, Term, Factor
    // State 9
    [None, None, None], // Expr, Term, Factor
    // State 10
    [None, None, None], // Expr, Term, Factor
    // State 11
    [None, None, None], // Expr, Term, Factor
];


