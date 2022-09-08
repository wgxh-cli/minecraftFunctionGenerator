use crate::lexer::str_utils::*;
use parsec::prelude::*;
use super::prelude::*;
use crate::token::prelude::*;

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Integer(pub String);
impl<'a> Lex<'a> for Integer {
  fn parser() -> BoxedParser<'a, String, Integer> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Integer> {
      filter(some(skip_white_space_and(Num::parser()))
        .map(|nums| {
          nums.into_iter()
            .map(|Num(char)| char)
            .collect::<String>()
        }), |result| !result.output.is_empty())
        .map(Integer)
        .parse(input)
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Float {
  pub point: Point,
  pub numbers: Integer,
}
impl<'a> Lex<'a> for Float {
  fn parser() -> BoxedParser<'a, String, Float> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Float> {
      and(Point::parser(), Integer::parser()).parse(input).map(|result| {
        result.map(|(point, numbers)| {
          Float {
            point,
            numbers,
          }
        })
      })
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Number {
  pub integer: Integer,
  pub float: Float,
}
impl<'a> Lex<'a> for Number {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Number> {
      and(Integer::parser(), or_default(Float::parser(), Float {
        point: Point,
        numbers: Integer("0".to_string()),
      })).parse(input).map(|result| {
        result.map(|(integer, float)| {
          Number {
            integer,
            float,
          }
        })
      })
    })
  }
}

