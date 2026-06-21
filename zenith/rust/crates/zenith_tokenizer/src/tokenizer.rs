use zenith_tokentypes::{TokenKind, REGEXES, TOKEN_KINDS};
use zenith_token::Token;
use regex::Regex;
use zenith_core::{Position, Span};


struct RegexToken {
    pattern: regex::Regex,
    token_type: TokenKind,
}
impl RegexToken {
    pub fn new(pattern: &str, token_type: TokenKind) -> Self {
        RegexToken {
            pattern: Regex::new(pattern).unwrap(),
            token_type,
        }
    }
}

pub struct Tokenizer {
    pub tokens: Vec<Token>,
    pub position: Position,
    pub current_line: String,
    pub current_index: usize,
    regexes: Vec<RegexToken>,
}

impl Tokenizer {
    pub fn new() -> Self {
        let regexes: Vec<RegexToken> = REGEXES.iter().zip(TOKEN_KINDS.iter())
            .map(|(pattern, token_type)| RegexToken::new(pattern, *token_type))
            .collect();
        Tokenizer {
            tokens: Vec::new(),
            current_line: String::new(),
            position: Position::new(1, 0),
            current_index: 0,
            regexes,
        }
    }

    pub fn tokenize(&mut self, input: String) -> Vec<Token> {
        // Token location takes the current position, and the current line
        // Token spanned takes the starting index and the current index and the value of the token
        let lines: Vec<String> = input.lines().map(|line| line.to_string()).collect();
        self.current_line = lines[self.position.line - 1].clone();
        while self.current_index < input.len() {
            let mut matched = false;
            for regex_token in &self.regexes {
                if let Some(mat) = regex_token.pattern.find(&input[self.current_index..]) {
                    if mat.start() == 0 {
                        let token_value = mat.as_str().to_string();
                        let token_length = token_value.len();
                        let span = Span::new(self.current_index, self.current_index + token_length - 1);

                        let token_type = regex_token.token_type;
                        if token_type != TokenKind::Whitespace && token_type != TokenKind::Newline {
                            let token = Token::new(
                                token_type,
                                Some(span),
                                Some(self.position.clone()),
                                Some(token_value.clone()),
                                Some(self.current_line.clone())
                            );
                            self.tokens.push(token);
                        }

                        // Update position and index
                        self.position.advance_by(&token_value);

                        if token_type == TokenKind::Newline {
                            if self.position.line - 1 < lines.len() {
                                self.current_line = lines[self.position.line - 1].clone();
                            }
                        }
                        self.current_index += token_length;
                        matched = true;
                        break;
                    }
                }
            }

            if !matched {
                // If no regex matched, we can handle it as an error or skip the character
                self.current_index += 1; // Skip the character
                self.position.advance(input.chars().nth(self.current_index - 1).unwrap());
            }
        }
        self.tokens.push(
            Token::new(
                TokenKind::Eof,
                None,
                None,
                None,
                None
            )
        );
        self.tokens.clone()
    }


}