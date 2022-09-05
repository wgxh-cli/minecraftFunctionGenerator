use std::marker::PhantomData;

pub mod utils;
pub mod prelude;

#[derive(Clone, Eq, PartialEq, Debug)]
pub struct ResultData<'a, I: 'a + Clone, O: 'a + Clone> {
  pub remain: I,
  pub output: O,
  _marker: PhantomData<&'a O>,
}

impl<'a, I: 'a + Clone, O: 'a + Clone> ResultData<'a, I, O> {
  pub fn new(remain: I, output: O) -> Self {
    ResultData {
      remain,
      output,
      _marker: PhantomData,
    }
  }

  pub fn map<NO: 'a + Clone>(&self, map_fn: impl Fn(O) -> NO) -> ResultData<'a, I, NO> {
    ResultData::new(
      self.remain.clone(),
      map_fn(self.output.clone()),
    )
  }
}

pub type ParseResult<'a, I, O> = Result<ResultData<'a, I, O>, String>;

pub trait Parser<'a, I: 'a + Clone, O: 'a + Clone> {
  fn parse(&self, input: I) -> ParseResult<'a, I, O>;
}

impl<'a, I: 'a + Clone, O: 'a + Clone, F> Parser<'a, I, O> for F
where F: Fn(I) -> ParseResult<'a, I, O>
{
  fn parse(&self, input: I) -> ParseResult<'a, I, O> {
    self(input)
  }
}
