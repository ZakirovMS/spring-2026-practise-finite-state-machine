#define BOOST_TEST_MODULE NormalizerTest
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "fsm.hpp"

BOOST_AUTO_TEST_SUITE(FsmTests)

BOOST_AUTO_TEST_CASE(empty_string)
{
  FSM fsm;
  auto res = fsm.analyze("");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Empty expression");
  BOOST_CHECK_EQUAL(res.error_ind, 0);
}

BOOST_AUTO_TEST_CASE(string_with_only_spaces)
{
  FSM fsm;
  auto res = fsm.analyze("        ");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Empty expression");
  BOOST_CHECK_EQUAL(res.error_ind, 0);
}

BOOST_AUTO_TEST_CASE(valid_with_unary_minus)
{
  FSM fsm;
  auto res = fsm.analyze("-5");
  BOOST_CHECK(res.success);
  BOOST_CHECK_EQUAL(res.tokens.size(), 2);
  BOOST_CHECK_EQUAL(res.tokens[0].first, "~");
  BOOST_CHECK_EQUAL(res.tokens[1].first, "5");
  
  auto res2 = fsm.analyze("-(-(-5))");
  BOOST_CHECK(res2.success);
  BOOST_CHECK_EQUAL(res2.tokens.size(), 8);
  BOOST_CHECK_EQUAL(res2.tokens[0].first, "~");
  BOOST_CHECK_EQUAL(res2.tokens[1].first, "(");
  
  auto res3 = fsm.analyze("5*-3");
  BOOST_CHECK(res3.success);
  BOOST_CHECK_EQUAL(res3.tokens.size(), 4);
  BOOST_CHECK_EQUAL(res3.tokens[0].first, "5");
  BOOST_CHECK_EQUAL(res3.tokens[1].first, "*");
  BOOST_CHECK_EQUAL(res3.tokens[2].first, "~");
  BOOST_CHECK_EQUAL(res3.tokens[3].first, "3");

  auto res4 = fsm.analyze("-+5");
  BOOST_CHECK(!res4.success);
  BOOST_CHECK_EQUAL(res4.error_msg, "Invalid operator sequence");
  BOOST_CHECK_EQUAL(res4.error_ind, 1);
}

BOOST_AUTO_TEST_CASE(string_with_operator_sequentially)
{
  FSM fsm;
  auto res = fsm.analyze("5**3");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Invalid operator sequence");
  BOOST_CHECK_EQUAL(res.error_ind, 2);

  auto res2 = fsm.analyze("5+-3");
  BOOST_CHECK(res2.success);
  BOOST_CHECK_EQUAL(res2.tokens.size(), 4);
  BOOST_CHECK_EQUAL(res2.tokens[0].first, "5");
  BOOST_CHECK_EQUAL(res2.tokens[1].first, "+");
  BOOST_CHECK_EQUAL(res2.tokens[2].first, "~");
  BOOST_CHECK_EQUAL(res2.tokens[3].first, "3");
}

BOOST_AUTO_TEST_CASE(invalid_with_only_brackets)
{
  FSM fsm;
  auto res = fsm.analyze("()");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Empty parentheses");
  BOOST_CHECK_EQUAL(res.error_ind, 1);
}

BOOST_AUTO_TEST_CASE(invalid_with_mismatched_parentheses)
{
  FSM fsm;
  auto res = fsm.analyze("(5 + 3");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Mismatched parentheses");
  BOOST_CHECK_EQUAL(res.error_ind, 0);
}

