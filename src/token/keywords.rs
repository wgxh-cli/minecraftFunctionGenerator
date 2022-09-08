use parsec::prelude::*;
use super::white_spaces::WhiteSpace;
use crate::lexer::{
  Lex,
  str_utils::*,
};

macro_rules! define_keywords {
  { $( ($k:literal, $name:ident) ),* $(,)? } => {
    $(
      #[derive(Clone, Eq, PartialEq, Debug)]
      pub struct $name;

      impl<'a> Lex<'a> for $name {
        fn parser() -> BoxedParser<'a, String, $name> {
          BoxedParser::new(|input: String| -> ParseResult<'a, String, $name> {
            let next_char = filter(next(), |result| {
              WhiteSpace::parser()
                .parse(result.output.to_string())
                .is_err()
            });
            skip_white_space_and(and(
              filter(
                some(next_char)
                  .map(|chars| chars.into_iter().collect::<String>()),
                |result| result.output == *$k),
              next()))
              .map(|_| $name)
              .parse(input)
          })
        }
      }
    )*
  }
}

define_keywords! {
  ("let", LetKeyword),
  ("fn", FnKeyword),
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Num(pub char);
impl<'a> Lex<'a> for Num {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      skip_white_space_and(filter(next(), |result| result.output.is_numeric()))
        .map(Num)
        .parse(input)
    })
  }
}
