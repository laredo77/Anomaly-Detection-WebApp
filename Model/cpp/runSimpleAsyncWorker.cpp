#include "SimpleAsyncWorker.h"

Value runSimpleAsyncWorker(const CallbackInfo& info) {
    std::string algo_type = info[0].As<String>();
    std::string csv_train = info[1].As<String>();
    std::string csv_detect = info[2].As<String>();
    Function callback = info[3].As<Function>();
    SimpleAsyncWorker* asyncWorker = new SimpleAsyncWorker(callback,csv_train,csv_detect,algo_type);
    asyncWorker->Queue();
    std::string msg = "SimpleAsyncWorker for " + algo_type;
    return String::New(info.Env(),msg.c_str());
};

Object Init(Env env, Object exports) {
    exports["runSimpleAsyncWorker"] = Function::New(env, runSimpleAsyncWorker, std::string("runSimpleAsyncWorker"));
    return exports;
}

NODE_API_MODULE(addon, Init)