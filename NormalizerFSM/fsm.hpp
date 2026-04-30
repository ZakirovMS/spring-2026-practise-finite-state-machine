#ifndef FSM_HPP
#define FSM_HPP
#include <string>
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

class FSM
{
public:
  struct ValidationResult
  {
    bool success;
    std::vector< std::pair< std::string, size_t > > tokens;
    size_t error_ind;
    std::string error_msg;
  };

  ValidationResult analyze(const std::string& input);

private:
  State curr_state_ = State::Start;
  std::vector< std::pair< std::string, size_t > > output_tokens_;
  std::string number_buffer_;
  size_t number_buffer_start_ = 0;
  std::vector< size_t > open_bracket_pos_;
  size_t last_error_pos_ = 0;

  InputType classify(char c, const std::string& input, size_t i) const;
  Transition getTransition(State curr, InputType inp) const;
  void applyAction(const Transition& tr, char c, size_t i);
  bool finalize();
};

#endif
