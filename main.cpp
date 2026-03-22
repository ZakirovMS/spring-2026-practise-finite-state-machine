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

  const State* findState(const char* name)
  {
    for (size_t i = 0; i < statesCount; ++i)
    {
      if (std::strcmp(states[i].name, name) == 0)
      {
        return &states[i];
      }
    }
    return nullptr;
  }

  bool isSymbolInAlphabet(char symbol)
  {
    for (size_t i = 0; alphabet[i] != '\0'; ++i)
    {
      if (alphabet[i] == symbol)
      {
        return true;
      }
    }
    return false;
  }

  const char* findNextState(const State* currentState, char symbol)
  {
    for (size_t i = 0; i < currentState->transitionsCount; ++i)
    {
      if (currentState->transitions[i].symbol == symbol)
      {
        return currentState->transitions[i].toState;
      }
    }
    return nullptr;
  }

  bool isFiniteState(const char* stateName)
  {
    for (size_t i = 0; i < finiteStatesCount; ++i)
    {
      if (std::strcmp(finiteStatesNames[i], stateName) == 0)
      {
        return true;
      }
    }
    return false;
  }

public:
  FSM(const char* alphabet, const State* states, int statesCount,
    const char* startState, const char** finiteStates, int finiteStatesCount);
  bool test(const char* value);
};

int main()
{

  return 0;
}
