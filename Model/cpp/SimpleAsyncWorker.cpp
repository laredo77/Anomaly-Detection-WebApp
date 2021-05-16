#include "SimpleAsyncWorker.h"
#include <chrono>
#include <thread>

SimpleAsyncWorker::SimpleAsyncWorker(Function& callback, int runTime)
    : AsyncWorker(callback), runTime(runTime) {};

void SimpleAsyncWorker::Execute() {
    if (runTime == 4) { SetError ("Oops! Failed before 'working' 4 seconds."); return; }
    std::this_thread::sleep_for(std::chrono::seconds(runTime));
};

void SimpleAsyncWorker::OnOK() {
    std::string msg = "SimpleAsyncWorker returning after 'working' " + std::to_string(runTime) + " seconds.";
    // test
    std::string cars[4] = {"Volvo", "BMW", "Ford", "Mazda"};
    // Callback().Call({Env().Null(), String::New(Env(), msg)});

    Napi::Array array_of_cars = Napi::Array::New(Env(), 4);
    int i = 0;
    Object obj = Object::New(Env());

    obj.Set("description", "A");
    obj.Set("timeStep",uint32_t(42));
    array_of_cars[i++] =  obj;
    array_of_cars[i++] = String::New(Env(), "BMW");
    array_of_cars[i++] = String::New(Env(), "Ford");
    array_of_cars[i++] = String::New(Env(), "Mazda");
    Callback().Call({Env().Null(), array_of_cars});
};