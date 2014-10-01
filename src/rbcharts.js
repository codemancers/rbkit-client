// top level rbkit which encapsulates all rbkit code. This also acts as
// namespace
var Rbkit = {
  // heap data which will be displayed as line chart
  liveObjectsData: {
    labels: ['-', '-'],
    datasets: [
        {
            label: 'Live Objects',
            fillColor: "rgba(220,220,220,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",
            data: [0, 0]
        }
    ]
  },

  heapData: {
    labels: ['-', '-'],
    datasets: [
        {
            label: 'RES Mem Size',
            fillColor: "rgba(220,220,220,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",
            data: [0, 0]
        },
        {
            label: 'Heap Size',
            fillColor: "rgba(151,187,205,0.2)",
            strokeColor: "rgba(151,187,205,1)",
            pointColor: "rgba(151,187,205,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(151,187,205,1)",
            data: [0, 0]
        }
    ]
  },

  // gc data, which will be used in populating in gc graph
  gcData: {
    datasets: [{label: 'GC Data', data: [0]}],
    labels: [0]
  },

  gcStatsImportantFields: {
    count: 'Count',
    minor_gc_count: 'Minor GC Count',
    major_gc_count: 'Major GC Count',
    heap_sorted_length: 'Heap Length',
    heap_eden_pages: 'Heap Eden Page Length',
    heap_allocated_pages: 'Heap Used',
    heap_live_slots: 'Heap Live Slots',
    heap_free_slots: 'Heap Free Slots',
    heap_swept_slots: 'Heap Swept Slots',
    old_objects: 'Old Objects',
    old_objects_limit: 'Old Object Limit',
    total_allocated_objects: 'Total Allocated Objects',
    total_freed_objects: 'Total Freed Objects'
  },

  // actual charts
  liveObjectsChart      : undefined,
  heapDataChart         : undefined,
  gcChart               : undefined,
  youngGenerationChart  : undefined,
  secondGenerationChart : undefined,
  oldGenerationChart    : undefined,

  // gc timings
  gcStartTime : undefined,
  gcCounter   : 0,

  // function to update gc graph
  updateGcChart: function (gcStarted, timestamp) {
  },

  // function to record gc start time
  gcStarted: function (timestamp) {
    this.gcStartTime = timestamp;
  },

  // function to record gc end time
  gcEnded: function (timestamp) {
    if (!this.gcStartTime) {
      console.info("not sure why gc start time is not set");
      return;
    }

    var gcDurationInMs = parseFloat(timestamp - this.gcStartTime);
    this.gcStartTime = undefined;

    this.gcChart.addData([gcDurationInMs], ++this.gcCounter);
    if (this.gcChart.datasets[0].bars.length > 10) {
      this.gcChart.removeData();
    }
    this.gcChart.update();
  },

  // set of polar colors which can be used
  polarChartDefaultColors: [
    // https://kuler.adobe.com/create/color-wheel/?base=2&rule=Analogous&selected=0&name=My%20Kuler%20Theme&mode=rgb&rgbvalues=1,0.8061476456696997,0.21226577883757825,0.91,0.3792784756568747,0.006674395779380451,1,0.05733450085646208,0.07395439645749258,0.6885308876037232,0.006674395779380451,0.91,0.007334500856462034,0.032410134950262015,1&swatchOrder=0,1,2,3,4
    "#FFCE36",
    "#E86102",
    "#FF0F13",
    "#B002E8",
    "#0208FF",

    // https://kuler.adobe.com/create/color-wheel/?base=2&rule=Analogous&selected=0&name=My%20Kuler%20Theme&mode=rgb&rgbvalues=0.04807254964605545,0.3841648710813974,1,0.026435686294499838,0.91,0.9023077127342737,0.07905020471923063,1,0.34320423970430325,0.48126239660538095,0.91,0.026435686294499838,1,0.8956645395163697,0.057458175950780066&swatchOrder=0,1,2,3,4
    "#0C62FF",
    "#07E8E6",
    "#14FF58",
    "#7BE807",
    "#FFE40F"
  ],

  // helper function to update a particular polar chart
  updatePolarChart: function(chart, newData) {
    // clear the whole chart first
    chart.segments = [];

    var iter = 0;
    for (var key in newData) {
      if (newData.hasOwnProperty(key)) {
        var color = this.polarChartDefaultColors[iter];
        var data = { value: newData[key], label: key, color: color, highlight: color };
        chart.addData(data);
        ++iter;
      }
    }

    chart.updateScaleRange(chart.segments);
    chart.scale.update();
    chart.update();
  },

  // function to update polar chart
  updateYoungGenerationChart: function (newData) {
    this.updatePolarChart(this.youngGenerationChart, newData);
  },

  updateSecondGenerationChart: function (newData) {
    this.updatePolarChart(this.secondGenerationChart, newData);
  },

  updateOldGenerationChart: function (newData) {
    this.updatePolarChart(this.oldGenerationChart, newData);
  },

  updateGcStats: function (gcStats) {
    var gcStatsKeys = Object.keys(this.gcStatsImportantFields);
    for (var iter = 0; iter != gcStatsKeys.length; ++iter) {
      var token = gcStatsKeys[iter];
      var key = this.gcStatsImportantFields[token];
      var value = gcStats[token];

      var entry = document.getElementById(key);
      if (entry) {
        entry.textContent = value;
      } else {
        var trNode = document.createElement('tr');
        var tdKey = document.createElement('td');
        tdKey.textContent = key;
        var tdVal = document.createElement('td');
        tdVal.id = key;
        tdVal.textContent = value;
        trNode.appendChild(tdKey);
        trNode.appendChild(tdVal);

        // append created node to table
        document.getElementById('gc-stats-table')
          .lastElementChild     // this is tbody
          .appendChild(trNode);
      }
    }
  },

  updateLiveObjectsChart: function (newData) {
    var timeStamp = this.getTimeStamp();

    var values = [newData['Heap Objects'].toFixed(2)];
    this.liveObjectsChart.addData(values, timeStamp);

    if (this.liveObjectsChart.datasets[0].points.length > 10) {
      this.liveObjectsChart.removeData();
    }

    this.liveObjectsChart.render();
  },

  getTimeStamp: function() {
    var date = new Date(),
        minutes = date.getMinutes(),
        seconds = date.getSeconds();

    var timeStamp = (minutes < 10 ? '0' + minutes : minutes ) +
                    ':' +
                    (seconds < 10 ? '0' + seconds : seconds );

    return timeStamp;
  },

  updateHeapChart: function (newData) {
    timeStamp = this.getTimeStamp();

    var values = [newData['Mem Size'].toFixed(2), newData['Heap Size'].toFixed(2)];
    this.heapDataChart.addData(values, timeStamp);

    if (this.heapDataChart.datasets[0].points.length > 10) {
      this.heapDataChart.removeData();
    }

    this.heapDataChart.render();
  },

  // helper method, which converts a string to node
  stringToNode: function (string) {
    var div = document.createElement('div');
    div.innerHTML = string;
    return div.childNodes[0];
  },

  // helper to generate and insert legend
  insertLegend: function (chart, canvasDiv) {
    var chartLegend = chart.generateLegend();
    var node = this.stringToNode(chartLegend);
    canvasDiv.parentNode.appendChild(node);
  },

  init: function () {
    // charts for live objects data
    var objectsTooltip = "<%= value %>k";
    var liveObjectsOptions = { animation: false, tooltipTemplate: objectsTooltip };
    var liveObjectsCanvas = document.getElementById('live-objects-chart');
    var liveObjectsCtx    = liveObjectsCanvas.getContext('2d');
    this.liveObjectsChart = new Chart(liveObjectsCtx)
      .Line(this.liveObjectsData, liveObjectsOptions);
    this.insertLegend(this.liveObjectsChart, liveObjectsCanvas);

    // charts for heap data
    var heapSizesTooltip = "<%= value %> MB";
    var heapChartOptions = { animation: false, multiTooltipTemplate: heapSizesTooltip };
    var heapDataCanvas = document.getElementById('heap-chart');
    var heapDataCtx    = heapDataCanvas.getContext('2d');
    this.heapDataChart = new Chart(heapDataCtx)
      .Line(this.heapData, heapChartOptions);
    this.insertLegend(this.heapDataChart, heapDataCanvas);

    // charts for gc stats.
    var gcChartsTooltip = "<%if (label){%><%=label%>: <%}%><%= value %> ms";
    var gcChartOptions = { animation: false, tooltipTemplate: gcChartsTooltip };
    var gcCtx     = document.getElementById('gc-chart').getContext('2d');
    this.gcChart  = new Chart(gcCtx).Bar(this.gcData, gcChartOptions);

    // charts for generations
    var youngGenerationCtx  = document
      .getElementById('generation-one').getContext('2d');
    var secondGenerationCtx = document
      .getElementById('generation-two').getContext('2d');
    var oldGenerationCtx    = document
      .getElementById('generation-three').getContext('2d');

    var polarChartOptions = { animation: false };
    this.youngGenerationChart  = new Chart(youngGenerationCtx)
      .PolarArea([], polarChartOptions);
    this.secondGenerationChart = new Chart(secondGenerationCtx)
      .PolarArea([], polarChartOptions);
    this.oldGenerationChart    = new Chart(oldGenerationCtx)
      .PolarArea([], polarChartOptions);
  },

  receiveLiveData: function(data) {
    switch (data.event_type) {
    case "young_gen":
      this.updateYoungGenerationChart(data.payload);
      break;
    case "second_gen":
      this.updateSecondGenerationChart(data.payload);
      break;
    case "old_gen":
      this.updateOldGenerationChart(data.payload);
      break;
    case "gc_start":
      this.gcStarted(data.timestamp);
      break;
    case "gc_stop":
      this.gcEnded(data.timestamp);
      break;
    case "gc_stats":
      this.updateGcStats(data.payload);
      break;
    case "object_stats":
      this.updateLiveObjectsChart(data.payload);
      this.updateHeapChart(data.payload);
      break;
    }
  }
};

Rbkit.init();
