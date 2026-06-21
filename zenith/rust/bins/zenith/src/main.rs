use zenith_token::Token;
use zenith_tokenizer::z_tokenizer::Tokenizer;

fn main() {
    let source: String = "123 + 123".to_string();
    let mut tokenizer: Tokenizer = Tokenizer::new();
    let tokens: Vec<Token> = tokenizer.tokenize(source);
    for token in tokens {
        println!("{:?}", token);
    }
}