use std::fmt::Debug;
use parsec::prelude::*;
use crate::ast::prelude::*;

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Pair<A, B> {
  Punctuated(A, B),
  End(A),
}
impl<'a, A: 'a + Clone + Lex<'a> + Debug, B: 'a + Clone + Lex<'a> + Debug> Lex<'a> for Pair<A, B> {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      and(A::parser(), to_option(B::parser())).map(|(a, b)| {
        match b {
          Some(b) => Pair::Punctuated(a, b),
          None => Pair::End(a),
        }
      })
      .parse(input)
    })
  }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Punctuated<T, P> {
  pub elements: Vec<Pair<T, P>>,
}
impl<T, P> Punctuated<T, P> {
  pub fn new(elements: Vec<Pair<T, P>>) -> Self {
    Punctuated { elements }
  }
}
impl<'a, T: 'a + Clone + Lex<'a> + Debug, P: 'a + Clone + Lex<'a> + Debug> Lex<'a> for Punctuated<T, P> {
  fn parser() -> BoxedParser<'a, String, Self> {
    BoxedParser::new(move |input: String| -> ParseResult<'a, String, Self> {
      some(Pair::<T, P>::parser()).map(|elements| {
        Punctuated::new(elements)
      }).parse(input)
    })
  }
}
