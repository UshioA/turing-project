#include "run.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
using namespace std;

void help() {
  fputs("usage: turing [-v|--verbose] [-h|--help] <tm> <input>", stderr);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  std::string tmpath;
  std::string input;
  bool verbose = 0;
  int howmany = 0;
  if (argc < 2)
    help();
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      verbose = 1;
    } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      help();
    } else {
      if (howmany < 2) {
        (howmany ? input : tmpath) = argv[i];
        ++howmany;
      } else
        help();
    }
  }
  if (tmpath.empty())
    help();
  runner elieen(verbose);
  elieen.load_program(tmpath);
  elieen.verify_input(input);
  elieen.run();
  return 0;
}
