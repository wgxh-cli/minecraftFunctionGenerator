use parsec::prelude::*;
use crate::lexer::{
  Lex,
  str_utils::*,
};

macro_rules! define_white_spaces {
  ( $( ($white_space:literal, $name:ident) ),*, $(,)? ) => {
    $(
      #[derive(Clone, Eq, PartialEq, Debug)]
      pub struct $name;
      impl<'a> Lex<'a> for $name {
        fn parser() -> BoxedParser<'a, String, $name> {
          BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
            filter(next(), |result| result.output == $white_space)
              .map(|_| $name)
              .parse(input)
          })
        }
      }
    )*
  }
}

define_white_spaces! {
  (' ', Space),
  ('\n', Enter),
  ('\t', Tab),
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct WhiteSpace;
impl<'a> Lex<'a> for WhiteSpace {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      Space::parser().map(|_| ())
        .or(Tab::parser().map(|_| ()))
        .or(Enter::parser().map(|_| ()))
        .map(|_| WhiteSpace)
        .parse(input)
    })
  }
}
