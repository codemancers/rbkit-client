var knownClasses = {"String": true};
var legendIndex = 1;
var lastTypeCount = {"String": 0};
var typeAsOther = {};

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

function addToTypeCollection(typeCollection) {
    var totalKeys = Object.keys(typeCollection).length;

    for(typeIndex in typeCollection) {
        var count = typeCollection[typeIndex];
        if(lastTypeCount[typeIndex]) {
            lastTypeCount[typeIndex] = count;
        } else {
            if(typeAsOther[typeIndex]) {
                addToOther(typeIndex, count);
            } else {
                if(totalKeys < 8) {
                    lastTypeCount[typeIndex] = count;
                } else {
                    addToOther(typeIndex, count);
                }
            }
        }
    }
}

function addToOther(key, count) {
    var otherTotalCount = lastTypeCount["Other"] || 0;
    lastTypeCount["Other"] = otherTotalCount + count;
    typeAsOther[key] = count;
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

updateChart = function() {
  console.info(lastTypeCount);
  var time1 = (new Date()).getTime();
  for(objectType in lastTypeCount) {
    var data = lastTypeCount[objectType];
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
}

receiveTypeCount = function(typeCounts) {
    addToTypeCollection(typeCounts);
};

var interval = null;

function establishBridge() {
  console.info("Calling establish bridge");
  setInterval(updateChart, 1000);
  if(rbkitClient) {
    clearInterval(interval);
    rbkitClient.sendDatatoJs.connect(receiveTypeCount);
  }
}

interval = setInterval(establishBridge, 1000);
