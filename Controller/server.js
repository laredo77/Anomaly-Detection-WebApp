const express = require('express')
const fileUpload = require('express-fileupload')
const querystring = require('querystring');
const model = require('../model/SearchInFile')

const app = express()
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('../view'))
app.get("/", (req, res) => {
    res.sendFile("./index.html")
})
app.post("/search", (req, res) => {
    res.write('searching for ' + req.body.key + ':\n')
    var key = req.body.key
    if(req.files) {
        var file = req.files.text_file
        var result = model.searchText(key, file.data.toString())
        res.write(result)
    }
    res.end()
})

app.post("/api/model", (req, res) => {
    const model_type = req.query.id;
})

app.listen(8080)