pub mod token;

use parsec::prelude::*;
use token::*;

pub trait Lex<'a>
where Self: 'a + Clone + Sized
{
  fn parser() -> BoxedParser<'a, String, Self>;
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct VariableBinding {
  pub let_keyword: Let,
  pub name: Ident,
  pub eqs: Eqs,
  pub value: Number,
}

pub struct Lexer {
  source: String,
}
impl Lexer {
  pub fn build<'a, T: Lex<'a>>(&mut self) -> Result<T, String> {
    T::parser().parse(self.source.clone()).map(|ResultData { remain, output, .. }| {
      self.source = remain;

      output
    })
  }

  pub fn new(source: String) -> Self {
    Lexer { source }
  }
}

impl<'a> Lex<'a> for VariableBinding {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Self> {
      let mut lexer = Lexer::new(input);
      Ok(VariableBinding {
        let_keyword: lexer.build()?,
        name: lexer.build()?,
        eqs: lexer.build()?,
        value: lexer.build()?,
      })
      .map(|variable_binding| {
        ResultData::new(lexer.source, variable_binding)
      })
    })
  }
}

#[test]
fn test() {
  let test_suit = "let a = 10";
  assert_eq!(
    VariableBinding::parser()
      .parse(
        test_suit
        .chars()
        .filter(|c| *c != ' ').collect()),
    Ok(ResultData::new(
      "".to_string(),
      VariableBinding {
        let_keyword: Let,
        name: Ident("a".to_string()),
        eqs: Eqs,
        value: Number {
          integer: Integer("10".to_string()),
          float: Float {
            point: Point,
            numbers: Integer("0".to_string())
          }
        }
      })));
}
