#ifndef __TM_TM_H__
#define __TM_TM_H__
#include "state.h"
#include "tape.h"
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>
class tm_tape;
struct trans {
  tm_state q;
  tm_state p;
  std::string view;
  std::string get;
  std::string direc;
};
class turing_machine {
public:
  struct error {
    std::string input;
    int loc;
  };
  struct id {
    const std::vector<tm_tape> *tape;
    const std::vector<tape_ptr> *tapeptr;
    const tm_state *curr_state;
  };
  turing_machine() {}
  void add_state(const tm_state &state);
  void add_final_state(const tm_state &fstate);
  void add_input_symbol(char c);
  void add_tape_symbol(char c);
  void set_space_symbol(char c);
  void set_start_state(const tm_state &sstate);
  void set_tapes(int N);
  void one_step();
  void add_transition(tm_state p, trans de);
  bool is_halt();
  void validate_input(const std::string &input);
  void do_print();
  id get_id() const;
  friend std::ostream &operator<<(std::ostream &os, const turing_machine &tm);

private:
  const static tm_state dead;
  void single_transition(int tape_id, char writedown, char dirc,
                         const tm_state &p);
  bool match_tape(const std::string &view);
  char &peek_tape(int tapeid);
  std::set<tm_state> states;       // Q
  std::set<tm_state> final_states; // F
  std::set<char> input_symbol;     // S
  std::set<char> tape_symbol;      // G
  tm_state start_state;            // q0
  tm_state curr_state = dead;
  char space_symbol;
  int N; // tapes;
  std::vector<tm_tape> tape;
  std::map<tm_state, std::vector<trans>> transitions;
  std::vector<tape_ptr> tapeptr;
  // TODO add manipulate
  std::string input;
};

#endif