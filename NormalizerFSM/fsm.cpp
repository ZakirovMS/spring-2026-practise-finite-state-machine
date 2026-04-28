#ifndef FSM_CPP
#define FSM_CPP
#include "fsm.hpp"
#include <cctype>

InputType FSM::classify(char c, const std::string& input, size_t i) const;

Transition FSM::getTransition(State curr, InputType inp) const;

void FSM::applyAction(const Transition& tr, char c, size_t i);

bool FSM::finalize();

FSM::ValidationResult FSM::analyze(const std::string& input);

#endif