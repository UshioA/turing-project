#include "tape.h"
#include <cstdlib>

char &tm_tape::operator[](int index) {
  if (is_exceed(index))
    resize();
  if (index > 0) {
    return positive_tape[index - 1];
  } else if (index == 0) {
    return zero_c;
  } else {
    return negative_tape[-index - 1];
  }
}

const char &tm_tape::operator[](int index) const {
  if (index > 0) {
    return positive_tape[index - 1];
  } else if (index == 0) {
    return zero_c;
  } else {
    return negative_tape[-index - 1];
  }
}

void tm_tape::resize() {
  positive_tape.resize(this->size << 1, B);
  negative_tape.resize(this->size << 1, B);
  this->size <<= 1;
}

bool tm_tape::is_exceed(int index) { return abs(index) - 1 >= size; }