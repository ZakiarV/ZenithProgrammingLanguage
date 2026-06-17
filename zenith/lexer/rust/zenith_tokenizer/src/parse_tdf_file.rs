use std::fs;
use std::io;
use std::path::Path;
use crate::argument_parser::Version;
use regex::Regex;

#[derive(Debug)]
pub struct TokenType {
    pub name: String,
    pub pattern: Option<Regex>,
}

pub struct TokenTypeList {
    pub token_types: Vec<TokenType>,
}

pub fn read_file_to_string<P: AsRef<Path>>(path: P) -> io::Result<String> {
    fs::read_to_string(path)
}

pub fn parse_tdf_line(token_type_list: &mut TokenTypeList, line: String) {
    let parts: Vec<&str> = line.split(' ').collect();
    if parts.len() != 3 && parts.len() != 4 {
        panic!("Invalid TDF line format: {}", line);
    }

    if parts[0] == "TOKEN_TYPE" && parts.len() == 3 {
        let name = parts[1].to_string();
        if parts[2] != ";" {
            panic!("Expected ';' at the end of TOKEN_TYPE line: {}", line);
        }
        token_type_list.token_types.push(TokenType { name, pattern: None });
    } else if parts[0] == "DEFINE" && parts.len() == 4 {
        let name = parts[1].to_string();
        for token_type in &mut token_type_list.token_types {
            if token_type.name == name {
                let pattern_str = parts[2];
                let pattern = Regex::new(pattern_str).expect("Invalid regex pattern");
                token_type.pattern = Some(pattern);
                if parts[3] != ";" {
                    panic!("Expected ';' at the end of DEFINE line: {}", line);
                }
                return;
            }
        }
        panic!("Token type '{}' not defined before DEFINE: {}", name, line);
    }

}

pub fn parse_tdf_file(version: Version) -> io::Result<TokenTypeList> {
    let file_content: Option<String> = match version {
        Version::V0_0_1ALPHA => {
            let version_str = version.as_str();
            let file_path = format!("zenith/lexer/tokenization_files/tdf_{}.tdf", version_str);
            let file_path_ref = Path::new(&file_path);
            Some(read_file_to_string(file_path_ref)?)
        }
    };
    
    let line_list: Vec<String> = file_content.expect("Failed to read file")
        .lines()
        .map(|line| line.trim().to_string())
        .filter(|line| !line.is_empty() && !line.starts_with("//"))
        .collect();

    let mut token_type_list = TokenTypeList { token_types: Vec::new() };
    for line in line_list {
        parse_tdf_line(&mut token_type_list, line);
    }

    Ok(token_type_list)
}