use std::marker::PhantomData;
use crate::prelude::*;

pub struct BoxedParser<'a, I: 'a + Clone, O: 'a + Clone> {
  parser: Box<dyn Parser<'a, I, O> + 'a>,
}

impl<'a, I: 'a + Clone, O: 'a + Clone> Parser<'a, I, O> for BoxedParser<'a, I, O> {
  fn parse(&self, input: I) -> ParseResult<'a, I, O> {
    self.parser.parse(input)
  }
}
impl<'a, I: 'a + Clone, O: 'a + Clone> Parser<'a, I, O> for &BoxedParser<'a, I, O> {
  fn parse(&self, input: I) -> ParseResult<'a, I, O> {
    self.parser.parse(input)
  }
}
impl<'a, I: 'a + Clone, O: 'a + Clone> BoxedParser<'a, I, O> {
  pub fn new(parser: impl Parser<'a, I, O> + 'a) -> Self {
    BoxedParser {
      parser: Box::new(parser),
    }
  }
}

pub struct Map<'a, I: 'a + Clone, A: 'a + Clone, B: 'a + Clone> {
  parser: BoxedParser<'a, I, A>,
  map_fn: Box<dyn Fn(A) -> B + 'a>,
}
impl<'a, I: 'a + Clone, A: 'a + Clone, B: 'a + Clone> Parser<'a, I, B> for Map<'a, I, A, B> {
  fn parse(&self, input: I) -> ParseResult<'a, I, B> {
    self.parser.parse(input).map(|r| r.map(|a| (self.map_fn)(a)))
  }
}

pub fn map<'a, I: 'a + Clone, A: 'a + Clone, B: 'a + Clone>(
  parser: impl Parser<'a, I, A> + 'a,
  map_fn: impl Fn(A) -> B + 'a
) -> Map<'a, I, A, B> {
  Map {
    parser: BoxedParser::new(parser),
    map_fn: Box::new(map_fn),
  }
}

pub struct OrDefault<'a, I: 'a + Clone, O: 'a + Clone> {
  parser: Box<dyn Parser<'a, I, O> + 'a>,
  default: O,
}

impl<'a, I: 'a + Clone, O: 'a + Clone> Parser<'a, I, O> for OrDefault<'a, I, O> {
  fn parse(&self, input: I) -> ParseResult<'a, I, O> {
    self.parser
      .parse(input.clone())
      .or_else(|_| Ok(ResultData::new(input, self.default.clone())))
  }
}

pub fn or_default<'a, I, O>(
  parser: impl Parser<'a, I, O> + 'a,
  default: O
) -> OrDefault<'a, I, O>
where
  I: 'a + Clone,
  O: 'a + Clone
{
  OrDefault {
    parser: Box::new(parser),
    default,
  }
}

pub struct And<'a, I: 'a, A: 'a, B: 'a> {
  parser_a: Box<dyn Parser<'a, I, A> + 'a>,
  parser_b: Box<dyn Parser<'a, I, B> + 'a>,
}

impl<'a, I: 'a + Clone, A: 'a + Clone, B: 'a + Clone> Parser<'a, I, (A, B)> for And<'a, I, A, B> {
  fn parse(&self, input: I) -> ParseResult<'a, I, (A, B)> {
    self.parser_a.parse(input).and_then(|a| {
      self.parser_b.parse(a.remain).map(|b| {
        ResultData::new(
          b.remain,
          (a.output, b.output)
        )
      })
    })
  }
}

pub fn and<'a, I: Clone, A: Clone, B: Clone>(
  parser_a: impl Parser<'a, I, A> + 'a,
  parser_b: impl Parser<'a, I, B> + 'a
) -> And<'a, I, A, B> {
  And {
    parser_a: Box::new(parser_a),
    parser_b: Box::new(parser_b),
  }
}

//fn first<'a, O: 'a>(os: &'a Vec<O>) -> &'a O {
//  os.iter()
//    .nth(0)
//    .unwrap()
//}
//
//fn last<O>(os: &Vec<O>) -> &O {
//  os.iter()
//    .nth_back(0)
//    .unwrap()
//}

pub struct Only<'a, I: 'a + Clone, D: 'a + Clone> {
  return_value: D,
  _marker: PhantomData<&'a I>
}
impl<'a, I: 'a + Clone, D: 'a + Clone> Parser<'a, I, D> for Only<'a, I, D> {
  fn parse(&self, input: I) -> ParseResult<'a, I, D> {
    Ok(ResultData::new(input, self.return_value.clone()))
  }
}
impl<'a, I: 'a + Clone, D: 'a + Clone> Only<'a, I, D> {
  pub fn new(return_value: D) -> Self {
    Only {
      return_value,
      _marker: PhantomData,
    }
  }
}
pub fn only<'a, I: 'a + Clone, D: 'a + Clone>(return_value: D) -> Only<'a, I, D> {
  Only::new(return_value)
}

//pub struct Pipe<'a, I: 'a + Clone, O: 'a + Clone> {
//  parsers: Vec<BoxedParser<'a, I, O>>,
//}
//impl<'a, I: 'a + Clone, O: 'a + Clone> Pipe<'a, I, O> {
//  pub fn pipe(&'a mut self, parser: impl Parser<'a, I, O> + 'a) -> &'a Self {
//    self.parsers.push(BoxedParser::new(parser));
//    self
//  }
//}
//impl<'a, I: 'a + Clone, O: 'a + Clone> Parser<'a, I, Vec<O>> for Pipe<'a, I, O> {
//  fn parse(&'a mut self, input: I) -> ParseResult<'a, I, Vec<O>> {
//    let first_parser = BoxedParser::new(map(first(&self.parsers), |a| vec![a]));
//    let mut a = &self.parsers.into_iter()
//      .fold(first_parser, |acc, parser| {
//        BoxedParser::new(map(and(acc, parser), |(rs, r)| {
//          once(r).chain(rs.into_iter()).collect()
//        }))
//      });
//    a.parse(input)
//  }
//}
