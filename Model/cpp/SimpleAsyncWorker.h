#pragma once
#include <napi.h>
#include "SimpleAsyncWorker.h"
#include "minCircle.h"
#include "timeseries.h"
#include "./HybridAnomalyDetector.h"

using std::string;
using namespace Napi;

class SimpleAsyncWorker : public AsyncWorker {
    
    public:
        SimpleAsyncWorker(Function& callback, string train_path, string detect_path, string algo_type);
        virtual ~SimpleAsyncWorker() {};

        void Execute();
        void OnOK();

    private:
        string train_path;
        string detect_path;
        string algo_type;
        std::vector<AnomalyReport> anomaly_reports;
};