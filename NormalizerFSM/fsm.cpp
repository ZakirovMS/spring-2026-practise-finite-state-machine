#ifndef FSM_CPP
#define FSM_CPP
#include "fsm.hpp"
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

FSM::ValidationResult FSM::analyze(const std::string& input)
{
  curr_state_ = State::Start;
  output_tokens_.clear();
  number_buffer_.clear();
  number_buffer_start_ = 0;
  open_bracket_pos_.clear();
  last_error_pos_ = 0;

  bool has_content = false;
  for (char ch : input)
  {
    if (!std::isspace(static_cast< unsigned char >(ch)))
    {
      has_content = true;
      break;
    }
  }
  if (!has_content)
  {
    return ValidationResult{false, {}, 0, "Empty expression"};
  }

  for (size_t i = 0; i <= input.size(); ++i)
  {
    char c = (i < input.size()) ? input[i] : '\0';
    InputType inp = classify(c, input, i);
    Transition tr = getTransition(curr_state_, inp);

    if (curr_state_ == State::Number && tr.nextstate != State::Number && !number_buffer_.empty())
    {
      output_tokens_.emplace_back(number_buffer_, number_buffer_start_);
      number_buffer_.clear();
    }

    applyAction(tr, c, i);

    if (tr.action == Action::Set_error)
    {
      size_t pos = last_error_pos_;
      if (inp == InputType::End && i > 0)
      {
        pos = i - 1;
      }
      return ValidationResult{false, {}, pos, tr.token};
    }

    if (c == '(')
    {
      open_bracket_pos_.push_back(i);
    }
    else if (c == ')')
    {
      if (open_bracket_pos_.empty())
      {
        return ValidationResult{false, {}, i, "Unexpected closing bracket"};
      }
      open_bracket_pos_.pop_back();
    }

    curr_state_ = tr.nextstate;
  }

  if (finalize() && open_bracket_pos_.empty())
  {
    return ValidationResult{true, std::move(output_tokens_), 0, ""};
  }

  if (!open_bracket_pos_.empty())
  {
    return ValidationResult{false, {}, open_bracket_pos_.front(), "Mismatched parentheses"};
  }

  return ValidationResult{false, {}, last_error_pos_ != 0 ? last_error_pos_ : input.size(), "Incomplete expression"};
}

#endif
