const express = require('express')
const fileUpload = require('express-fileupload')
const path = require("path")
const fs = require("fs")
const api /*var name doesn't matter*/ = require('../build/Release/ExplanationAPI')


// cpp filesystem

const app = express()
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('./view'))
app.get("/", (req, res) => {
    res.sendFile("./index.html")
})

app.post("/detect", (req, res) => {
    // res.write('searching for ' + req.body.key + ':\n')

    if (req.files) {
        var csv_train = req.files.text_train
        var csv_detect = req.files.text_detect
        var algo_type = req.body.Algorithm


        // upload File physically and not through link (ZERO BYTES) like const fs = require('fs');
        var text = csv_detect.data.toString()
        fs.writeFileSync(csv_detect.name, text)

        text = csv_train.data.toString()
        fs.writeFileSync(csv_train.name, text)

        if (algo_type === "RegLinear") {
            const detect_linear_alg = api.detectLinearAlg("anomalyTrain.csv", "anomalyTest.csv");
            const init_linear_graphs = api.initializeLinearGraphs("anomalyTrain.csv", "anomalyTest.csv");
            // console.log(detect_linear_alg)
            // console.log(init_linear_graphs)
        } else {
            const detect_hybrid_alg = api.detectHybridAlg("anomalyTrain.csv", "anomalyTest.csv");
            const init_hybrid_graphs = api.initializeHybridGraphs("anomalyTrain.csv", "anomalyTest.csv");
            // console.log(detect_hybrid_alg)
            // console.log(init_hybrid_graphs)
        }
        // test
    }
    // res.end()
})
const detect_linear_alg = api.detectLinearAlg("anomalyTrain.csv", "anomalyTest.csv");
const init_linear_graphs = api.initializeLinearGraphs("anomalyTrain.csv", "anomalyTest.csv");
const detect_hybrid_alg = api.detectHybridAlg("anomalyTrain.csv", "anomalyTest.csv");
const init_hybrid_graphs = api.initializeHybridGraphs("anomalyTrain.csv", "anomalyTest.csv");
const expressionString = "50*x+3";
app.post("/detect/linear", (req, res) => {
    res.send(detect_linear_alg)
})
app.post("/detect/hybrid", (req, res) => {
    res.send(detect_hybrid_alg)
})
app.post("/init/linear", (req, res) => {
    res.send(expressionString)
})
app.post("/init/hybrid", (req, res) => {
    res.send(expressionString)
})
// console.log("LINEAR_PART")
// console.log(detect_linear_alg)
// console.log("LINEAR_LINE")

// api.getFeatures

// return map of csv from cpp
 console.log(api.getMap("anomalyTrain.csv"))
// console.log("HYBRID_PART")
// console.log(detect_hybrid_alg)
// console.log("HYBRID_CIRCLE")

app.listen(8080)


// first run "node-gyp configure" and "node-gyp build" for the build and release folders to appear along with your api
// after this is done, run "node main" to run this file. Every time you change the c++ run "node-gyp build" again,
// and if you changed anything in the configurations like how many functions you want to use etc, run "node-gyp configure" again

// then you can write this require. It will NOT autofill, but do it anyway.

// example of printing what the c++ returns
