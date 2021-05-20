function resultLinear() {
    var trace1 = {
        x: [1, 2, 3, 4],
        y: [10, 15, 13, 17],
        mode: 'markers'
    };
    var data = [trace1];
    var layout = {
        title: 'Linear Reg'
    };
    Plotly.newPlot('myDiv', data, layout);
}

function resultHybrid() {

    var trace1 = {
        x: [1, 2, 3, 4],
        y: [10, 15, 13, 17],
        mode: 'markers'
    };
    var data = [trace1];
    var layout = {
        title: 'Hybrid Algorithm'
    };
    Plotly.newPlot('myDiv', data, layout);

}