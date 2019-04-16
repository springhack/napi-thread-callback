/*
 *  Author: SpringHack - springhack@live.cn
 *  Last modified: 2019-04-17 00:58:30
 *  Filename: thread_callback.h
 *  Description: Created by SpringHack using vim automatically.
 */
#include "napi.h"


void triggerCallback();
void setCallback(Napi::Env& env, Napi::Function& cb); 
