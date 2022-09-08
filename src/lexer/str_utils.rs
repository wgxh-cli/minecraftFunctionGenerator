use parsec::prelude::*;
use crate::lexer::Lex;
use crate::token::prelude::*;

pub fn next<'a>() -> impl Parser<'a, String, char> {
  |input: String| -> ParseResult<'a, String, char> {
    let mut chars = input.chars();

    chars.next()
      .map(|char| {
        ResultData::new(chars.collect(), char)
      })
      .ok_or_else(|| "Empty arguments give".to_string())
  }
}

pub fn peek_next<'a>() -> impl Parser<'a, String, char> {
  |input: String| -> ParseResult<'a, String, char> {
    let chars = input.chars();
    chars.peekable()
      .next()
      .map(|char| {
        ResultData::new(input, char)
      })
      .ok_or_else(|| "Empty string found".to_string())
  }
}

pub fn satisfy<'a>(target: char) -> impl Parser<'a, String, char> {
  filter(
    next(),
    move |result| result.output == target)
}

pub fn skip_white_space_and<'a, O: 'a + Clone>(
  parser: impl Parser<'a, String, O> + 'a
) -> impl Parser<'a, String, O> {
  and(some(WhiteSpace::parser()), parser)
    .map(|(_, output)| output)
}

pub fn all<'a, O: 'a + Clone>(parser: impl Parser<'a, String, O> + 'a) -> impl Parser<'a, String, Vec<O>> {
  filter(some(parser), |result| result.remain.is_empty())
}
