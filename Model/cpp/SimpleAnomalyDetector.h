/*
 * SimpleAnomalyDetector.h
 *
 * Author: 311547087, Itamar Laredo
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float x;
    float y;
    float radius;
    vector<Point> correlated_points;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
    vector<AnomalyReport> v_ar;
    vector<Point> anomaly_points;
    //vector<Point> correlated_points;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);
    virtual void learnNormalLinear(const TimeSeries &ts);
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    virtual vector<AnomalyReport> detectLinear(const TimeSeries &ts);

    // correlated features getter
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    // anomaly reports getter
    vector<AnomalyReport> getVar() {
        return v_ar;
    }

    vector<Point> getAnomalyPoints() {
        return anomaly_points;
    }

//    vector<Point> getCorrelatedPoints() {
//        return correlated_points;
//    }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
