class @Chart
  placeHolderSeries = ->
    ([(new Date()).getTime() + num*1000, 0] for num in [-9..0])

  dataForNewSeries = (series, currentTime, count)->
    data = series.data
    newData = ([data[num].x, 0] for num in [0..8])
    newData.push([currentTime, count])
    newData

  constructor: ->
    @knownClasses = {"String": true}
    @legendIndex = 1
    @currentObjectCount = {"String": 0}
    @objectsAsOther = {}

  plotChart: ->
    @chart = $("#container").highcharts(
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
    if window.rbkitClient
      window.rbkitClient.sendDatatoJs.connect(@receiveLiveData)

  receiveLiveData: (liveObjectCount) =>
    @addToCurrentObjects(liveObjectCount)

  addToOther: (objectType, count) ->
    existingCount = @objectsAsOther[objectType]
    if existingCount
      @objectsAsOther[objectType] = existingCount + count
    else
      @objectsAsOther[objectType] = count

  addToCurrentObjects: (liveObjectCount) ->
    knownClassesCount = Object.keys(@currentObjectCount).length

    for objectType, count of liveObjectCount
      if @currentObjectCount[objectType]?
        @currentObjectCount[objectType] = count
      else
        if(@objectsAsOther[objectType]?)
          @addToOther(objectType, count)
        else
          if(knownClassesCount < 8)
            @currentObjectCount[objectType] = count
          else
            @addToOther(objectType, count)

  updateChart: =>
    currentTime = (new Date()).getTime()
    for objectType, count of @currentObjectCount
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
