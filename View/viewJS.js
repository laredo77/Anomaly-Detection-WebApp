function results(algo) {
    if (algo == 'LinearReg') {
        var featueres = require('');
        var trace1 = {
            x: [1, 2, 3, 4],
            y: [10, 15, 13, 17],
            mode: 'markers'
        };
        var data = [ trace1 ];
        var layout = {
            title:'Linear Reg'
        };
        Plotly.newPlot('myDiv', data, layout);
    } else {
        var trace1 = {
            x: [1, 2, 3, 4],
            y: [10, 15, 13, 17],
            mode: 'markers'
        };
        var data = [ trace1 ];
        var layout = {
            title:'Hybrid Algorithm'
        };
        Plotly.newPlot('myDiv', data, layout);
    }
}