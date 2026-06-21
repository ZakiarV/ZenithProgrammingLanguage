#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub enum TokenKind {
    IntLiteral = 0,
    Identifier = 1,
    Slash = 2,
    Star = 3,
    Plus = 4,
    Minus = 5,
    LParen = 6,
    RParen = 7,
    Newline = 8,
    Whitespace = 9,
    Unknown = 10,
    Eof = 11,
}
impl TokenKind {
    pub fn kind_name(&self) -> &'static str {
        match self {
            TokenKind::IntLiteral => "IntLiteral",
            TokenKind::Identifier => "Identifier",
            TokenKind::Slash => "Slash",
            TokenKind::Star => "Star",
            TokenKind::Plus => "Plus",
            TokenKind::Minus => "Minus",
            TokenKind::LParen => "LParen",
            TokenKind::RParen => "RParen",
            TokenKind::Newline => "Newline",
            TokenKind::Whitespace => "Whitespace",
            TokenKind::Unknown => "Unknown",
            TokenKind::Eof => "Eof",
        }
    }
}

pub const REGEXES: [&str;10] = [
    r"\r?\n", 
    r"\s+", 
    r"\d+", 
    r"[a-zA-Z_][a-zA-Z0-9_]*", 
    r"/", 
    r"\*", 
    r"\+", 
    r"-", 
    r"\(", 
    r"\)"
];
pub const TOKEN_KINDS: [TokenKind;10] = [
    TokenKind::Newline, 
    TokenKind::Whitespace, 
    TokenKind::IntLiteral, 
    TokenKind::Identifier, 
    TokenKind::Slash, 
    TokenKind::Star, 
    TokenKind::Plus, 
    TokenKind::Minus, 
    TokenKind::LParen, 
    TokenKind::RParen
];
