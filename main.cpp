#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstddef>

struct Transition
{
  char symbol;
  const char* toState;
};

struct State
{
  const char* name;
  const Transition* transitions;
  int transitionsCount;
};

class FSM
{
private:
  const char* alphabet;
  const State* states;
  int statesCount;
  const char* startState;
  const char** finiteStatesNames;
  int finiteStatesCount;
  const char* currentState;
public:
  FSM(const char* alphabet, const State* states, int statesCount,
    const char* startState, const char** finiteStates, int finiteStatesCount);
  bool test(const char* value);
};

int main()
{

  return 0;
}
