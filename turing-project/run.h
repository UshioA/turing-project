#ifndef __RUN_H__
#define __RUN_H__
#include "parse.h"
#include "tape.h"
#include "tm.h"
#include <string>
class runner {
public:
  runner(bool verbose = false)
      : verbose(verbose), tm(nullptr), tmp(nullptr), step(0) {}

  ~runner() {
    if (tmp)
      delete tmp;
    tmp = nullptr;
  }

  void one_step();
  void print_id();
  void one_step_and_print_id();
  void print_result();
  size_t print_index(const tm_tape &tp, const tape_ptr &tptr);
  void print_head(size_t where);
  void print_tape(const tm_tape &tp, const tape_ptr &tptr);
  void draw(const char *what) { std::cout << what << std::endl; }
  void draw_line() { draw(this_is_a_line); }
  void draw_err() { draw(this_is_an_err); }
  void draw_end() { draw(this_is_an_end); }
  void draw_run() { draw(this_is_a_run); }
  void verify_input(const std::string &input);
  void load_program(const std::string &filepath);
  void run();
  void die(int code);
  void input_error_handle(turing_machine::error tme);
  void parser_error_handle(tm_parser::error pe);
  turing_machine *get_tm() { return tm; }

private:
  bool verbose;
  int step;
  const char *this_is_a_line = "---------------------------------------------";
  const char *this_is_an_err = "==================== ERR ====================";
  const char *this_is_an_end = "==================== END ====================";
  const char *this_is_a_run = "==================== RUN ====================";
  size_t digit(size_t n) { return std::to_string(n).length(); }
  tm_parser *tmp;
  turing_machine *tm;
  turing_machine::id curr_id;
};

#endif