class @Chart
  placeHolderSeries = ->
    ([(new Date()).getTime() + num*1000, 0] for num in [-9..0])

  dataForNewSeries = (series, currentTime, count)->
    data = series.data
    newData = ([data[num].x, 0] for num in [0..8])
    newData.push([currentTime, count])
    newData

  constructor: ->
    @objectCounter = new ObjectCount()
    @knownClasses = {"String": true}
    @legendIndex = 1

  plotChart: ->
    @chart = $("#object-container").highcharts(
      chart: { type: 'column' },
      title: { text: 'Live objects'},
      xAxis: {
        type: 'datetime',
        tickPixelInterval: 150,
        title: { enabled: false }
      },
      yAxis: {
        min: 0,
        title: { text: 'Live Objects' },
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
      series: [{name: 'String', data: placeHolderSeries()}]
    ).highcharts()

  tryQtBridge: =>
    window.setTimeout(@establishQtBridge, 1000)

  establishQtBridge: =>
    setInterval(@updateChart, 1000)
    window.jsBridge?.jsEvent.connect(@receiveLiveData)

  receiveLiveData: (data) =>
    switch data.event_type
      when "object_stats"
        @addToCurrentObjects(data.payload)
      when "gc_stats"
        @updateGcStats(data.payload)

  updateGcStats: (gcStats) =>
    $stats = $('#gc-stats-table tbody')
    $stats.empty()
    importantFields = [
      'count', 'minor_gc_count', 'major_gc_count',
      'heap_length', 'heap_eden_page_length', 'heap_used',
      'heap_live_slot', 'heap_free_slot', 'heap_swept_slot',
      'old_object', 'old_object_limit', 'remembered_shady_object',
      'total_allocated_object', 'total_freed_object'
    ]
    for key in importantFields
      value = gcStats[key]
      row = "<tr><td>#{key}</td><td>#{value}</td></tr>"
      $stats.append(row)

  addToCurrentObjects: (liveObjectCount) ->
    @objectCounter.addToCurrentObjects(liveObjectCount)

  updateChart: =>
    currentTime = (new Date()).getTime()
    timeSeries = @objectCounter.timeSeries()
    for objectType, count of timeSeries
      if @knownClasses[objectType]?
        @addNewDataPoint(objectType, count, currentTime)
      else
        @addNewSeries(objectType, count, currentTime)

  addNewDataPoint: (objectType, count, currentTime) ->
    selectedSeries = (series for series in @chart.series when series.name == objectType)[0]
    selectedSeries.addPoint([currentTime, count], true, true)

  addNewSeries: (objectType, count, currentTime) ->
    @chart.addSeries({
      name: objectType,
      data: dataForNewSeries(@chart.series[0], currentTime, count),
      legendIndex: @legendIndex
    }, true, true)
    @knownClasses[objectType] = true;
    @legendIndex += 1

chart = new Chart()
chart.plotChart()
chart.tryQtBridge()
