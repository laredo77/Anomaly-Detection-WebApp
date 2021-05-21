// const delay = ms => new Promise(resolve => setTimeout(resolve, ms))
// points of anomalies in linear reg
function resultLinear(feature) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            var layout = {
                title: 'Linear Reg Algorithm'
            };
            var xArray = [];
            var yArray = [];
            const dataFromServer = JSON.parse(this.response);
            for(let i=0; i<dataFromServer.length;i++){
                if(dataFromServer[i].description === feature) {
                    xArray.push(dataFromServer[i].Point.x);
                    yArray.push(dataFromServer[i].Point.y);
                }
            }
            var trace1 = {
                x: xArray,
                y: yArray,
                mode: 'markers'
            };
            var data = [trace1];
            Plotly.newPlot('myDiv', data, layout)
        }
    };
    xhttp.open("POST", "/detect/linear", true);
    xhttp.send()
}
// points of anomalies in Hybrid
function resultHybrid(feature) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {
            var layout = {
                title: 'Hybrid Algorithm'
            };
            var xArray = [];
            var yArray = [];
            const dataFromServer = JSON.parse(this.response);
            for(let i=0; i<dataFromServer.length;i++){
                if(dataFromServer[i].description === feature) {
                    xArray.push(dataFromServer[i].Point.x);
                    yArray.push(dataFromServer[i].Point.y);
                }
            }
            var trace1 = {
                x: xArray,
                y: yArray,
                mode: 'markers'
            };
            var data = [trace1];
            Plotly.newPlot('myDiv', data, layout)
        }
    };
    xhttp.open("POST", "/detect/hybrid", true);
    xhttp.send()
}
// init the regression line
function initLinearReg(feature){

}
function initHybrid(feature){

}