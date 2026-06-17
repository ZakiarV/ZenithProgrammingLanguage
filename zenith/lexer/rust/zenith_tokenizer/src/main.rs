mod argument_parser;
mod parse_tdf_file;

fn main() {
    let args = argument_parser::Arguments::parse();
    println!("Input file: {}", args.input_file);
    if let Some(output) = args.output_file {
        println!("Output file: {}", output);
    } else {
        println!("No output file specified.");
    }

    println!("Version: {}", args.version.as_str());
    let token_type_list = parse_tdf_file::parse_tdf_file(args.version).unwrap();
    println!("Parsed token types: {:?}", token_type_list.token_types);
}
