const express = require('express')
const fileUpload = require('express-fileupload')
const Joi = require('joi');
const fs = require("fs")
const api = require('../build/Release/API')
var csv_train;
var csv_detect;

const app = express()
// important for postman - user story 2
app.use(express.json())
// dont remove the above code
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('./view'))
app.get("/", (req, res) => {
    res.sendFile("./index.html")
})

app.post("/upload", (req, res) => {
    // protection - don't remove it - maoz
    const fileSchema = Joi.object().keys({
        train: Joi.required(),
        detect: Joi.required(),
    });
    const bodySchema = Joi.object().keys({
        algo: Joi.string().required()
    });
    const body_result = bodySchema.validate(req.body);
    const files_result = fileSchema.validate(req.files);
    const body_valid = (typeof body_result.error === 'undefined')
    const files_valid = (typeof files_result.error === 'undefined')

    // the code validate the files ( empty or wrong properties )

    if (files_valid && body_valid) {
        // create files
        csv_train = req.files.train
        csv_detect = req.files.detect
        // upload File physically
        var text = csv_detect.data.toString()
        fs.writeFileSync(csv_detect.name, text)
        text = csv_train.data.toString()
        fs.writeFileSync(csv_train.name, text)

        // calculate
        if (req.body.algo.includes("linear")) {
            let detect_linear_alg = api.detectLinearAlg(csv_train.name, csv_detect.name);
            res.send(JSON.stringify(detect_linear_alg))
        } else if (req.body.algo.includes("hybrid")) {
            let detect_hybrid_alg = api.detectHybridAlg(csv_train.name, csv_detect.name);
            res.send(JSON.stringify(detect_hybrid_alg))
        } else res.sendStatus(400);
    }
    else res.sendStatus(400);
})

app.post("/detect", (req, res) => {
    if (req.files) {
        csv_train = req.files.text_train
        csv_detect = req.files.text_detect
        // upload File physically
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