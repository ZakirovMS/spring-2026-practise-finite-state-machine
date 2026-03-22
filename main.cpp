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
    const char* startState, const char** finiteStates, int finiteStatesCount)
  {
    this->alphabet = alphabet;
    this->states = states;
    this->statesCount = statesCount;
    this->startState = startState;
    this->finiteStatesNames = finiteStates;
    this->finiteStatesCount = finiteStatesCount;
    this->currentState = nullptr;

    if (findState(startState) == nullptr)
    {
      throw std::runtime_error("Start state not found in states list");
    }
  }

  bool test(const char* value)
  {
    currentState = startState;
    const State* currentStateObj = findState(currentState);

    for (size_t i = 0; value[i] != '\0'; ++i)
    {
      char symbol = value[i];

      if (!isSymbolInAlphabet(symbol))
      {
        return false;
      }

      const char* nextState = findNextState(currentStateObj, symbol);
      if (nextState == nullptr)
      {
        return false; 
      }

      currentState = nextState;
      currentStateObj = findState(currentState);
    }
    return isFiniteState(currentState);
  }
};

int main()
{
  const char* alphabet = "01";

  Transition q0_Transitions[] =
  {
    {'0', "q0"},
    {'1', "q1"}
  };

  Transition q1_Transitions[] =
  {
    {'0', "q0"},
    {'1', "q1"}
  };

  State states[] =
  {
    {"q0", q0_Transitions, 2},
    {"q1", q1_Transitions, 2}
  };
  
  int statesCount = 2;

  const char* startState = "q0";

  const char* finiteStates[] = {"q0", "q1"};
  int finiteStatesCount = 2;

  return 0;
}
