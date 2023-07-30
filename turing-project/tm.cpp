#include "tm.h"
#include "state.h"
#include "tape.h"
#include <cstdio>
#include <iostream>
#include <iterator>
#include <ostream>


const tm_state turing_machine::dead{""};

void turing_machine::add_state(const tm_state &state) { states.emplace(state); }

void turing_machine::add_final_state(const tm_state &fstate) {
  final_states.emplace(fstate);
}

void turing_machine::add_input_symbol(char c) { input_symbol.emplace(c); }

void turing_machine::set_space_symbol(char c) { space_symbol = c; }

void turing_machine::add_tape_symbol(char c) { tape_symbol.emplace(c); }

void turing_machine::set_start_state(const tm_state &sstate) {
  start_state = sstate;
  curr_state = start_state;
}

turing_machine::id turing_machine::get_id() const {
  return {&tape, &tapeptr, &curr_state};
}

void turing_machine::validate_input(const std::string &input) {
  for (int i = 0; i < input.size(); ++i) {
    if (!input_symbol.count(input[i])) {
      throw error{input, i};
    }
  }
  this->input = input;
  for (int i = 0; i < input.size(); ++i) {
    tape[0][i] = input[i];
  }
  tapeptr[0].index = 0;
  tapeptr[0].set_rightmost((int)input.size() - 1);
}

#define print_helper(setname, vec, it, expr)                                   \
  do {                                                                         \
    os << (setname) << " = {";                                                 \
    if (!(vec).empty()) {                                                      \
      auto it = std::begin((vec));                                             \
      os << (expr);                                                            \
      while (++it != std::end((vec))) {                                        \
        os << ", " << (expr);                                                  \
      }                                                                        \
    }                                                                          \
    os << "}" << std::endl;                                                    \
  } while (0)

std::ostream &operator<<(std::ostream &os, const turing_machine &tm) {
  print_helper("Q", tm.states, it, it->name());
  print_helper("F", tm.final_states, it, it->name());
  print_helper("S", tm.input_symbol, it, *it);
  print_helper("G", tm.tape_symbol, it, *it);
  os << "q0 = " << tm.start_state.name() << std::endl;
  os << "B = " << tm.space_symbol << std::endl;
  os << "N = " << tm.N << std::endl;
  for (auto i = std::begin(tm.transitions); i != std::end(tm.transitions);) {
    auto t = i->second;
    for (auto j = std::begin(t); j != std::end(t);) {
      os << i->first.name() << ' ';
      for (const auto &u : j->view) {
        os << u;
      }
      os << ' ';
      for (const auto &u : j->get) {
        os << u;
      }
      os << ' ';
      for (const auto &u : j->direc) {
        os << u;
      }
      os << ' ';
      os << j->p.name();
      if (++j != std::end(t))
        os << std::endl;
    }
    if (++i != std::end(tm.transitions))
      os << std::endl;
  }
  return os;
}

#undef print_helper

char &turing_machine::peek_tape(int tape_id) {
  return tape[tape_id][tapeptr[tape_id].index];
}

bool turing_machine::match_tape(const std::string &view) {
  for (int i = 0; i < view.size(); ++i) {
    if (view[i] != peek_tape(i) &&
        (view[i] != '*' || (view[i] == '*' && peek_tape(i) == space_symbol))) {
      return false;
    }
  }
  return true;
}

void turing_machine::single_transition(int tape_id, char writedown, char dirc,
                                       const tm_state &p) {
  if (writedown != '*')
    peek_tape(tape_id) = writedown;
  if (dirc == 'l') {
    --(tapeptr[tape_id]);
  } else if (dirc == 'r') {
    ++(tapeptr[tape_id]);
  }
  int i = 0;
  tapeptr[tape_id].set_leftmost(tape[tape_id].max_index());
  tapeptr[tape_id].set_rightmost(tape[tape_id].min_index());
  for (i = tape[tape_id].min_index(); i <= tape[tape_id].max_index(); ++i) {
    if (tape[tape_id][i] != space_symbol) {
      tapeptr[tape_id].set_leftmost(i);
      break;
    }
  }
  for (i = tape[tape_id].max_index(); i >= tape[tape_id].min_index(); --i) {
    if (tape[tape_id][i] != space_symbol) {
      tapeptr[tape_id].set_rightmost(i);
      break;
    }
  }
  curr_state = p;
}

void turing_machine::set_tapes(int N) {
  this->N = N;
  tape.resize(N);
  for (auto &i : tape) {
    i.set_value(space_symbol);
  }
  tapeptr.resize(N);
  tapeptr.resize(N);
}

void turing_machine::one_step() {
  auto tr = transitions[curr_state];
  for (const auto &i : tr) {
    if (match_tape(i.view)) {
      for (int j = 0; j < N; ++j) {
        single_transition(j, i.get[j], i.direc[j], i.p);
      }
      return;
    }
  }
  curr_state = dead; // it's dead now.
}

void turing_machine::add_transition(tm_state p, trans de) {
  transitions[p].push_back(de); // 哈哈, 这个[]的行为太奇怪了
}

bool turing_machine::is_halt() { return curr_state == dead; }

void turing_machine::do_print() {
  for (int i = tapeptr[0].leftmost; i <= tapeptr[0].rightmost; ++i) {
    std::cout << tape[0][i];
  }
  std::cout << std::endl;
}