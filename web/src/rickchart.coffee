class @Graph
  constructor: (element) ->
    @element = element
    @colorPalette = new Rickshaw.Color.Palette(scheme: 'spectrum14')
    @otherObjects = {}

  # This method is used to convert the following seriesData object structure:
  #   { Foo: 12, String: 13 }
  # to:
  #   [ { name: 'Foo', Foo: 12 }, { name: 'String', String: 13 } ]
  #
  # This format is required to instantiate the series.
  formatSeriesData: (seriesData) =>
    for name, count of seriesData
      data = new Object
      data['name'] = name
      data[name]   = count
      data

  sortSeriesData: (seriesData) =>
    sortedPairs = _.sortBy(_.pairs(seriesData), (element) -> element[1] * -1)
    _.object(sortedPairs)

  # The series gets sorted in descending order and then aggregates the data
  # to pick the first 10 items and shove the rest into 'Others' data point
  sortAndPickObjectsForRendering: (seriesData) =>
    sortedData = @sortSeriesData(seriesData)
    finalData = {}
    otherCount = 0
    atomicData = 0
    for objectType, count of sortedData
      if @otherObjects[objectType]
        otherCount += count
      else
        if atomicData < 15
          finalData[objectType] = count
          atomicData += 1
        else
          otherCount += count
          @otherObjects[objectType] = true
    if otherCount > 0
      finalData["Other"] = otherCount
    finalData

  init: (seriesData) =>
    sortedData = @sortAndPickObjectsForRendering(seriesData)
    formattedData = @formatSeriesData(sortedData)

    @graph = new Rickshaw.Graph(
      element: document.querySelector(@element)
      height: document.height - 150
      renderer: 'bar'
      stack: true
      gapSize: 0.3
      series: new Rickshaw.Series.FixedDuration(
        formattedData, @colorPalette, { timeInterval: 1000, maxDataPoints: 15 }
      )
    )
    @renderAxes()

  renderAxes: =>
    new Rickshaw.Graph.Axis.Time(graph: @graph).render()
    new Rickshaw.Graph.Axis.Y.Scaled(
      graph: @graph
      tickFormat: Rickshaw.Fixtures.Number.formatKMBT
      scale: d3.scale.linear()
    ).render()

  renderHoverDetail:  =>
    new Rickshaw.Graph.HoverDetail(
      graph: @graph
      formatter: (series, x, y) ->
        name  = '<div class="class-name">Class Name : <strong>' + series.name + '</strong></div>'
        count = '<div class="class-count">Class Count: <strong>' + parseInt(y) + '</strong></div>'
        colorSwatch = '<span class="class-color" style="background-color: ' + series.color + '"></span>'

        '<div class="class-hoverdetail">' +
          colorSwatch +
          '<div class="class-metadata">' +
            name +
            count +
          '</div>' +
        '</div>'
    )

  initLegend: =>
    @legend = new Rickshaw.Graph.Legend(
      graph: @graph,
      element: document.getElementById('legend')
    )
    shelving = new Rickshaw.Graph.Behavior.Series.Toggle(
      graph: @graph,
      legend: @legend
    )
    new Rickshaw.Graph.Behavior.Series.Highlight(
      graph: @graph
      legend: @legend
    )

    # This is required because Rickshaw is dumb for now.
    # We can't call @legend.render() to update the items because that
    # would clear the highlighting and toggling setup that's being done.
    @legend.shelving = shelving
    @graph.series.legend = @legend

  addData: (item) =>
    @init(item) unless @graph
    sortedData = @sortAndPickObjectsForRendering(item)
    @graph.series.addData(sortedData)

  renderGraphAndLegend: =>
    @graph.render()
    @renderHoverDetail()

    @initLegend() unless @legend

  updateGcStats: (gcStats) =>
    stats = $('#gcstats tbody')
    stats.empty()
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
      stats.append(row)

class @Charter
  constructor: (grapher) ->
    @grapher = grapher

  receiveObjectData: (objectData) =>
    switch objectData.event_type
      when 'object_stats'
        @grapher.addData(objectData.payload)
      when "gc_stats"
        @grapher.updateGcStats(objectData.payload)

    @grapher.renderGraphAndLegend()

  tryQtBridge: =>
    window.jsBridge?.jsEvent.connect(@receiveObjectData)

grapher = new Graph('#chart')

charter = new Charter(grapher)
setTimeout(charter.tryQtBridge, 1000)
