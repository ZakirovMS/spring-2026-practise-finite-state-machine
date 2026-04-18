#include "FSM.hpp"
#include <cctype>

InputType FSM::classify(char c, const std::string& input, size_t i) const
{
  if (c == 'n' && i > 0 && input[i - 1] == 'l')
  {
    return InputType::Ignored;
  }
  else if (std::isdigit(c))
  {
    return InputType::Digit;
  }
  else if (c == '.')
  {
    return InputType::Dot;
  }
  else if (c == '-')
  {
    return InputType::Un_op;
  }
  else if (c == '+' || c == '*' || c == '/' || c == '%' || c == '^' || c == '|')
  {
    return InputType::Bin_op;
  }
  else if (c == '(')
  {
    return InputType::Lbracket;
  }
  else if (c == ')')
  {
    return InputType::Rbracket;
  }
  else if (c == 'l' && i + 1 < input.size() && input[i + 1] == 'n')
  {
    return InputType::Lnprefix;
  }
  else if (std::isspace(static_cast< unsigned char >(c)))
  {
    return InputType::Ignored;
  }
  else if (i == input.size() || c == '\0')
  {
    return InputType::End;
  }
  return InputType::Unknown;
}

Transition FSM::getTransition(State curr, InputType inp) const
{
  switch (curr)
  {
  case State::Start:
    switch (inp)
    {
    case InputType::Digit:
      return {State::Number, Action::Continue, ""};
    case InputType::Dot:
      return {State::Error, Action::Set_error, "Invalid number format"};
    case InputType::Bin_op:
      return {State::Error, Action::Set_error, "Invalid expression start"};
    case InputType::Un_op:
      return {State::Unary_minus, Action::Push_marker, "~"};
    case InputType::Lbracket:
      return {State::Open_bracket, Action::Push, "("};
    case InputType::Rbracket:
      return {State::Error, Action::Set_error, "Unexpected closing bracket"};
    case InputType::Lnprefix:
      return {State::Open_bracket, Action::Push, "ln"};
    case InputType::Ignored:
      return {State::Start, Action::Skip, ""};
    case InputType::End:
      return {State::Error, Action::Set_error, "Empty expression"};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  case State::Number:
    switch (inp)
    {
    case InputType::Digit:
      return {State::Number, Action::Continue, ""};
    case InputType::Dot:
      return {State::Error, Action::Set_error, "Invalid number format"};
    case InputType::Bin_op:
    case InputType::Un_op:
      return {State::Operator, Action::Push, ""};
    case InputType::Lbracket:
    case InputType::Lnprefix:
      return {State::Error, Action::Set_error, "Missing operator between expressions"};
    case InputType::Rbracket:
      return {State::Close_bracket, Action::Push, ")"};
    case InputType::Ignored:
    case InputType::End:
      return {State::Number, Action::Skip, ""};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  case State::Operator:
    switch (inp)
    {
    case InputType::Digit:
      return {State::Number, Action::Continue, ""};
    case InputType::Dot:
      return {State::Error, Action::Set_error, "Invalid number format"};
    case InputType::Bin_op:
      return {State::Error, Action::Set_error, "Invalid operator sequence"};
    case InputType::Un_op:
      return {State::Unary_minus, Action::Push_marker, "~"};
    case InputType::Lbracket:
      return {State::Open_bracket, Action::Push, "("};
    case InputType::Rbracket:
      return {State::Error, Action::Set_error, "Invalid operator sequence"};
    case InputType::Lnprefix:
      return {State::Open_bracket, Action::Push, "ln"};
    case InputType::Ignored:
      return {State::Operator, Action::Skip, ""};
    case InputType::End:
      return {State::Error, Action::Set_error, "Incomplete expression"};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  case State::Open_bracket:
    switch (inp)
    {
    case InputType::Digit:
      return {State::Number, Action::Continue, ""};
    case InputType::Dot:
      return {State::Error, Action::Set_error, "Invalid number format"};
    case InputType::Bin_op:
      return {State::Error, Action::Set_error, "Invalid operator sequence"};
    case InputType::Un_op:
      return {State::Unary_minus, Action::Push_marker, "~"};
    case InputType::Lbracket:
      return {State::Open_bracket, Action::Push, "("};
    case InputType::Rbracket:
      return {State::Error, Action::Set_error, "Empty parentheses"};
    case InputType::Lnprefix:
      return {State::Open_bracket, Action::Push, "ln"};
    case InputType::Ignored:
      return {State::Open_bracket, Action::Skip, ""};
    case InputType::End:
      return {State::Error, Action::Set_error, "Mismatched parentheses"};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  case State::Close_bracket:
    switch (inp)
    {
    case InputType::Digit:
    case InputType::Dot:
    case InputType::Lbracket:
    case InputType::Lnprefix:
      return {State::Error, Action::Set_error, "Missing operator between expressions"};
    case InputType::Bin_op:
    case InputType::Un_op:
      return {State::Operator, Action::Push, ""};
    case InputType::Rbracket:
      return {State::Close_bracket, Action::Push, ")"};
    case InputType::Ignored:
    case InputType::End:
      return {State::Close_bracket, Action::Skip, ""};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  case State::Unary_minus:
    switch (inp)
    {
    case InputType::Digit:
      return {State::Number, Action::Continue, ""};
    case InputType::Dot:
      return {State::Error, Action::Set_error, "Invalid number format"};
    case InputType::Bin_op:
      return {State::Error, Action::Set_error, "Invalid operator sequence"};
    case InputType::Un_op:
      return {State::Unary_minus, Action::Push_marker, "~"};
    case InputType::Lbracket:
      return {State::Open_bracket, Action::Push, "("};
    case InputType::Rbracket:
      return {State::Error, Action::Set_error, "Unexpected closing bracket"};
    case InputType::Lnprefix:
      return {State::Open_bracket, Action::Push, "ln"};
    case InputType::Ignored:
      return {State::Unary_minus, Action::Skip, ""};
    case InputType::End:
      return {State::Error, Action::Set_error, "Incomplete expression"};
    default:
      return {State::Error, Action::Set_error, "Invalid character"};
    }
  default:
    return {State::Error, Action::Set_error, "Unknown state"};
  }
}

void FSM::applyAction(const Transition& tr, char c, size_t i)
{
  switch (tr.action)
  {
  case Action::Push:
    output_tokens_.emplace_back(tr.token.empty() ? std::string(1, c) : tr.token, i);
    break;
  case Action::Push_marker:
    output_tokens_.emplace_back(tr.token, i);
    break;
  case Action::Continue:
    if (number_buffer_.empty())
    {
      number_buffer_start_ = i;
    }
    number_buffer_ += c;
    break;
  case Action::Skip:
    break;
  case Action::Set_error:
    last_error_pos_ = i;
    break;
  }
}

bool FSM::finalize()
{
  if (!number_buffer_.empty())
  {
    output_tokens_.emplace_back(number_buffer_, number_buffer_start_);
    number_buffer_.clear();
  }

  if (curr_state_ == State::Operator || curr_state_ == State::Unary_minus || curr_state_ == State::Open_bracket)
  {
    return false;
  }

  return true;
}
