use parsec::prelude::*;
use super::prelude::*;
use super::ident::Ident;
use crate::token::prelude::*;
use crate::lexer::str_utils::*;

#[derive(Clone, Eq, PartialEq, Debug)]
pub enum Command {
  Raw(String),
  Interpolation(Ident),
}
impl<'a> Lex<'a> for Command {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      let char = || filter(next(), |result| result.output != '$' || result.output.is_alphanumeric());

      let raw = || filter(some(char())
        .map(|chars| chars.into_iter().collect::<String>()),
        |result| !result.output.is_empty())
        .map(Command::Raw);

      let interpolation = || satisfy('$')
        .and(Ident::parser())
        .and(satisfy('$'))
        .map(|((_, ident), _)| Command::Interpolation(ident));

      dbg!(raw()
        .or(interpolation())
        .parse(input))
    })
  }
}
impl<'a> Lex<'a> for Vec<Command> {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      some(Command::parser()).parse(input)
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct McCommand {
  pub starter: At,
  pub commands: Vec<Command>,
}
impl<'a> Lex<'a> for McCommand {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut ast_builder = AstBuilder::new(input);
      Ok(McCommand {
        starter: ast_builder.build()?,
        commands: ast_builder.build()?,
      }).map(|mc_command| {
        ResultData::new(ast_builder.source, mc_command)
      })
    })
  }
}
