const express = require('express')
const fileUpload = require('express-fileupload')
const fs = require('fs');
const path = require("path");


// cpp algo
//const runWorker = require('../Model/cpp/build/Release/AsyncWorker');

const app = express()
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('../view'))
app.get("/", (req, res) => {
    res.sendFile("./index.html")
})
// app.post("/detect", (req, res) => {
//     // res.write('searching for ' + req.body.key + ':\n')
//
//     if (req.files) {
//         var csv_train = req.files.text_train
//         var csv_detect = req.files.text_detect
//         var algo_type = req.body.Algorithm
//
//
//         // write csv file
//         fs.writeFile(csv_train.name, csv_train.data.toString(), function (err) {
//             if (err)
//                 return console.log(err);
//             //console.log(path.resolve(csv_train.name));
//         });
//         // write second csv file
//         fs.writeFile(csv_detect.name, csv_detect.data.toString(), function (err) {
//             if (err)
//                 return console.log(err);
//             //console.log(path.resolve(csv_detect.name));
//         });
//         result = runWorker.runSimpleAsyncWorker(algo_type, path.resolve(csv_train.name), path.resolve(csv_detect.name), AsyncWorkerCompletion);
//         console.log("runSimpleAsyncWorker returned '" + result + "'.");
//     }
//     // res.end()
// })

app.post("/api/model", (req, res) => {
    const model_type = req.query.id;
})

//app.listen(8080)



// function AsyncWorkerCompletion(err, result) {
//     if (err) {
//         console.log("SimpleAsyncWorker returned an error: ", err);
//     } else {
//         console.log("SimpleAsyncWorker returned '" + result + "'.");
//         // data.push(result[0]);
//         // console.log(JSON.stringify(result));
//     }
// };

// first run "node-gyp configure" and "node-gyp build" for the build and release folders to appear along with your api
// after this is done, run "node main" to run this file. Every time you change the c++ run "node-gyp build" again,
// and if you changed anything in the configurations like how many functions you want to use etc, run "node-gyp configure" again

// then you can write this require. It will NOT autofill, but do it anyway.
const api /*var name doesn't matter*/ = require('../build/Release/ExplanationAPI') //the name of your .node file in Release, according to your binding.gyp

// example of printing what the c++ returns
const str1 = api.nameIwantInJS("anomalyTrain.csv", "anomalyTest.csv");
console.log(str1)
//console.log(api.nameIwantInJS("anomalyTrain.csv", "anomalyTest.csv"))

//console.log(api.getAnomalyPoints("anomalyTrain.csv", "anomalyTest.csv"))

// let variableName = api.otherNameForFunctionInJS() //example of storing return val and printing later
// console.log(variableName)

// let tryAlg = api.run("anomalyTrain.csv")
// console.log(tryAlg)
