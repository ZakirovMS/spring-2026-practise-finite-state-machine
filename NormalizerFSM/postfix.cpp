#ifndef POSTFIX_CPP
#define POSTFIX_CPP
#include "postfix.hpp"

size_t getPriority(const std::string& op)
{
  if (op == "~")
  {
    return 5;
  }
  else if (op == "ln")
  {
    return 4;
  }
  else if (op == "^")
  {
    return 3;
  }
  else if (op == "*" ||  op == "/" || op == "%")
  {
    return 2;
  }
  else if (op == "+" ||  op == "-" || op == "|")
  {
    return 1;
  }
  return 0;
}

bool isRightAssociative(const std::string& op)
{
  return (op == "~" ||  op == "ln" || op == "^");
}

long long sum(long long a, long long b)
{
  if (b > 0 && a > MAX - b)
  {
    throw std::overflow_error("Sum overflow");
  }
  else if (b < 0 && a < MIN - b)
  {
    throw std::overflow_error("Sum underflow");
  }
  return a + b;
}

#endif
