const express = require('express')
const fileUpload = require('express-fileupload')
const path = require("path")
const fs = require("fs")
const api /*var name doesn't matter*/ = require('../build/Release/ExplanationAPI')
var csv_train;
var csv_detect;

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

app.post("/upload", (req, res) => {
    // res.write('searching for ' + req.body.key + ':\n')

    if (req.files) {
        csv_train = req.files.text_train
        csv_detect = req.files.text_detect
        // upload File physically and not through link (ZERO BYTES) like const fs = require('fs');
        var text = csv_detect.data.toString()
        fs.writeFileSync(csv_detect.name, text)

        text = csv_train.data.toString()
        fs.writeFileSync(csv_train.name, text)
        res.sendStatus(200);
    } else res.sendStatus(400);

})
app.post("/detect/linear", (req, res) => {
    const detect_linear_alg = api.detectLinearAlg(csv_train.name, csv_detect.name);
    res.send(detect_linear_alg)
})
app.post("/detect/hybrid", (req, res) => {
    const detect_hybrid_alg = api.detectHybridAlg(csv_train.name, csv_detect.name);
    res.send(detect_hybrid_alg)
})
app.post("/init/linear", (req, res) => {
    const expressionString = api.initializeLinearGraphs(csv_train.name, csv_detect.name);
    res.send(expressionString)
})
app.post("/init/linear/dots", (req, res) => {
    res.send(JSON.stringify([...api.getMap(csv_train.name)]));
})

app.post("/init/hybrid/dots", (req, res) => {
    res.send(JSON.stringify([...api.getMap(csv_train.name)]));
})

app.post("/init/hybrid", (req, res) => {
    const expressionString = api.initializeHybridGraphs(csv_train.name, csv_detect.name);
    res.send(expressionString)
})

app.post("/features/linear", (req, res) => {
    const get_features = api.getLinearFeatures(csv_train.name);
    res.send(get_features)
})

app.post("/features/hybrid", (req, res) => {
    const get_features = api.getHybridFeatures(csv_train.name);
    res.send(get_features)
})

app.listen(8080)

//
// first run "node-gyp configure" and "node-gyp build" for the build and release folders to appear along with your api
// after this is done, run "node main" to run this file. Every time you change the c++ run "node-gyp build" again,
// and if you changed anything in the configurations like how many functions you want to use etc, run "node-gyp configure" again

// then you can write this require. It will NOT autofill, but do it anyway.

// example of printing what the c++ returns
