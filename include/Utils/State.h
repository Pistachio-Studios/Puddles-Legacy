#ifndef __STATE_H__
#define __STATE_H__
#include "Utils/SString.h"
#include "Utils/StateMachine.h"

template <typename T>
class State {
public:
    SString name;
    StateMachine<T>* StateMachineReference;

public:
    State() {}
    State(SString name) : name(name) {}
    virtual void Enter() {}
    virtual void PreUpdate() {}
    virtual void Update(float dt) {}
    virtual void PostUpdate() {}
    virtual void Exit() {}
};
#endif // __STATE_H__