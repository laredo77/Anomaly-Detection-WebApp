{
    "libraries": [
    # "headerThatYourCPPusesAtSomePointEvenIndirectly .h",
    "anomaly_detection_util.h",
    "timeseries.h",
    "HybridAnomalyDetector.h",
    "minCircle.h",
    "SimpleAnomalyDetector.h",
    "AnomalyDetector.h",
    "timeseries.h"
    ],
  "targets": [
    {
      "target_name": "ExplanationAPI",
      "sources": [ "./Model/cpp/ExplanationAPI.cpp" ] #file location
    }
  ]
}