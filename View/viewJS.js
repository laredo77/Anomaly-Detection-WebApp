// const delay = ms => new Promise(resolve => setTimeout(resolve, ms))
// points of anomalies in linear reg
var isLinear = 0;

function setIsLinear(value) {
    if (value == 0) {
        isLinear = 0;
    } else
        isLinear = 1;
}

function result(feature) {
    if (isLinear == 0) {
        resultHybrid(feature);
    } else {
        resultLinear(feature);
    }
}

function resultLinear(feature) {
    var trace1;
    var traceInit;
    var layout;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            var layout = {
                title: 'Linear Reg Algorithm'
            };
            var xArray = [];
            var yArray = [];
            const dataFromServer = JSON.parse(this.response);
            for (let i = 0; i < dataFromServer.length; i++) {
                if (dataFromServer[i].description === feature) {
                    xArray.push(dataFromServer[i].Point.x);
                    yArray.push(dataFromServer[i].Point.y);
                }
            }
            trace1 = {
                x: xArray,
                y: yArray,
                mode: 'markers'
            };

        }
    };
    xhttp.open("POST", "/detect/linear", true);
    xhttp.send()
    var xhttp2 = new XMLHttpRequest();
    xhttp2.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            try {
                const expr = math.compile(this.response)
                //instead of the numbers need to put minimum value and maximum
                const xValues = math.range(0, 10, 0.5).toArray()
                const yValues = xValues.map(function (x) {
                    return expr.evaluate({x: x})
                })
                traceInit = {
                    x: xValues,
                    y: yValues,
                    type: 'scatter'
                }
                var data = [trace1, traceInit];
                Plotly.newPlot('myDiv', data, layout)
            } catch (err) {
                console.error(err)
                alert(err)
            }
        }
    };
    xhttp2.open("POST", "/init/linear", true);
    xhttp2.send()
}

// points of anomalies in Hybrid
function resultHybrid(feature) {
    var trace1;
    var traceInit;
    var layout;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            var layout = {
                title: 'Linear Reg Algorithm'
            };
            var xArray = [];
            var yArray = [];
            const dataFromServer = JSON.parse(this.response);
            for (let i = 0; i < dataFromServer.length; i++) {
                if (dataFromServer[i].description === feature) {
                    xArray.push(dataFromServer[i].Point.x);
                    yArray.push(dataFromServer[i].Point.y);
                }
            }
            trace1 = {
                x: xArray,
                y: yArray,
                mode: 'markers'
            };

        }
    };
    xhttp.open("POST", "/detect/hybrid", true);
    xhttp.send()
    var xhttp2 = new XMLHttpRequest();
    xhttp2.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            try {
                const expr = math.compile(this.response)
                //instead of the numbers need to put minimum value and maximum
                const xValues = math.range(0, 100, 0.5).toArray()
                const yValues = xValues.map(function (x) {
                    return expr.evaluate({x: x})
                })
                traceInit = {
                    x: xValues,
                    y: yValues,
                    type: 'scatter'
                }
                var data = [trace1, traceInit];
                Plotly.newPlot('myDiv', data, layout)
            } catch (err) {
                console.error(err)
                alert(err)
            }
        }
    };
    xhttp2.open("POST", "/init/hybrid", true);
    xhttp2.send()
}

function generateFeatures() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            // array of features sent from the server
            var values = JSON.parse(this.response);
            var featuresSelect = document.createElement("select");
            featuresSelect.name = "features";
            featuresSelect.id = "features"
            for (const val of values) {
                var option = document.createElement("option");
                option.value = val;
                option.text = val;
                featuresSelect.appendChild(option);
            }
            var label = document.createElement("label");
            label.innerHTML = "Choose a feature: "
            label.htmlFor = "features";
            document.getElementById("container").appendChild(label).appendChild(featuresSelect);
        }
    };
    xhttp.open("POST", "/features", true);
    xhttp.send()
}