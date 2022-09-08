use std::fs::read;
use std::path::Path;
use mfg::lexer::str_utils::*;
use parsec::prelude::*;
use mfg::lexer::Lex;
use mfg::ast::statement::Statement;

fn main() {
  let source_code = String::from_utf8_lossy(
    &read(Path::new("./test.mfg"))
    .unwrap())
    .to_string()
    .trim()
    .to_string();
  let result = all(Statement::parser()).parse(source_code).unwrap();
  dbg!(result);
}