BOOST_AUTO_TEST_CASE(valid_with_brackets_unary_minus)
{
  FSM fsm;
  auto res = fsm.analyze("((5 + 3))");
  BOOST_CHECK(res.success);
  BOOST_CHECK_EQUAL(res.tokens.size(), 7);
  BOOST_CHECK_EQUAL(res.tokens[0].first, "(");
  BOOST_CHECK_EQUAL(res.tokens[1].first, "(");
  BOOST_CHECK_EQUAL(res.tokens[2].first, "5");
  BOOST_CHECK_EQUAL(res.tokens[3].first, "+");
  BOOST_CHECK_EQUAL(res.tokens[4].first, "3");
  BOOST_CHECK_EQUAL(res.tokens[5].first, ")");
  BOOST_CHECK_EQUAL(res.tokens[6].first, ")");

  auto res2 = fsm.analyze("-(5+3)");
  BOOST_CHECK(res2.success);
  BOOST_CHECK_EQUAL(res2.tokens.size(), 6);
  BOOST_CHECK_EQUAL(res2.tokens[0].first, "~");
  BOOST_CHECK_EQUAL(res2.tokens[1].first, "(");
  BOOST_CHECK_EQUAL(res2.tokens[2].first, "5");
  BOOST_CHECK_EQUAL(res2.tokens[3].first, "+");
  BOOST_CHECK_EQUAL(res2.tokens[4].first, "3");
  BOOST_CHECK_EQUAL(res2.tokens[5].first, ")");
}

BOOST_AUTO_TEST_CASE(invalid_with_unknown_character)
{
  FSM fsm;
  auto res = fsm.analyze("5a + 3");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Invalid character");
  BOOST_CHECK_EQUAL(res.error_ind, 1);
}

BOOST_AUTO_TEST_CASE(invalid_number_with_dot)
{
  FSM fsm;
  auto res1 = fsm.analyze("5.2.3");
  BOOST_CHECK(!res1.success);
  BOOST_CHECK_EQUAL(res1.error_msg, "Invalid number format");
  BOOST_CHECK_EQUAL(res1.error_ind, 1);

  auto res2 = fsm.analyze("5.2 | 4.3");
  BOOST_CHECK(!res2.success);
  BOOST_CHECK_EQUAL(res2.error_msg, "Invalid number format");
  BOOST_CHECK_EQUAL(res2.error_ind, 1);
}

BOOST_AUTO_TEST_CASE(invalid_start_or_end_of_expression)
{
  FSM fsm;
  auto res1 = fsm.analyze("*5+3");
  BOOST_CHECK(!res1.success);
  BOOST_CHECK_EQUAL(res1.error_msg, "Invalid expression start");
  BOOST_CHECK_EQUAL(res1.error_ind, 0);

  auto res2 = fsm.analyze(")5+3");
  BOOST_CHECK(!res2.success);
  BOOST_CHECK_EQUAL(res2.error_msg, "Unexpected closing bracket");
  BOOST_CHECK_EQUAL(res2.error_ind, 0);

  auto res3 = fsm.analyze("5+3*");
  BOOST_CHECK(!res3.success);
  BOOST_CHECK_EQUAL(res3.error_msg, "Incomplete expression");
  BOOST_CHECK_EQUAL(res3.error_ind, 3);
}

BOOST_AUTO_TEST_CASE(invalid_with_no_operator)
{
  FSM fsm;
  auto res = fsm.analyze("(5 + 3)(2 + 1)");
  BOOST_CHECK(!res.success);
  BOOST_CHECK_EQUAL(res.error_msg, "Missing operator between expressions");
  BOOST_CHECK_EQUAL(res.error_ind, 7);
}

BOOST_AUTO_TEST_CASE(invalid_with_concatenation)
{
  FSM fsm;
  auto res1 = fsm.analyze("5 | 3");
  BOOST_CHECK(res1.success);
  BOOST_CHECK_EQUAL(res1.tokens.size(), 3);
  BOOST_CHECK_EQUAL(res1.tokens[0].first, "5");
  BOOST_CHECK_EQUAL(res1.tokens[1].first, "|");
  BOOST_CHECK_EQUAL(res1.tokens[2].first, "3");

  auto res2 = fsm.analyze("5 *| 5");
  BOOST_CHECK(!res2.success);
  BOOST_CHECK_EQUAL(res2.error_msg, "Invalid operator sequence");
  BOOST_CHECK_EQUAL(res2.error_ind, 3);
}

BOOST_AUTO_TEST_SUITE_END()
