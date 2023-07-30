#ifndef __TM_TAPE_H__
#define __TM_TAPE_H__

#include <vector>
class tm_tape {
public:
  tm_tape(int size = 4096, char B = '_') {
    this->size = size;
    positive_tape.resize(size, B);
    negative_tape.resize(size, B);
    zero_c = 0;
  }
  char &operator[](int index);
  const char &operator[](int index) const;
  void resize();
  void set_value(char B) {
    zero_c = B;
    positive_tape = std::vector<char>(size, B);
    negative_tape = std::vector<char>(size, B);
    this->B = B;
  }
  bool is_exceed(int index);
  int max_index() { return size; }
  int min_index() { return -size; }

private:
  char zero_c;
  std::vector<char> positive_tape;
  std::vector<char> negative_tape;
  char B;
  int size;
};

struct tape_ptr {
  int index = 0;
  int leftmost = 0;
  int rightmost = 0;
  void set_leftmost(int l) { leftmost = l; }
  void set_rightmost(int r) { rightmost = r; }
  tape_ptr operator++() { return tape_ptr{++index, leftmost, rightmost}; }
  tape_ptr operator++(int) {
    tape_ptr old = *this;
    ++index;
    return old;
  }
  tape_ptr operator--() { return tape_ptr{--index, leftmost, rightmost}; }
  tape_ptr operator--(int) {
    tape_ptr old = *this;
    --index;
    return old;
  }
};

#endif