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
    @graph.series.addData(item)

  renderGraphAndLegend: =>
    @graph.render()
    @renderHoverDetail()

    @initLegend() unless @legend

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
