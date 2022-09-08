use parsec::prelude::*;
use crate::lexer::{
  Lex,
  str_utils::*,
};

macro_rules! define_symbols {
  { $( ($symbol:literal, $name:ident) ),* $(,)? } => {
    $(
      #[derive(Clone, Eq, PartialEq, Debug)]
      pub struct $name;
      impl<'a> Lex<'a> for $name {
        fn parser() -> BoxedParser<'a, String, $name> {
          BoxedParser::new(|input: String| -> ParseResult<'a, String, $name> {
            skip_white_space_and(filter(next(), |result| result.output == $symbol))
              .map(|_| $name)
              .parse(input)
          })
        }
      }
    )*
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
  (',', Comma),
  ('@', At)
}
