use parsec::prelude::*;
use super::Lex;

macro_rules! define_symbols {
  { $( ($symbol:literal, $name:ident) ),* $(,)? } => {
    $(
      #[derive(Clone, Eq, PartialEq, Debug)]
      pub struct $name;
      impl<'a> Lex<'a> for $name {
        fn parser() -> BoxedParser<'a, String, $name> {
          BoxedParser::new(|input: String| -> ParseResult<'a, String, $name> {
            let input = input.chars();
            input.clone()
              .next()
              .filter(|char| *char == $symbol).map(|_| {
                ResultData::new(input.clone().collect(), $name)
              })
              .ok_or_else(|| input.collect())
          })
        }
      }
    )*
  }
}

macro_rules! define_keywords {
  { $( ($k:literal, $name:ident) ),* $(,)? } => {
    $(
      #[derive(Clone, Eq, PartialEq, Debug)]
      pub struct $name;

      impl<'a> Lex<'a> for $name {
        fn parser() -> BoxedParser<'a, String, $name> {
          BoxedParser::new(|input: String| -> ParseResult<'a, String, $name> {
            let input = input.chars();
            let keyword_len = $k.len();
            let a: String = input.clone().take(keyword_len).collect();
            Some($name)
              .filter(|_| a == $k)
              .map(|k| {
                ResultData::new(
                  input.clone().skip(keyword_len).collect(),
                  k,
                )
              })
              .ok_or_else(|| input.collect())
          })
        }
      }
    )*
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Ident(pub String);
impl<'a> Lex<'a> for Ident {
  fn parser() -> BoxedParser<'a, String, Ident> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Ident> {
      let mut chars = input.chars();
      let mut result: Vec<char> = vec![];
      for char in chars.by_ref() {
        if char.is_alphabetic() {
          result.push(char);
        } else {
          break;
        }
      }
      Ok(ResultData::new(
        chars.collect(),
        Ident(result.into_iter().collect())
      ))
    })
  }
}

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct Integer(pub String);
impl<'a> Lex<'a> for Integer {
  fn parser() -> BoxedParser<'a, String, Integer> {
    BoxedParser::new(|input: String| -> ParseResult<'a, String, Integer> {
      let mut chars = input.chars();
      let mut result: Vec<char> = vec![];
      for char in chars.by_ref() {
        if char.is_numeric() {
          result.push(char);
        } else {
          break;
        }
      }
      Ok(ResultData::new(
         chars.collect(),
         Integer(result.into_iter().collect()),
      ))
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

define_symbols! {
  ('=', Eqs),
  ('<', LArrow),
  ('>', RArrow),
  ('+', Plus),
  ('-', Minus),
  ('*', Asterisk),
  ('/', Slash),
  ('\\', BackSlash),
  ('(', LParenthesis),
  (')', RParenthesis),
  ('{', LCBrace),
  ('}', RCBrace),
  ('[', LSBrace),
  (']', RSBrace),
  ('#', Hash),
  ('|', Or),
  ('&', And),
  ('.', Point),
  ('$', Dollar),
  (',', Colon),
  ('\'', SingleQuote),
  ('"', Quote),
  (';', SemiColon),
  ('!', Exclamation),
  ('~', Tilde),
  ('%', Percent),
}

define_keywords! {
  ("let", Let),
}
