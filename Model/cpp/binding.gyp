{
  "targets": [
    {
      "target_name": "AsyncWorker",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "./runSimpleAsyncWorker.cpp",
        "./SimpleAsyncWorker.cpp",
        "./anomaly_detection_util.cpp",
        "./HybridAnomalyDetector.cpp",
        "./SimpleAnomalyDetector.cpp",
        "./timeseries.cpp",
        "./minCircle.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}