counter = 0
mappedClasses = []
colorPalette = new Rickshaw.Color.Palette()

isRegistered = (klass) ->
  mappedClasses.indexOf(klass) > -1

notMappedClasses =  (objectData) ->
  klasses = Object.keys objectData
  klasses.filter (element) ->
    mappedClasses.indexOf(element) < 0

receiveObjectData = (objectData) ->
  unMappedClasses = notMappedClasses(objectData)

  for klass in unMappedClasses
    graph.series.addItem({name: klass, color: colorPalette.color()})
    mappedClasses.push klass
  graph.series.addData(objectData)

graph = new Rickshaw.Graph(
  element: document.querySelector('#chart')
  width: document.width - 30
  height: document.height - 30
  renderer: 'bar'
  series: new Rickshaw.Series.FixedDuration(
    [{ name: '', color: colorPalette.color() }],
    undefined,
    {
      timeInterval: 100
      maxDataPoints: 100
      timeBase: new Date().getTime() / 1000
    }
  )
)

tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)
    graph.render()

setInterval(tryQtBridge, 1000)

new Rickshaw.Graph.Axis.Time(graph: graph)
new Rickshaw.Graph.Axis.Y.Scaled(
  graph: graph
  tickFormat: Rickshaw.Fixtures.Number.formatKMBT
  scale: d3.scale.log()
)
new Rickshaw.Graph.HoverDetail(
  graph: graph
  yFormatter: (y) -> "Count: #{y}"
)
