#include <node.h>
#include <string>
#include <vector>
#include "HybridAnomalyDetector.cpp"
#include "anomaly_detection_util.cpp"
#include "minCircle.cpp"
#include "SimpleAnomalyDetector.cpp"
#include "timeseries.cpp"

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
    using v8::HandleScope;
    using v8::Map;
    using namespace std; //for the strings

    void InitializeHybridGraphs(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();

        // create array
        Local<Array> points = Array::New(isolate);

        for (int i = 0; i < cf.size(); i++) {
            if (isfinite(cf[i].corrlation) && cf[i].corrlation > 0.9) {
                Local<Object> correlated_point = Object::New(isolate);
                // create a and b
                string a(to_string(cf[i].lin_reg.a));
                string b(to_string(cf[i].lin_reg.b));
                if (!isfinite(std::stod(a)) || !isfinite(std::stod(b)))
                    break;
                // type
                correlated_point->Set(String::NewFromUtf8(isolate, "type"), String::NewFromUtf8(isolate, "line"));
                // create point
                Local<Object> point = Object::New(isolate);
                // set point
                point->Set(String::NewFromUtf8(isolate, "description"), String::NewFromUtf8(isolate,(cf[i].feature1 + "," + cf[i].feature2).c_str()));
                if (std::stod(b) < 0)
                    point->Set(String::NewFromUtf8(isolate, "expression"), String::NewFromUtf8(isolate, (a+"*x"+b).c_str()));
                else
                    point->Set(String::NewFromUtf8(isolate, "expression"), String::NewFromUtf8(isolate, (a+"*x"+"+"+b).c_str()));
                correlated_point->Set(String::NewFromUtf8(isolate, "point"), point);
                points->Set(i, correlated_point);
            }
            else if (isfinite(cf[i].corrlation) && cf[i].corrlation > 0.5) {
                if (isfinite(cf[i].x) && isfinite(cf[i].y) && isfinite(cf[i].radius)) {
                    Local<Object> correlated_point = Object::New(isolate);
                    correlated_point->Set(String::NewFromUtf8(isolate, "type"), String::NewFromUtf8(isolate, "circle"));
                    // create point
                    Local<Object> point = Object::New(isolate);
                    // set point
                    point->Set(String::NewFromUtf8(isolate, "description"), String::NewFromUtf8(isolate,(cf[i].feature1 + "," + cf[i].feature2).c_str()));
                    point->Set(String::NewFromUtf8(isolate, "xPlus"), Number::New(isolate, cf[i].x + cf[i].radius));
                    point->Set(String::NewFromUtf8(isolate, "yPlus"), Number::New(isolate, cf[i].y + cf[i].radius));
                    point->Set(String::NewFromUtf8(isolate, "xMinus"), Number::New(isolate, cf[i].x - cf[i].radius));
                    point->Set(String::NewFromUtf8(isolate, "yMinus"), Number::New(isolate, cf[i].y - cf[i].radius));
                    // create point with data
                    correlated_point->Set(String::NewFromUtf8(isolate, "point"), point);
                    points->Set(i, correlated_point);
                }
            }
        }
        args.GetReturnValue().Set(points);
    }

    void InitializeLinearGraphs(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormalLinear(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();

        // create array
        Local<Array> points = Array::New(isolate);

        for (int i = 0; i < cf.size(); i++) {
            Local<Object> correlated_point = Object::New(isolate);
            // create a and b
            string a(to_string(cf[i].lin_reg.a));
            string b(to_string(cf[i].lin_reg.b));
            if (!isfinite(std::stod(a)) || !isfinite(std::stod(b)))
                break;
            // type
            correlated_point->Set(String::NewFromUtf8(isolate, "type"), String::NewFromUtf8(isolate, "line"));
            // create point
            Local<Object> point = Object::New(isolate);
            // set point
            point->Set(String::NewFromUtf8(isolate, "description"), String::NewFromUtf8(isolate,(cf[i].feature1 + "," + cf[i].feature2).c_str()));
            if(std::stof(b) < 0)
                point->Set(String::NewFromUtf8(isolate, "expression"), String::NewFromUtf8(isolate, (a+"*x"+b).c_str()));
            else
                point->Set(String::NewFromUtf8(isolate, "expression"), String::NewFromUtf8(isolate, (a+"*x"+"+"+b).c_str()));
            correlated_point->Set(String::NewFromUtf8(isolate, "point"), point);
            points->Set(i, correlated_point);

        }
        args.GetReturnValue().Set(points);
    }

    void DetectHybridAlg(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        string csvFileName2 = *v8::String::Utf8Value(args[1]);

        // initialize graph
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        TimeSeries ts2(csvFileName2);
        vector<AnomalyReport> r = ad.detect(ts2);
        vector<Point> anomaly_points = ad.getAnomalyPoints();

        // create javascript array of AnomalyReport
        Local<Array> reports = Array::New(isolate);

        // create objects
        for (int i = 0; i < r.size() && i < anomaly_points.size(); i++) {
            if(isfinite(anomaly_points[i].x) && isfinite(anomaly_points[i].y) && isfinite(r[i].timeStep)) {
                Local<Object> report = Object::New(isolate);
                report->Set(String::NewFromUtf8(isolate, "description"), String::NewFromUtf8(isolate, r[i].description.c_str()));
                report->Set(String::NewFromUtf8(isolate, "timeStep"), Number::New(isolate, r[i].timeStep));
                Local<Object> point = Object::New(isolate);
                point->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, anomaly_points[i].x));
                point->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, anomaly_points[i].y));
                report->Set(String::NewFromUtf8(isolate, "Point"), point);
                reports->Set(i, report);
            }
        }

        args.GetReturnValue().Set(reports);
    }

    void DetectLinearAlg(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        string csvFileName2 = *v8::String::Utf8Value(args[1]);

        // initialize graph
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormalLinear(ts);
        TimeSeries ts2(csvFileName2);
        vector<AnomalyReport> r = ad.detectLinear(ts2);
        vector<Point> anomaly_points = ad.getAnomalyPoints();

        // create javascript array of AnomalyReport
        Local<Array> reports = Array::New(isolate);

        // create objects
        for (int i = 0; i < r.size() && i < anomaly_points.size(); i++) {
            if(isfinite(anomaly_points[i].x) && isfinite(anomaly_points[i].y) && isfinite(r[i].timeStep)) {
            Local<Object> report = Object::New(isolate);
            report->Set(String::NewFromUtf8(isolate, "description"), String::NewFromUtf8(isolate, r[i].description.c_str()));
            report->Set(String::NewFromUtf8(isolate, "timeStep"), Number::New(isolate, r[i].timeStep));
            Local<Object> point = Object::New(isolate);
            point->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, anomaly_points[i].x));
            point->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, anomaly_points[i].y));
            report->Set(String::NewFromUtf8(isolate, "Point"), point);
            reports->Set(i, report);
            }
        }
        args.GetReturnValue().Set(reports);
    }

    void GetLinearFeatures(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormalLinear(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();
        Local<Array> features = Array::New(isolate);

        for (int i = 0; i < cf.size(); i++) {
            features->Set(i, String::NewFromUtf8(isolate, (cf[i].feature1+","+cf[i].feature2).c_str()));
        }
        args.GetReturnValue().Set(features);
    }

    void GetHybridFeatures(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        TimeSeries ts(csvFileName);
        HybridAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf=ad.getNormalModel();

        Local<Array> features = Array::New(isolate);

        for (int i = 0; i < cf.size(); i++) {
            features->Set(i, String::NewFromUtf8(isolate, (cf[i].feature1+","+cf[i].feature2).c_str()));
        }
        args.GetReturnValue().Set(features);
    }

    void GetMap(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        string csvFileName = *v8::String::Utf8Value(args[0]);
        TimeSeries ts(csvFileName);

        map<std::string, vector<float>> map = ts.get_data_structure();
        Local<Map> map_js = Map::New(isolate);

        // create MAP OF JAVASCRIPT FROM CPP MAP BY V8
        for (const auto& value : map) {
            // create array
            Local<Array> array_js = Array::New(isolate);
            // from vector

            for (int i = 0; i < value.second.size(); i++) {
                if (finite(value.second[i])) {
                    array_js->Set(i, Number::New(isolate, value.second[i]));
                }
            }
            // add value to javascript map
            map_js->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate,value.first.c_str()), array_js);
        }
        // return map
        args.GetReturnValue().Set(map_js);
    }

    void Initialize(Local<Object> exports) {

        NODE_SET_METHOD(exports, "initializeHybridGraphs", InitializeHybridGraphs);
        NODE_SET_METHOD(exports, "initializeLinearGraphs", InitializeLinearGraphs);
        NODE_SET_METHOD(exports, "detectHybridAlg", DetectHybridAlg);
        NODE_SET_METHOD(exports, "detectLinearAlg", DetectLinearAlg);
        NODE_SET_METHOD(exports, "getLinearFeatures", GetLinearFeatures);
        NODE_SET_METHOD(exports, "getHybridFeatures", GetHybridFeatures);
        NODE_SET_METHOD(exports, "getMap", GetMap);
    }
    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize); // this needs to be at the end, don't touch
 }