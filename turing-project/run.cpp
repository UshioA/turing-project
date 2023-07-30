#include "run.h"
#include "parse.h"
#include "tape.h"
#include "tm.h"
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <string>
#include <sys/types.h>

void runner::one_step() {
  tm->one_step();
  ++step;
  curr_id = tm->get_id();
}

void runner::print_id() {
  auto tape_num = curr_id.tape->size();
  std::cout << std::left << std::setw(6 + digit(tape_num)) << "Step"
            << ": " << step << std::endl;
  std::cout << std::left << std::setw(6 + digit(tape_num)) << "State"
            << ": " << curr_id.curr_state->name() << std::endl;
  for (int i = 0; i < tape_num; ++i) {
    std::cout << "Index" << std::left << std::setw(1 + digit(tape_num)) << i
              << ": ";
    size_t space_num = print_index((*(curr_id.tape))[i], (*(curr_id.tapeptr))[i]);
    std::cout << "Tape" << std::left << std::setw(2 + digit(tape_num)) << i
              << ": ";
    print_tape((*(curr_id.tape))[i], (*(curr_id.tapeptr))[i]);
    std::cout << "Head" << std::left << std::setw(2 + digit(tape_num)) << i
              << ": ";
    print_head(space_num);
  }
}

void runner::print_tape(const tm_tape &tp, const tape_ptr &tptr) {
  int l = std::min(tptr.leftmost, tptr.index);
  int r = std::max(tptr.rightmost, tptr.index);
  for (int i = l; i <= r; ++i) {
    std::cout << std::left << std::setw(std::to_string(abs(i)).size()) << tp[i]
              << ' ';
  }
  std::cout << std::endl;
}

size_t runner::print_index(const tm_tape &tp, const tape_ptr &tptr) {
  size_t width = 0;
  bool stop=0;
  for (int i = std::min(tptr.leftmost, tptr.index);
       i <= std::max(tptr.rightmost, tptr.index); ++i) {
    if (i == tptr.index)
      stop = 1;
    if (!stop) {
      width += digit(abs(i)) + 1;
    }
    std::cout << abs(i) << ' ';
  }
  std::cout << std::endl;
  return width;
}

void runner::print_head(size_t where) {
  std::cout << std::string(where, ' ') << '^' << std::endl;
}

void runner::one_step_and_print_id() {
  one_step();
  print_id();
}

void runner::load_program(const std::string &filepath) {
  std::ifstream p(filepath);
  if (p.fail()) {
    fprintf(stderr, "fatal error! %s : no such file or directory\n",
            filepath.c_str());
    exit(EXIT_FAILURE);
  }
  std::string sb;
  std::string tmp;
  while (getline(p, tmp)) {
    sb += tmp;
    sb += '\n';
  }
  this->tmp = new tm_parser(sb);
  try {
    this->tmp->eat_input();
    this->tmp->parse();
  } catch (tm_parser::error tmpe) {
    parser_error_handle(tmpe);
  } catch (turing_machine::error tme) {
    assert(0);
  }
  tm = this->tmp->res();
  curr_id = tm->get_id();
}

void runner::print_result() { tm->do_print(); }

void runner::verify_input(const std::string &input) {
  try {
    if (verbose) {
      printf("Input: %s\n", input.c_str());
    }
    tm->validate_input(input);
  } catch (turing_machine::error tme) {
    if (verbose) {
      draw_err();
      input_error_handle(tme);
    } else {
      fprintf(stderr, "illegal input\n");
    }
    die(114514);
  }
  assert(!tm->is_halt());
}

void runner::input_error_handle(turing_machine::error tme) {
  fprintf(stderr, "error: '%c' was not declared in the set of input symbols\n",
          tme.input[tme.loc]);
  fprintf(stderr, "input: %s\n", tme.input.c_str());
  fprintf(stderr, "%s^\n", std::string(7 + tme.loc, ' ').c_str());
}

void runner::parser_error_handle(tm_parser::error pe) {
  if (verbose) {
    if (pe.error_type == tm_parser::error::lex) {
      fprintf(stderr, "lex error %s at (%d, %d) : `%s'\n", pe.what(),
              pe.loc.col, pe.loc.row, pe.relate_segment.c_str());
    } else {
      fprintf(stderr, "syntax error %s : %s\n", pe.what(),
              pe.relate_segment.c_str());
    }
    exit(114514);
  } else {
    fprintf(stderr, "syntax error\n");
    exit(114514);
  }
}

void runner::die(int code) {
  // 风光大葬!
  if (verbose) {
    draw_end();
  }
  exit(code);
}

void runner::run() {
  if (verbose)
    draw_run();
  while (!tm->is_halt()) {
    if (verbose) {
      print_id();
      draw_line();
    }
    one_step();
  }
  if (verbose) {
    printf("Result: ");
  }
  print_result();
  die(0);
}
