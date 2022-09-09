use super::prelude::*;
use parsec::prelude::*;
use crate::token::prelude::*;
use super::{
  literal::Literal,
  ident::Ident,
};

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Expr {
  Literal(Literal),
  Ident(Ident),
  Delimited(Box<Delimited>),
}
impl<'a> Lex<'a> for Expr {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      Literal::parser().map(Expr::Literal)
        .or(Ident::parser().map(Expr::Ident))
        .or(Delimited::parser().map(|delimited| Expr::Delimited(Box::new(delimited))))
        .parse(input)
    })
  }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Delimited {
  pub l_paren: LParenthesis,
  pub expr: Expr,
  pub r_paren: RParenthesis,
}
impl<'a> Lex<'a> for Delimited {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut ast_builder = AstBuilder::new(input);

      Ok(Delimited {
        l_paren: ast_builder.build()?,
        expr: ast_builder.build()?,
        r_paren: ast_builder.build()?,
      })
      .map(|delimited| {
        ResultData::new(ast_builder.finish(), delimited)
      })
    })
  }
}
