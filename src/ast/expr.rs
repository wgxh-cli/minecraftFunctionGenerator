use super::prelude::*;
use parsec::prelude::*;
use super::{
  literal::Literal,
  ident::Ident,
};

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Expr {
  Literal(Literal),
  Ident(Ident),
}
impl<'a> Lex<'a> for Expr {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      Literal::parser().map(Expr::Literal)
        .or(Ident::parser().map(Expr::Ident))
        .parse(input)
    })
  }
}
