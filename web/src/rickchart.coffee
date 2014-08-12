class @Graph
  constructor: (element) ->
    @element = element
    @colorPalette = new Rickshaw.Color.Palette()

  init: =>
    @graph = new Rickshaw.Graph(
      element: document.querySelector(@element)
      width: document.width - 30
      height: document.height - 30
      renderer: 'bar'
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

  addData: (item) =>
    @graph.series.addData(item)

  render: =>
    @graph.render()

class @Charter
  constructor: (grapher) ->
    @grapher = grapher

  receiveObjectData: (objectData) =>
    console.log("receiving data now #{new Date()}")
    @grapher.addData(objectData)
    @grapher.graph.render()

  tryQtBridge: =>
    if window.rbkitClient
      window.rbkitClient.sendDatatoJs.connect(@receiveObjectData)

grapher = new Graph('#chart')
grapher.init()

charter = new Charter(grapher)
setTimeout(charter.tryQtBridge, 1000)
