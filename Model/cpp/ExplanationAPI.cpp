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

    void InitializeLinearGraphs(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate(); // just do if you need isolate somewhere like NewFromUTF8, see below
        string csvFileName = *v8::String::Utf8Value(args[0]); // get first argument sent from JS and converts it to string we can use
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();
        string str = "\n";

        for (int i = 0; i < cf.size(); i++) {
            if (cf[i].corrlation > 0.9) {
                str += cf[i].feature1;
                str += "-";
                str += cf[i].feature2;
                str += "\nlinear reg\n";
                str += to_string(cf[i].lin_reg.a);
                str += ",";
                str += to_string(cf[i].lin_reg.b);
                str += "\n";
            }
        }
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
    }

    void DetectHybridAlg(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        string csvFileName2 = *v8::String::Utf8Value(args[1]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();
        TimeSeries ts2(csvFileName2);
        vector<AnomalyReport> r = ad.detect(ts2);
        string str = "\n";

        for (int i = 0; i < r.size(); i++) {
            str += r[i].description;
            str += "\t";
            str += to_string(r[i].timeStep);
            str += "\n";
        }

        str += "anomaly points:\n";
        vector<std::string> anomaly_points = ad.getAnomalyPoints();
        for (int i = 0; i < anomaly_points.size(); i++) {
            str += anomaly_points[i];
            str += "\n";
        }
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
    }


    void DetectLinearAlg(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        string csvFileName2 = *v8::String::Utf8Value(args[1]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormalLinear(ts);
        TimeSeries ts2(csvFileName2);
        vector<AnomalyReport> r = ad.detectLinear(ts2);
        string str = "\n";

        for (int i = 0; i < r.size(); i++) {

            str += r[i].description;
            str += "\t";
            str += to_string(r[i].timeStep);
            str += "\n";
        }

        str += "anomaly points:\n";
        vector<std::string> anomaly_points = ad.getAnomalyPoints();
        for (int i = 0; i < anomaly_points.size(); i++) {
            str += anomaly_points[i];
            str += "\n";
        }
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
    }

    // Initialize write exactly as is, NODE_SET_METHOD have the 2nd arg be the name you want to use in JS and the 3rd arg is the function here
    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "initializeHybridGraphs", InitializeHybridGraphs);
        NODE_SET_METHOD(exports, "initializeLinearGraphs", InitializeLinearGraphs);
        NODE_SET_METHOD(exports, "detectHybridAlg", DetectHybridAlg);
        NODE_SET_METHOD(exports, "detectLinearAlg", DetectLinearAlg);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize); // this needs to be at the end, don't touch


 }