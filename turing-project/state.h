#ifndef __TM_STATE_H__
#define __TM_STATE_H__
#include <iostream>

class tm_state {
public:
  tm_state() {}
  tm_state(std::string state) { state_name = state; }
  tm_state(const tm_state &s) { state_name = s.state_name; }
  const std::string &name() const { return state_name; }
  bool operator==(const tm_state &other) const {
    return state_name == other.state_name;
  }
  bool operator<(const tm_state &other) const {
    return state_name < other.state_name;
  }

private:
  std::string state_name;
};

#endif