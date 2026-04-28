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

long long sum(long long a, long long b);

long long subtract(long long a, long long b);

long long multiply(long long a, long long b);

long long divide(long long a, long long b);

long long mod(long long a, long long b);

long long power(long long base, long long exp);

long long concatenate(long long a, long long b);

long long negate(long long a);

long long logarithm(long long a);

long long computeBinary(long long a, long long b, const std::string& op);

long long computeUnary(long long a, const std::string& op);

long long evaluatePostfix(const std::queue< std::pair< std::string, size_t > >& tokens);

std::queue< std::pair< std::string, size_t > > convertToPostfix(const std::vector< std::pair< std::string, size_t > >& tokens);

#endif
