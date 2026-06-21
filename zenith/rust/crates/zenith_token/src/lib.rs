use zenith_core::{Position, Span};
use zenith_tokentypes::TokenKind;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Token {
    pub kind: TokenKind,
    pub value: Option<String>,
    pub span: Option<Span>,
    pub line_content: Option<String>,
    pub location: Option<Position>,
}
impl Token {
    pub fn new(kind: TokenKind, span: Option<Span>, location: Option<Position>, value: Option<String>, line_content: Option<String>) -> Self {
        Self {
            kind,
            value,
            span,
            line_content,
            location,
        }
    }
    pub fn copy(&self) -> Self {
        Self {
            kind: self.kind,
            value: self.value.clone(),
            span: self.span,
            line_content: self.line_content.clone(),
            location: self.location,
        }
    }
}
