#ifndef __TM_PARSE_H__
#define __TM_PARSE_H__
#include "state.h"
#include "tm.h"
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <set>
class tm_parser {
public:
  struct error {
    const char *what() { return descri.c_str(); }
    std::string descri;
    std::string relate_segment;
    struct {
      int col, row;
    } loc;
    enum { lex, syntax } error_type;
  };
  tm_parser(const std::string &t);
  ~tm_parser() { delete result_tm; }
  turing_machine *res() { return result_tm; }
  void parse();
  void eat_input();

private:
  std::string text;
  const static std::set<char> schar;
  const static std::set<char> gchar;
  int col, row;
  int at;
  char curr_char;
  bool validate_q(char c) { return isalnum(c) || c == '_'; }
  bool validate_s(char c) {
    return isgraph(c) &&
           !schar.count(c);
  }
  static bool validate_g(char c) {
    return isgraph(c) && !gchar.count(c);
  }
  template <typename what>
  void throw_if(error e, std::function<bool(what)> f, what w);
  char one_symbol_char();
  tm_state one_state(bool star = false);
  std::string one_str(bool star = false);
  std::string one_stream() {
    std::string sb;
    while (validate_g(curr_char) || curr_char == '*') {
      sb.push_back(curr_char);
      advance();
    }
    return sb;
  }
  trans delta();
  void skip_whitespace();
  void skip_comment();
  void advance();
  bool end() const;
  turing_machine *result_tm;
  std::set<tm_state> states;        // Q
  std::set<tm_state> finish_states; // F
  std::set<char> input_symbol;      // S
  std::set<char> tape_symbol;       // G
  tm_state start_state;             // q0
  char space_symbol;
  int _N; // tapes;
  bool Q, B, F, S, G, q0, N, tr;
  std::map<tm_state, std::vector<trans>> transitions;
};

#endif