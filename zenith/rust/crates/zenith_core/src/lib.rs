#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Span {
    pub start: usize,
    pub end: usize,
}
impl Span {
    pub fn new(start: usize, end: usize) -> Self {
        Self { start, end }
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Position {
    pub line: usize,
    pub column: usize,
}
impl Position {
    pub fn new(line: usize, column: usize) -> Self {
        Self { line, column }
    }
    pub fn advance(&mut self, c: char) {
        if c == '\n' {
            self.line += 1;
            self.column = 0;
        } else if c == '\t' {
            self.column += 4; // Assuming a tab width of 4 spaces
        } else {
            self.column += 1;
        }
    }
    pub fn advance_by(&mut self, s: &str) {
        for c in s.chars() {
            self.advance(c);
        }
    }
}