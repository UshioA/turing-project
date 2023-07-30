#include "parse.h"
#include "state.h"
#include "tm.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>

const std::set<char> tm_parser::schar = {' ', ',', ';', '{', '}', '*', '_'};
const std::set<char> tm_parser::gchar = {' ', ',', ';', '{', '}', '*'};
tm_parser::tm_parser(const std::string &t) {
  text = t;
  curr_char = t[0];
  at = col = row = 0;
  result_tm = new turing_machine;
}

bool tm_parser::end() const { return at >= text.size(); }

void tm_parser::skip_whitespace() {
  while (std::isspace(curr_char)) {
    advance();
  }
}

void tm_parser::advance() {
  if (curr_char == '\n') {
    col = 0;
    ++row;
  } else {
    ++col;
  }
  ++at;
  curr_char = text[at];
}

void tm_parser::skip_comment() {
  while (curr_char != '\n') {
    advance();
  }
  advance();
}

trans tm_parser::delta() {
  tm_state p;
  tm_state q;
  std::string view;
  std::string get;
  std::string direc;

  p = one_state(1);
  skip_whitespace();
  auto tmp = one_stream();
  for (const auto &i : tmp) {
    view.push_back(i);
  }
  skip_whitespace();
  tmp = one_stream();
  for (const auto &i : tmp) {
    get.push_back(i);
  }
  skip_whitespace();
  tmp = one_stream();
  for (const auto &i : tmp) {
    direc.push_back(i);
  }
  skip_whitespace();
  q = one_state(1);
  return trans{p, q, view, get, direc};
}

std::string tm_parser::one_str(bool star) {
  std::string sb;
  while (tm_parser::validate_q(curr_char) || (star && curr_char == '*')) {
    sb.push_back(curr_char);
    advance();
  }
  return sb;
}

tm_state tm_parser::one_state(bool star) { return tm_state(one_str(star)); }

char tm_parser::one_symbol_char() {
  throw_if<char>(
      error{
          "unexpected char", std::string(1, curr_char), {col, row}, error::lex},
      [](char c) { return c == ','; }, curr_char);
  char c = curr_char;
  advance();
  return c;
}

template <typename what>
void tm_parser::throw_if(error e, std::function<bool(what)> f, what w) {
  if (f(w))
    throw e;
}

#define bracket_helper(func, type, table)                                      \
  do {                                                                         \
    while (curr_char != '{') {                                                 \
      advance();                                                               \
    }                                                                          \
    skip_whitespace();                                                         \
    advance();                                                                 \
    if (curr_char != '}') {                                                    \
      auto name = func();                                                      \
      table.insert(name);                                                      \
      skip_whitespace();                                                       \
      while (curr_char == ',') {                                               \
        advance();                                                             \
        skip_whitespace();                                                     \
        name = func();                                                         \
        table.insert(name);                                                    \
        skip_whitespace();                                                     \
      }                                                                        \
      throw_if<type>(                                                          \
          error{"unexpected char",                                             \
                std::string(1, curr_char),                                     \
                {col, row},                                                    \
                error::lex},                                                   \
          [](char c) { return c != '}'; }, curr_char);                         \
      advance();                                                               \
    } else {                                                                   \
      advance();                                                               \
    }                                                                          \
  } while (0)

