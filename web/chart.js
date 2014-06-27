var knownClasses = {"String": true};
var legendIndex = 1;

function makeDummySeries() {
  var data = [];

  for (i = -19; i <= 0; i++) {
    var time = (new Date()).getTime() + i*1000;
    data.push([time, 0]);
  }
  return data;
}


// Returns a random number between min and max
function getRandomArbitrary(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

var chart = $('#container').highcharts({
  chart: {
    type: 'column',
  },
  title: {
    text: 'Live Objects'
  },
  xAxis: {
    type: 'datetime',
    tickPixelInterval: 150,
    title: {
      enabled: false
    }
  },
  yAxis: {
    min: 0,
    title: {
      text: 'Live Objects'
    },
    stackLabels: {
      enabled: true,
      style: {
        fontWeight: 'bold',
        color: (Highcharts.theme && Highcharts.theme.textColor) || 'gray'
      }
    }
  },
  plotOptions: {
    column: {
      stacking: 'normal',
      dataLabels: {
        enabled: false,
        color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white',
        style: {
          textShadow: '0 0 3px black, 0 0 3px black'
        }
      }
    }
  },
  series: [
    {
      name: "String",
      data: makeDummySeries()
    }
  ]
}).highcharts();

function addNewSeriesData(chart, time, value) {
  var selectedSeriesData = chart.series[0].data;
  var data = [];
  for(i = 0; i < 19; i++) {
    data.push([selectedSeriesData[i].x, 0]);
  }
  data.push([time, value]);
  return data;
}

function receiveTypeCount(typeCounts) {
    console.info(typeCounts);
  var time1 = (new Date()).getTime();
  for(objectType in typeCounts) {
    var data = typeCounts[objectType];
    if(knownClasses[objectType]) {
      var selectedSeries = null;
      for(index in chart.series) {
        if(chart.series[index].name === objectType) {
          chart.series[index].addPoint([time1, data], true, true);
        }
      }
    } else {
      chart.addSeries({
        name: objectType,
        data: addNewSeriesData(chart, time1, data),
        legendIndex: legendIndex
      }, true, true);
      knownClasses[objectType] = true;
      legendIndex = legendIndex + 1;
    }
  }
};

function establishBridge() {
    if(rbkitClient) {
      clearInterval(interval);
      rbkitClient.sendDatatoJs.connect(receiveTypeCount);
    }
}

var interval = setInterval(establishBridge, 1000);
