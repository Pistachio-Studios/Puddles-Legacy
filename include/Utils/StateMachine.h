#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "Utils/DynArray.h"
#include "Utils/Log.h"
#include "Utils/SString.h"
#include <vector>

template <typename T>
class State;

template <typename T>
class StateMachine {
public:
    StateMachine(T* owner) : owner(owner){
    }

    ~StateMachine(){
    }

    void PreUpdate(){
        currentState->PreUpdate();
    }

    void Update(float dt){
        currentState->Update(dt);
    }

    void PostUpdate(){
        currentState->PostUpdate();
    }

    void CleanUp(){
        for (int i = 0; i < states.Count(); i++){
            states[i]->Exit();
            delete states[i];
        }
        states.Clear();
    }

    void AddState(State<T>* newState){
        newState->StateMachineReference = this;
        
        if (currentState == nullptr){
            currentState = newState;
            currentState->Enter();
        }
        states.PushBack(newState);
    }

State<T>* FindState(SString stateName){
    for (int i = 0; i < states.Count(); i++){
        if (states[i]->name == stateName){
            return states[i];
        }
    }
    return nullptr;
}

void ChangeState(SString stateName){
    currentState->Exit();
    State<T>* newState = FindState(stateName);
    if (newState != nullptr){
        currentState = newState;
    }
    else{
        LOG("State %s not found\n", stateName.GetString());
    }
    currentState->Enter();
}

State<T> GetCurrentState(){
    return *currentState;
}

public:
    T* owner;
private:
    State<T>* currentState = nullptr;
    DynArray<State<T>*> states; //ver si tienen que ser punteros o no
};
#endif // __STATEMACHINE_H__