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

long long subtract(long long a, long long b)
{
  if (b > 0 && a < MIN + b)
  {
    throw std::overflow_error("Subtract underflow");
  }
  else if (b < 0 && a > MAX + b)
  {
    throw std::overflow_error("Subtract overflow");
  }
  return a - b;
}

long long multiply(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return 0;
  }

  if (a > 0)
  {
    if (b > 0 && a > MAX / b)
    {
      throw std::overflow_error("Mult overflow");
    }
    else if (b < 0 && b < MIN / a)
    {
      throw std::overflow_error("Mult underflow");
    }
  }
  else
  {
    if (b > 0 && a < MIN / b)
    {
      throw std::overflow_error("Mult underflow");
    }
    else if (b < 0 && a < MAX / b)
    {
      throw std::overflow_error("Mult overflow");
    }
  }
  return a * b;
}

long long divide(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }

  if (a == MIN && b == -1)
  {
    throw std::overflow_error("Division overflow");
  }
  return a / b;
}

long long mod(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }
  else
  {
    return (a % b + b) % b;
  }
}

long long power(long long base, long long exp);

long long concatenate(long long a, long long b);

long long negate(long long a);

long long logarithm(long long a);

long long computeBinary(long long a, long long b, const std::string& op);

long long computeUnary(long long a, const std::string& op);

long long evaluatePostfix(const std::queue< std::pair< std::string, size_t > >& tokens);

std::queue< std::pair< std::string, size_t > > convertToPostfix(const std::vector< std::pair< std::string, size_t > >& tokens);

#endif
