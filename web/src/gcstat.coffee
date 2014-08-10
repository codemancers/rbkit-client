class @GcStat
  placeHolderSeries = ->
    ([(new Date()).getTime() + num*1000, 0] for num in [-9..0])

  constructor: ->
    @gcStartTime = null
    @gcEndTime = null
    @lastGcDuration = null

  gcStarted: (gcEvent) ->
    console.info("Received start #{gcEvent.timestamp}")
    @gcStartTime = gcEvent.timestamp

  gcStopped: (gcEvent) ->
    @gcEndTime = gcEvent.timestamp
    @lastGcDuration = @gcEndTime - @gcStartTime
    console.info("stop is #{gcEvent.timestamp} and GC duration is #{@lastGcDuration}")


  updateChart: ->
    currentTime = (new Date()).getTime()
    if @lastGcDuration?
      @chart.series[0].addPoint([currentTime, @lastGcDuration], true, true)
    else
      @chart.series[0].addPoint([currentTime, 0], true, true)

  plotChart: ->
    @chart = $("#gc-container").highcharts(
      chart: { type: 'spline'},
      title: { text: null },
      xAxis: {
        type: 'datetime',
        tickPixelInterval: 150,
        title: { enabled: false }
      },
      yAxis: {
        min: 0,
        title: { text: 'GC duration (msec)'},
        plotLines: [{
          value: 0,
          width: 1,
          color: '#808080'
        }]
      },
      series: [ {name: 'GC duration', data: placeHolderSeries()}]
    ).highcharts()
