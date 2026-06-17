use std::env::{args};


pub enum Version {
    V0_0_1ALPHA,
}

impl Version {
    pub fn as_str(&self) -> &'static str {
        match self {
            Version::V0_0_1ALPHA => "0.0.1-alpha",
        }
    }
    
}


pub struct Arguments {
    pub input_file: String,
    pub output_file: Option<String>,
    pub version: Version,
}

impl Arguments {
    pub fn parse() -> Self {
        let mut input_file_index: Option<usize> = None;
        let mut output_file_index: Option<usize> = None;
        let mut version_index: Option<usize> = None;

        let args_vec: Vec<String> = args().collect();

        for (index, arg) in args_vec.iter().enumerate() {
            if arg == "-o" || arg == "--output" {
                output_file_index = Some(index + 1);
            } else if arg == "-i" || arg == "--input" {
                input_file_index = Some(index + 1);
            } else if arg == "-v" || arg == "--version" {
                version_index = Some(index + 1);
            }
        }

        let input_idx = input_file_index.expect("No input file provided");
        let input_file = args_vec.get(input_idx).cloned().expect("Invalid input file index");

        let output_file = match output_file_index {
            Some(idx) => Some(args_vec.get(idx).cloned().expect("Expected output file after -o/--output")),
            None => None,
        };

        let version = match version_index {
            Some(idx) => match args_vec.get(idx).cloned().expect("Expected version after -v/--version").as_str() {
                "0.0.1-alpha" => Version::V0_0_1ALPHA,
                _ => panic!("Unsupported version"),
            },
            None => Version::V0_0_1ALPHA, // Default version if not specified
        };

        Arguments { input_file, output_file, version }
    }
}
