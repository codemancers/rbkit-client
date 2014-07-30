counter = 0
data = [ {x: 0, y: 0} ]
mappedClasses = []
colorPalette = new Rickshaw.Color.Palette()
seriesData = [
  { color: colorPalette.color(), data: [x: 0, y: 0], name: '' }
]

findSeriesFor = (klass) ->
  colors = mappedClasses.filter (element) ->
    element.klass == klass
  color = colors[0].color
  series = seriesData.filter (element) ->
    element.color == color
  series[0]


isRegistered = (klass) ->
  names = mappedClasses.filter (element) ->
    element.klass == klass
  names.length

receiveObjectData = (objectData) ->
  for klass, count of objectData
    if isRegistered(klass)
      dataArray = findSeriesFor(klass)
      dataArray.data.push { x: counter, y: count }
    else
      newColor = colorPalette.color()
      mappedClasses.push klass: klass, color: newColor, name: klass
      seriesData.push({ color: newColor, data: [ x: counter, y: count], name: klass })

  counter += 1

graph = new Rickshaw.Graph(
  element: document.querySelector('#chart')
  width: document.width - 30
  height: document.height - 30
  renderer: 'line'
  series: seriesData
)

new Rickshaw.Graph.Axis.Time(graph: graph)
new Rickshaw.Graph.Axis.Y(graph: graph)

tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)
    graph.render()

setInterval(tryQtBridge, 100)
