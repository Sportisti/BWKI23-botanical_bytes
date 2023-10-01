data = twoDArray
function x_label(duration, data) {
    var labels = [];
    for (var b = 0; b < data.length; b++) {
        labels.push(Math.floor((b * duration)/60));
    }
    return labels;
}

for (var spalte = 0; spalte < data[0].length; spalte++) {
    var header = data[0][spalte];
    console.log(header)
    var data_temp = [];
    for (var i = 1; i < data.length; i++) {

        data_temp.push(data[i][spalte]);
    }

    const ctx = document.getElementById(spalte);

    new Chart(ctx, {
        type: 'line',
        data: {
            labels: x_label(1, data_temp),
            datasets: [{
                label: header,
                data: data_temp,
                borderWidth: 1,
                pointRadius: 0
            }]
        },
    });
}