void tm_parser::eat_input() {
  while (!end()) {
    if (curr_char == ';') {
      advance();
      skip_comment();
    } else if (curr_char == '#') {
      advance();
      switch (curr_char) {
      case 'Q': {
        Q = 1;
        bracket_helper(one_state, char, states);
      }; break;
      case 'F': {
        F = 1;
        bracket_helper(one_state, char, finish_states);
      } break;
      case 'G': {
        G = 1;
        bracket_helper(one_symbol_char, char, tape_symbol);
      } break;
      case 'S': {
        S = 1;
        bracket_helper(one_symbol_char, char, input_symbol);
      } break;
      case 'B':
      case 'N': {
        char c = curr_char;
        advance();
        while (curr_char == ' ' || curr_char == '=')
          advance();
        if (c == 'B') {
          B = 1;
          space_symbol = curr_char;
          advance();
        } else {
          N = 1;
          std::string n;
          while (isdigit(curr_char)) {
            n.push_back(curr_char);
            advance();
          }
          _N = std::stoi(n);
          throw_if<int>(
              error{"too few tapes :(",
                    std::to_string(_N),
                    {col, row},
                    error::lex},
              [](int i) { return i <= 0; }, _N);
        }
      } break;
      case 'q': {
        advance();
        throw_if<char>(
            error{"unexpected char",
                  std::string(1, curr_char),
                  {col, row},
                  error::lex},
            [](char c) { return c != '0'; }, curr_char);
        advance();
        q0 = 1;
        while (curr_char == ' ' || curr_char == '=')
          advance();
        auto sstate = one_state();
        start_state = sstate;
      } break;
      default: {
        throw_if<char>(
            error{"why you use this option?",
                  std::string(1, curr_char),
                  {col, row},
                  error::lex},
            [](char) { return true; }, 114514 & 0xff);
      }
      }
    } else if (std::isspace(curr_char)) {
      skip_whitespace();
    } else { // assume is delta
      tr = 1;
      auto t = delta();
      transitions[t.q].push_back(t);
    }
  }
}
#undef bracket_helper
void tm_parser::parse() {
  throw_if<int>(
      error{"you missed something",
            "all parts must be initialized",
            {col, row},
            error::syntax},
      [this](int i) { return !(Q && B && F && S && G && q0 && N && tr); },
      114514); // all parts must be initialized
  throw_if<tm_state>(
      error{"start state not in Q",
            start_state.name(),
            {col, row},
            error::syntax},
      [this](tm_state q0) { return states.count(q0) == 0; },
      start_state); // to check q0 in Q

  throw_if<const std::set<tm_state> &>(
      error{"finish states not subset of states",
            "please check",
            {col, row},
            error::syntax},
      [this](const std::set<tm_state> &s) {
        return !std::includes(states.begin(), states.end(), s.begin(), s.end());
      },
      finish_states); // to check F is subset of Q
  throw_if<char>(
      error{"space symbol not in G",
            std::string(1, space_symbol),
            {0, 0},
            error::syntax},
      [this](char c) { return tape_symbol.count(c) == 0; },
      space_symbol); // check space must in G

  for (const auto &u : transitions) {
    for (const auto &i : u.second) {
      auto q = i.q;
      auto p = i.p;
      auto view = i.view;
      auto get = i.get;
      auto direc = i.direc;
      throw_if<tm_state>(
          error{"transition q not in Q ", q.name(), {col, row}, error::syntax},
          [this](tm_state q) { return states.count(q) == 0; },
          q); // to check q in Q
      throw_if<tm_state>(
          error{"transition p not in Q ", p.name(), {col, row}, error::syntax},
          [this](tm_state q) { return states.count(q) == 0; },
          p); // to check p in Q
      throw_if<size_t>(
          error{"view width not match",
                std::to_string(_N),
                {0, 0},
                error::syntax},
          [this](size_t n) { return n != _N; }, view.size());
      throw_if<size_t>(
          error{"get width not match", std::to_string(_N), {}, error::syntax},
          [this](size_t n) { return n != _N; }, get.size());
      throw_if<size_t>(
          error{"direc width not match", std::to_string(_N), {}, error::syntax},
          [this](size_t n) { return n != _N; },
          direc.size()); // to check width match
      for (int j = 0; j < _N; ++j) {
        throw_if<char>(
            error{"transition symbol not in G",
                  std::string(1, view[j]),
                  {},
                  error::syntax},
            [this](char c) { return tape_symbol.count(c) == 0 && c != '*'; },
            view[j]);
        throw_if<char>(
            error{"transition symbol not in G",
                  std::string(1, get[j]),
                  {},
                  error::syntax},
            [this](char c) { return tape_symbol.count(c) == 0 && c != '*'; },
            get[j]);
        throw_if<char>(
            error{"what direction r u using :(",
                  std::string(1, direc[j]),
                  {},
                  error::syntax},
            [](char c) { return c != 'r' && c != 'l' && c != '*'; }, direc[j]);
      }
    }
  }

  for (const auto &state : states) {
    result_tm->add_state(state);
  }
  for (const auto &state : finish_states) {
    result_tm->add_final_state(state);
  }
  for (const auto &c : input_symbol) {
    result_tm->add_input_symbol(c);
  }
  for (const auto &c : tape_symbol) {
    result_tm->add_tape_symbol(c);
  }
  for (const auto &tr : transitions) {
    for (const auto &i : tr.second)
      result_tm->add_transition(tr.first, i);
  }
  result_tm->set_start_state(start_state);
  result_tm->set_space_symbol(space_symbol);
  result_tm->set_tapes(_N);
}

#undef bracket_helper
