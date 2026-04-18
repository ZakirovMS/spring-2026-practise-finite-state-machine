#ifndef FSM_H
#define FSM_H

#include <string>
#include <utility>
#include <vector>

enum class State
{
  Start,
  Number,
  Operator,
  Open_bracket,
  Close_bracket,
  Unary_minus,
  Error
};

enum class InputType
{
  Digit,
  Dot,
  Bin_op,
  Un_op,
  Lbracket,
  Rbracket,
  Lnprefix,
  Ignored,
  Unknown,
  End
};

enum class Action
{
  Push,
  Push_marker,
  Skip,
  Set_error,
  Continue
};

struct Transition
{
  State nextstate;
  Action action;
  std::string token;
};

#endif
