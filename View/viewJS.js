const delay = ms => new Promise(resolve => setTimeout(resolve, ms))

function resultLinear() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = async function () {
        if (this.readyState === 4 && this.status === 200) {

            // this.repsonse can get objects from the server
            document.getElementById("demo").innerHTML = this.responseText;
            // update graph
            await delay(1000)
            var layout = {
                title: this.responseText
            };
            var data
            Plotly.newPlot('myDiv', data, layout)

        }
    };

    xhttp.open("POST", "/api/model", true);
    xhttp.send()

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