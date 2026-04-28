#include <iostream>
#include "fsm.hpp"

int main()
{
  std::string input;
  if (!std::getline(std::cin, input))
  {
    std::cerr << "Empty expression" << '\n';
    return 1;
  }

  FSM fsm;
  auto validation_result = fsm.analyze(input);
  if (!validation_result.success)
  {
    std::cerr << "at pos " << validation_result.error_ind << " system error occurred: " << validation_result.error_msg << '\n';
    return 1;
  }

  try
  {
    std::queue< std::pair< std::string, size_t > > postfix_queue = convertToPostfix(validation_result.tokens);
    long long result_value = evaluatePostfix(postfix_queue);
    std::cout << result_value << '\n';
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Evaluation error: " << e.what() << '\n';
    return 2;
  }

  return 0;
}
