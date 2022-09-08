use std::fmt::Debug;
use super::prelude::*;
use crate::lexer::str_utils::*;
use parsec::prelude::*;
use crate::token::prelude::*;
use super::{
  number::Number,
  expr::Expr,
};

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Literal {
  Number(Number),
  String(Str),
  Arrary(Arrary),
}
impl<'a> Lex<'a> for Literal {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      Number::parser().map(Literal::Number)
        .or(Str::parser().map(Literal::String))
        .or(Arrary::parser().map(Literal::Arrary))
        .parse(input)
    })
  }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Str {
  pub l_quote: Quote,
  pub content: String,
  pub r_quote: Quote,
}
impl<'a> Lex<'a> for Str {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let quote = || satisfy('"');
      let next_quote = || filter(next(), |result| result.output != '"');

      skip_white_space_and(quote().and(some(next_quote()))
        .and(quote()))
        .map(|((_, chars), _) | {
          Str {
            l_quote: Quote,
            content: chars.into_iter().collect(),
            r_quote: Quote,
          }
        })
        .parse(input)
    })
  }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Arrary {
  pub l_bracket: LSBrace,
  pub elements: punctuated::Punctuated<Expr, Comma>,
  pub r_bracket: RSBrace,
}
impl<'a> Lex<'a> for Arrary {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut ast_builder = AstBuilder::new(dbg!(input));
      Ok(Arrary {
        l_bracket: dbg!(ast_builder.build()?),
        elements: ast_builder.build()?,
        r_bracket: dbg!(ast_builder.build()?),
      })
      .map(|arrary| {
        ResultData::new(ast_builder.source, arrary)
      })
    })
  }
}
