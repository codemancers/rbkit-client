class @Graph
  constructor: (element) ->
    @element = element
    @colorPalette = new Rickshaw.Color.Palette()

  init: =>
    @graph = new Rickshaw.Graph(
      element: document.querySelector(@element)
      width: document.width - 30
      height: document.height - 150
      renderer: 'bar'
      stack: true
      series: new Rickshaw.Series.FixedDuration(
        [{ name: 'baseline' }],
        @colorPalette,
        {
          timeInterval: 100
          maxDataPoints: 50
          timeBase: new Date().getTime()/1000
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
      yFormatter: (y) -> "Count: #{y}"
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
    @graph.render()
    if @legend then @legend.render() else @renderLegend()

class @Charter
  constructor: (grapher) ->
    @grapher = grapher

  receiveObjectData: (objectData) =>
    @grapher.addData(objectData)
    @grapher.renderGraphAndLegend()

  tryQtBridge: =>
    if window.rbkitClient
      window.rbkitClient.sendDatatoJs.connect(@receiveObjectData)

grapher = new Graph('#chart')
grapher.init()

charter = new Charter(grapher)
setTimeout(charter.tryQtBridge, 1000)
