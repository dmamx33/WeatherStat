/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-03-11T20:26:42+01:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: script.js
 * @Last modified by:   daniel
 * @Last modified time: 2021-03-12T00:14:45+01:00
 * @License: CC by-sa
 */


var chartT = new Highcharts.Chart({
    chart: { renderTo: 'chart-temperature' },
    title: { text: 'BME680 Temperature Chart' },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
        series: { color: '#059e8a' }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Temperature in Celsius' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);

            if (chartT.series[0].data.length > 40) {
                chartT.series[0].addPoint([x, y], true, true, true);
            } else {
                chartT.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
}, 2000);

var chartI = new Highcharts.Chart({
    chart: { renderTo: 'chart-IAQ' },
    title: { text: 'BME680 IAQ Chart' },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
        series: { color: '#c04d39' }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'IAQ Level' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);

            if (chartI.series[0].data.length > 40) {
                chartI.series[0].addPoint([x, y], true, true, true);
            } else {
                chartI.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/iaq", true);
    xhttp.send();
}, 2000);

var chartP = new Highcharts.Chart({
    chart: { renderTo: 'chart-pressure' },
    title: { text: 'BME680 Pressure Chart' },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
        series: { color: '#98ff3c' }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Pressure in HPa' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);

            if (chartP.series[0].data.length > 40) {
                chartP.series[0].addPoint([x, y], true, true, true);
            } else {
                chartP.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/pressure", true);
    xhttp.send();
}, 2000);


var chartH = new Highcharts.Chart({
    chart: { renderTo: 'chart-humidity' },
    title: { text: 'BME680 Humidity Chart' },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Humidity in %' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);

            if (chartH.series[0].data.length > 40) {
                chartH.series[0].addPoint([x, y], true, true, true);
            } else {
                chartH.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
}, 2000);
