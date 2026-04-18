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