/*
 *  Author: SpringHack - springhack@live.cn
 *  Last modified: 2019-04-17 01:23:57
 *  Filename: thread_callback.cc
 *  Description: Created by SpringHack using vim automatically.
 */
#include "napi.h"
#include "thread_callback.h"

#include <thread>
#include <mutex>


std::mutex js_mutex;
std::condition_variable js_cond;

using namespace Napi;

FunctionReference host_callback;
void noop(const CallbackInfo& info) {}

class ThreadCallbackWorker : public AsyncWorker {
private:
  void Execute() override {
    std::unique_lock<std::mutex> lock(js_mutex);
    js_cond.wait(lock);
  }
  void OnOK() override {
    HandleScope scope(Env());
    host_callback.Call({});
    ThreadCallbackWorker::StartWork(Env());
  }
public:
  ThreadCallbackWorker(Function& cb, const char* resource_name) : AsyncWorker(cb, resource_name) {}
  static void StartWork(Napi::Env&& env) {
    Function _noop = Function::New(env, noop);
    ThreadCallbackWorker* worker = new ThreadCallbackWorker(_noop, "ThreadCallbackWorker");
    worker->Queue();
  }
};


void setCallback(Env& env, Function& cb) {
  host_callback = Persistent(cb);
  ThreadCallbackWorker::StartWork(std::move(env));
}

void triggerCallback() {
  js_cond.notify_all();
}
