#include "SimpleAsyncWorker.h"
#include <chrono>
#include <thread>
#include <iostream>

SimpleAsyncWorker::SimpleAsyncWorker(Function &callback, std::string train_path, std::string detect_path, std::string algo_type)
    : AsyncWorker(callback), train_path(train_path), detect_path(detect_path), algo_type(algo_type){};

void SimpleAsyncWorker::Execute()
{
    TimeSeries train(train_path.c_str());
    TimeSeries anomaly(detect_path.c_str());
    SimpleAnomalyDetector *detector;
    if (strcmp(algo_type.c_str(), "LinearReg") == 0)
        detector = new SimpleAnomalyDetector();
    else
        detector = new HybridAnomalyDetector();
    detector->learnNormal(train);
    anomaly_reports = detector->detect(anomaly);
};

void SimpleAsyncWorker::OnOK()
{
    // std::string msg = "SimpleAsyncWorker returning after 'working' " + algo_type;
    Napi::Array array_of_reports = Napi::Array::New(Env(), anomaly_reports.size());
    int i = 0;
    // foreach reports and create javascript objects and javascripty array
    for (AnomalyReport report : anomaly_reports) // access by reference to avoid copying
    {
        Object obj = Object::New(Env());
        obj.Set("description", report.description);
        obj.Set("timeStep", report.timeStep);
        array_of_reports[i++] = obj;
    }
    // return javascript array
    Callback().Call({Env().Null(), array_of_reports});
};