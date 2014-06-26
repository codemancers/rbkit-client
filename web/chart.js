var stringObjects = [],
    floatObjects = [],
    integerObjects = [],
    otherObjects = [];

// Returns a random number between min and max
function getRandomArbitrary(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

function makeData(offset) {
  var time = (new Date()).getTime() + offset*1000;
  stringObjects.push([time, getRandomArbitrary(0,5000)]);
  floatObjects.push([time, getRandomArbitrary(0,5000)]);
  integerObjects.push([time, getRandomArbitrary(0,5000)]);
  otherObjects.push([time, getRandomArbitrary(0,5000)]);
}

function makeSeriesData() {
  var data = [];
  for (i = -19; i <= 0; i++) { makeData(i); }
  return [{
    name: "String",
    data: stringObjects
    },
    {
      name: "Float",
      data: floatObjects
    },
    {
      name: "Integer",
      data: integerObjects
    },
    {
      name: "Other",
      data: otherObjects
    }];
}

$(function () { 
  $('#container').highcharts({
    chart: {
      type: 'column',
      events: {
        load: function() {
          // set up the updating of the chart each second
          var series = this.series;
          setInterval(function() {
            var x = (new Date()).getTime(), // current time
                y = getRandomArbitrary(0,5000);
            series[0].addPoint([x,y], true, true);
            series[1].addPoint([x,getRandomArbitrary(0,5000)], true, true);
            series[2].addPoint([x,getRandomArbitrary(0,5000)], true, true);
            series[3].addPoint([x,getRandomArbitrary(0,5000)], true, true);
          }, 1000);
        }
      }
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
          enabled: true,
          color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white',
          style: {
            textShadow: '0 0 3px black, 0 0 3px black'
          }
        }
      }
    },
    series: makeSeriesData()
  });
});
