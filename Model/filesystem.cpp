#include <fstream>
#include <iostream>
#include <node.h>

 namespace whateverNamespace {
    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Value;
    using v8::Array;
    using v8::Exception;
    using namespace std; //for the strings
void uploadFile(const FunctionCallbackInfo<Value>&args) {
    Isolate* isolate = args.GetIsolate();
    string line = *v8::String::Utf8Value(args[0]);
    string csvFileName = *v8::String::Utf8Value(args[1]);

    fstream outfile;
    outfile.open(csvFileName,ios::app);
    if(!outfile)  cout << "failed";
    outfile << line << endl;

};

// remove all
void removeFile(const FunctionCallbackInfo<Value>&args) {
    Isolate* isolate = args.GetIsolate();
    string csvFileName = *v8::String::Utf8Value(args[1]);
    remove(csvFileName.c_str());
};


    // Initialize write exactly as is, NODE_SET_METHOD have the 2nd arg be the name you want to use in JS and the 3rd arg is the function here
    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "removeFile", removeFile);
        NODE_SET_METHOD(exports, "uploadFile", uploadFile);
        //NODE_SET_METHOD(exports, "run", BuildFunction);
    }

   NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize); // this needs to be at the end, don't touch
}