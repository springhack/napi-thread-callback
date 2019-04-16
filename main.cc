/*
 *  Author: SpringHack - springhack@live.cn
 *  Last modified: 2019-04-17 01:24:19
 *  Filename: main.cc
 *  Description: Created by SpringHack using vim automatically.
 */
#include "napi.h"
#include "thread_callback.h"

#include <thread>
#include <chrono>
#include <ctime>


using namespace Napi;

FunctionReference js_cb;

void time_tick() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    triggerCallback();
  }
}

void JSCallbackInvoker(const CallbackInfo& info) {
  Env env = info.Env();
  time_t now = time(0);
  char* dt = ctime(&now);
  js_cb.Call({
    String::New(env, dt)
  });
}

Value StartWork(const CallbackInfo& info) {
  // Get env
  Env env = info.Env();
  // Store origin js callback
  js_cb = Persistent(info[0].As<Function>());
  // Setup our invoker as callback
  Function cb = Function::New(env, JSCallbackInvoker);
  setCallback(env, cb);
  // Start worker thread
  std::thread(time_tick).detach();
  // Return undefined
  return env.Undefined();
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "startWork"), Function::New(env, StartWork));
  return exports;
}

NODE_API_MODULE(napi_callback_thread, Init)
