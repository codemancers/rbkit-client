class @Graph
  constructor: (element) ->
    @element = element
    @colorPalette = new Rickshaw.Color.Palette()

  init: =>
    @graph = new Rickshaw.Graph(
      element: document.querySelector(@element)
      width: document.width - 50
      height: document.height - 150
      renderer: 'bar'
      stack: true
      gapSize: 0.5
      series: new Rickshaw.Series.FixedDuration(
        [{ name: 'baseline' }],
        @colorPalette,
        {
          timeInterval: 1000
          maxDataPoints: 15
        }
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

  renderLegend: =>
    @legend = new Rickshaw.Graph.Legend(
      graph: @graph,
      element: document.getElementById('legend')
    )
    new Rickshaw.Graph.Behavior.Series.Toggle(
      graph: @graph,
      legend: @legend
    )
    new Rickshaw.Graph.Behavior.Series.Order(
      graph: @graph,
      legend: @legend
    )
    new Rickshaw.Graph.Behavior.Series.Highlight(
      graph: @graph
      legend: @legend
    )

  addData: (item) =>
    @graph.series.addData(item)

  renderGraphAndLegend: =>
    if @legend
      @legend.render()
    else
      # Remove the baseline series only if there is one more data item is present
      # This is to avoid bad rendering
      @graph.series.shift() if @graph.series.length > 1
      @renderLegend()

    @graph.render()

class @Charter
  constructor: (grapher) ->
    @grapher = grapher

  receiveObjectData: (objectData) =>
    switch objectData.event_type
      when 'object_stats'
        @grapher.addData(objectData.payload)

    @grapher.renderGraphAndLegend()

  tryQtBridge: =>
    window.jsBridge?.jsEvent.connect(@receiveObjectData)

grapher = new Graph('#chart')
grapher.init()

charter = new Charter(grapher)
setTimeout(charter.tryQtBridge, 1000)
