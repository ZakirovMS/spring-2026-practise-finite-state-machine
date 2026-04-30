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

long long power(long long base, long long exp)
{
  if (exp < 0)
  {
    throw std::invalid_argument("Negative exponent in integer context");
  }
  long long res = 1;
  for (long long i = 0; i < exp; ++i)
  {
    res = multiply(res, base);
  }
  return res;
}

long long concatenate(long long a, long long b)
{
  std::string combo = std::to_string(a) + std::to_string(b);

  try
  {
    return std::stoll(combo);
  }
  catch(const std::out_of_range&)
  {
    throw std::overflow_error("Concatenation overflow");
  }
}

long long negate(long long a)
{
  if (a == MIN)
  {
    throw std::overflow_error("Negation overflow");
  }
  return -a;
}

long long logarithm(long long a)
{
  if (a <= 0)
  {
    throw std::invalid_argument("Natural log of not positive number");
  }
  return static_cast< long long >(std::log(static_cast< double >(a)));
}

long long computeBinary(long long a, long long b, const std::string& op)
{
  if (op == "+")
  {
    return sum(a, b);
  }
  else if (op == "-")
  {
    return subtract(a, b);
  }
  else if (op == "*")
  {
    return multiply(a, b);
  }
  else if (op == "/")
  {
    return divide(a, b);
  }
  else if (op == "%")
  {
    return mod(a, b);
  }
  else if (op == "|")
  {
    return concatenate(a, b);
  }
  else if (op == "^")
  {
    return power(a, b);
  }
  throw std::invalid_argument("Unknown binary operator");
}

long long computeUnary(long long a, const std::string& op)
{
  if (op == "~")
  {
    return negate(a);
  }
  else if (op == "ln")
  {
    return logarithm(a);
  }
  throw std::invalid_argument("Unknown unary operator");
}

long long evaluatePostfix(const std::queue< std::pair< std::string, size_t > >& tokens)
{
  std::stack< long long > stk;
  std::queue< std::pair< std::string, size_t > > queue = tokens;

  while (!queue.empty())
  {
    auto token = queue.front();
    queue.pop();

    if (std::isdigit(static_cast< unsigned char >(token.first[0])))
    {
      stk.push(std::stoll(token.first));
    }
    else
    {
      try
      {
        if (token.first == "~" || token.first == "ln")
        {
          if (stk.empty())
          {
            throw std::runtime_error("Stack underflow on unary operator");
          }
          long long a = stk.top();
          stk.pop();
          stk.push(computeUnary(a, token.first));
        }
        else
        {
          if (stk.size() < 2)
          {
            throw std::runtime_error("Stack underflow on binary operator");
          }
          long long b = stk.top();
          stk.pop();
          long long a = stk.top();
          stk.pop();
          stk.push(computeBinary(a, b, token.first));
        }
      }
      catch (const std::exception& e)
      {
        std::string msg = "at pos " + std::to_string(token.second) + " " + e.what();
        throw std::runtime_error(msg);
      }
    }
  }

  if (stk.size() != 1)
  {
    throw std::runtime_error("Invalid expression structure");
  }
  return stk.top();
}

#endif
