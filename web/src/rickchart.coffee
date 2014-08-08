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
        [{ name: '', color: @colorPalette.color() }],
        undefined,
        {
          timeInterval: 100
          maxDataPoints: 100
          timeBase: new Date().getTime() / 1000
        }
      )
    )

    new Rickshaw.Graph.Axis.Time(graph: @graph)
    new Rickshaw.Graph.Axis.Y.Scaled(
      graph: @graph
      tickFormat: Rickshaw.Fixtures.Number.formatKMBT
      scale: d3.scale.log()
    )
    new Rickshaw.Graph.HoverDetail(
      graph: @graph
      yFormatter: (y) -> "Count: #{y}"
    )

  addItem: (item) =>
    @graph.series.addItem(name: item.name, color: @colorPalette.color())

  addData: (item) =>
    @graph.series.addData(item)

class @Charter
  mappedClasses: []

  constructor: (grapher) ->
    @grapher = grapher

  receiveObjectData: (objectData) =>
    unMappedClasses = @notMappedClasses(objectData, @mappedClasses)

    for klass in unMappedClasses
      @grapher.addItem(name: klass)
      @mappedClasses.push klass

    @grapher.addData(objectData)

  tryQtBridge: =>
    if window.rbkitClient
      window.rbkitClient.sendDatatoJs.connect(@receiveObjectData)
      @grapher.graph.render()

  notMappedClasses: (objectData, mappedClasses) =>
    klasses = Object.keys objectData
    klasses.filter (element) =>
      @mappedClasses.indexOf(element) < 0


grapher = new Graph('#chart')
grapher.init()

charter = new Charter(grapher)
setInterval(charter.tryQtBridge, 1000)
