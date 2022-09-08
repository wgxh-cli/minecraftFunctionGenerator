pub mod statement;
pub mod expr;
pub mod literal;
pub mod ident;
pub mod number;
pub mod punctuated;
pub mod mc_command;
pub mod prelude;

use prelude::*;
use parsec::*;

pub struct AstBuilder {
  pub source: String,
}

impl AstBuilder {
  pub fn new(source: String) -> Self {
    AstBuilder {
      source,
    }
  }
  pub fn build<'a, T: Lex<'a>>(&mut self) -> Result<T, String> {
    T::parser().parse(self.source.clone()).map(|a| {
      self.source = a.remain.clone();

      a.output
    })
  }

  pub fn finish(&self) -> String {
    self.source.clone()
  }
}
