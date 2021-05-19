// #include "every cpp you need, NOT the h .cpp"
#include <node.h>
#include <string>
#include <vector>
#include "HybridAnomalyDetector.cpp"
#include "anomaly_detection_util.cpp"
#include "minCircle.cpp"
#include "SimpleAnomalyDetector.cpp"
#include "timeseries.cpp"

 //just put in all these usings, they're for node to use.
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

    void InitializeHybridGraphs(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate(); // just do if you need isolate somewhere like NewFromUTF8, see below
        string csvFileName = *v8::String::Utf8Value(args[0]); // get first argument sent from JS and converts it to string we can use
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();
        string str = "\n";

        for (int i = 0; i < cf.size(); i++) {
            str += cf[i].feature1;
            str += "-";
            str += cf[i].feature2;
            if (cf[i].corrlation > 0.9) {
                str += "\nlinear reg\n";
                str += to_string(cf[i].lin_reg.a);
                str += ",";
                str += to_string(cf[i].lin_reg.b);
                str += "\n";
            }
            else if (cf[i].corrlation > 0.5) {
                str += "\nmin circle\n";
                str += to_string(cf[i].x);
                str += ",";
                str += to_string(cf[i].y);
                str += ",";
                str += to_string(cf[i].radius);
                str += "\n";
            }
        }
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
    }

    void WhateverFunction(const FunctionCallbackInfo<Value>&args) /*needs to have exactly that as the argument*/ { 
//        Isolate* isolate = args.GetIsolate(); // just do if you need isolate somewhere like NewFromUTF8, see below
//
//        string someString = *v8::String::Utf8Value(args[0]); // get first argument sent from JS and converts it to string we can use
//
//        int someInt = args[1]->Int32Value(); // how to receive an int from JS argument
//
//        string str = "test C++ string";
//        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str())); // how you send a c++ string back to JS
//        // args.GetReturnValue().Set(String::NewFromUtf8(isolate, "{}")); if I wanted to return "{}" back to JS
        Isolate* isolate = args.GetIsolate(); // just do if you need isolate somewhere like NewFromUTF8, see below
        string csvFileName = *v8::String::Utf8Value(args[0]); // get first argument sent from JS and converts it to string we can use
    	string csvFileName2 = *v8::String::Utf8Value(args[1]);
    	TimeSeries ts(csvFileName);
    	HybridAnomalyDetector ad;
    	ad.learnNormal(ts);
    	vector<correlatedFeatures> cf=ad.getNormalModel();
    	TimeSeries ts2(csvFileName2);
        vector<AnomalyReport> r = ad.detect(ts2);
        string str = "\n";
        vector<std::string> anomaly_points = ad.getAnomalyPoints();
        int pos = 0;
        int counter = 0;
        int i;
        for (i = 0; i < r.size(); i++) {
            if (i == 0) {
                str += r[i].description;
                counter++;
            }
            else if(r[i].description == r[i - 1].description) {
                counter++;
            }
            else {
                str += "\n";
                for (int j = pos; j < i; j++) {
                    str += anomaly_points[j];
                    str += "\n";
                }
                str += "\n";
                str += r[i].description;
                pos += counter;
                counter = 0;
            }
        }
        str += "\n";
        pos ++;
        for (int j = pos; j < i; j++) {
            str += anomaly_points[j];
            str += "\n";
        }

//        vector<std::string> anomaly_points = ad.getAnomalyPoints();
//        string points_str = "anomaly points:\n";
//        for (int j = 0; j < anomaly_points.size(); j++) {
//            points_str += anomaly_points[j];
//            points_str += "\n";
//        }
//        str += points_str;
//        str += "Linear reg:\n";


        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str())); // how you send a c++ string back to JS
        //args.GetReturnValue().Set(String::NewFromUtf8(isolate, points_str.c_str()));
    }

    void GetAnomalyPoints(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate(); // just do if you need isolate somewhere like NewFromUTF8, see below
        string csvFileName = *v8::String::Utf8Value(args[0]); // get first argument sent from JS and converts it to string we can use
    	string csvFileName2 = *v8::String::Utf8Value(args[1]);
    	TimeSeries ts(csvFileName);
    	HybridAnomalyDetector ad;
    	ad.learnNormal(ts);
    	vector<correlatedFeatures> cf=ad.getNormalModel();
    	TimeSeries ts2(csvFileName2);
        ad.detect(ts2);
        string str = "Anomaly points: \n";
        vector<std::string> anomaly_points = ad.getAnomalyPoints();

        for (int i = 0; i < anomaly_points.size(); i++) {
            str += anomaly_points[i];
            str += "\n";
        }

        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
    }

    void WhateverFunction2(const FunctionCallbackInfo<Value>&args) {
        /* example of how to use 2 functions from js */
        Isolate* isolate = args.GetIsolate();
        int x = 9876;
        auto total = Number::New(isolate, x);
        args.GetReturnValue().Set(total);
    }

    // Initialize write exactly as is, NODE_SET_METHOD have the 2nd arg be the name you want to use in JS and the 3rd arg is the function here
    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "nameIwantInJS", WhateverFunction);
        NODE_SET_METHOD(exports, "otherNameForFunctionInJS", WhateverFunction2);
        NODE_SET_METHOD(exports, "getAnomalyPoints", GetAnomalyPoints);
        NODE_SET_METHOD(exports, "initializeHybridGraphs", InitializeHybridGraphs);
        //NODE_SET_METHOD(exports, "run", BuildFunction);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize); // this needs to be at the end, don't touch


 }