#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include <limits>
#include <stack>
#include <queue>
#include <stdexcept>
#include <cmath>
#include <string>
#include <utility>

const long long MAX = std::numeric_limits< long long >::max();
const long long MIN = std::numeric_limits< long long >::min();

size_t getPriority(const std::string& op);
bool isRightAssociative(const std::string& op);

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
long long computeUnary(long long a, const std::string& op)

long long evaluatePostfix(const std::queue< std::pair< std::string, size_t > >& tokens);

std::queue< std::pair< std::string, size_t > > convertToPostfix(const std::vector< std::pair< std::string, size_t > >& tokens);

#endif