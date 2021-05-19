const express = require('express')
const fileUpload = require('express-fileupload')
const path = require("path");


// cpp filesystem

const app = express()
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('../view'))
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
        var lines = text.match(/[^\r\n]+/g)
        filesystem.removeFile(csv_detect.name)
        lines.forEach(element => filesystem.uploadFile(element,csv_detect.name))

        text = csv_train.data.toString()
        lines = text.match(/[^\r\n]+/g)
        filesystem.removeFile(csv_train.name)
        lines.forEach(element => filesystem.uploadFile(element,csv_train.name))

        // test
    }
    // res.end()
})


app.post("/api/model", (req, res) => {
    const model_type = req.query.id;
})

//app.listen(8080)




// ITAMAR PART CUZ THERES NO .NODE FILE  

// first run "node-gyp configure" and "node-gyp build" for the build and release folders to appear along with your api
// after this is done, run "node main" to run this file. Every time you change the c++ run "node-gyp build" again,
// and if you changed anything in the configurations like how many functions you want to use etc, run "node-gyp configure" again

// then you can write this require. It will NOT autofill, but do it anyway.
const api /*var name doesn't matter*/ = require('../build/Release/ExplanationAPI') //the name of your .node file in Release, according to your binding.gyp

// example of printing what the c++ returns
const init_hybrid_graphs = api.initializeHybridGraphs("anomalyTrain.csv", "anomalyTest.csv");
const init_linear_graphs = api.initializeLinearGraphs("anomalyTrain.csv", "anomalyTest.csv");
const detect_hybrid_alg = api.detectHybridAlg("anomalyTrain.csv", "anomalyTest.csv");
const detect_linear_alg = api.detectLinearAlg("anomalyTrain.csv", "anomalyTest.csv");

// to print the data like below
console.log(detect_hybrid_alg)

