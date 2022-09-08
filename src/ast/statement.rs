use std::fmt::Debug;
use super::prelude::*;
use parsec::prelude::*;
use crate::token::prelude::*;
use crate::lexer::Lex;
use super::{
  expr::Expr,
  ident::Ident,
  mc_command::McCommand,
};

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct InStmt<T> {
  pub source: T,
  pub ending: SemiColon,
}
impl<'a, T: 'a + Clone + Lex<'a> + Debug> Lex<'a> for InStmt<T> {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut ast_builder = AstBuilder::new(input);

      Ok(InStmt {
        source: ast_builder.build()?,
        ending: ast_builder.build()?,
      })
      .map(|in_stmt| {
        ResultData::new(ast_builder.finish(), in_stmt)
      })
    })
  }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Statement {
  LetBinding(InStmt<VariableBinding>),
  McCommand(InStmt<McCommand>),
}
impl<'a> Lex<'a> for Statement {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      InStmt::<VariableBinding>::parser().map(Self::LetBinding)
        .or(InStmt::<McCommand>::parser().map(Self::McCommand))
        .parse(input)
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct VariableBinding {
  pub let_keyword: LetKeyword,
  pub name: Ident,
  pub eqs: Eqs,
  pub value: Expr,
}
impl<'a> Lex<'a> for VariableBinding {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut ast_builder = AstBuilder::new(input);
      Ok(VariableBinding {
        let_keyword: ast_builder.build()?,
        name: ast_builder.build()?,
        eqs: ast_builder.build()?,
        value: ast_builder.build()?,
      })
      .map(|variable_binding| {
        ResultData::new(ast_builder.source, variable_binding)
      })
    })
  }
}

