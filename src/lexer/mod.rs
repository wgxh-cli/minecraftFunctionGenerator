pub mod str_utils;

use parsec::prelude::*;

pub trait Lex<'a>
where Self: 'a + Clone + Sized
{
  fn parser() -> BoxedParser<'a, String, Self>;
}

