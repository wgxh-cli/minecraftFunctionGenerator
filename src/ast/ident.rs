use parsec::prelude::*;
use crate::lexer::str_utils::*;
use super::prelude::*;

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct ValidIdentChar(pub char);
impl<'a> Lex<'a> for ValidIdentChar {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      filter(next(), |result| {
        result.output.is_alphanumeric() ||
        result.output == '_'
      })
      .map(ValidIdentChar)
      .parse(input)
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Ident(pub String);
impl<'a> Lex<'a> for Ident {
  fn parser() -> BoxedParser<'a, String, Ident> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Ident> {
      filter(skip_white_space_and(some(ValidIdentChar::parser())
        .map(|chars| {
          chars.into_iter()
            .map(|ValidIdentChar(char)| char)
            .collect::<String>()
        })),
        |result| !result.output.is_empty())
        .map(Ident)
        .parse(input)
    })
  }
}
