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
  console.log 'receiving data'
  for klass, count of objectData
    if isRegistered(klass)
      dataArray = findSeriesFor(klass)
      console.log "the color for data array is #{dataArray.color}"
      dataArray.data.push { x: counter, y: count }
    else
      console.log 'Got an new Class, pushing to mapped Classes'
      newColor = colorPalette.color()
      mappedClasses.push klass: klass, color: newColor, name: klass
      seriesData.push({ color: newColor, data: [ x: counter, y: count], name: klass })

  console.log "incrementing counter to #{counter}"
  counter += 1

graph = new Rickshaw.Graph(
  element: document.querySelector('#chart')
  width: document.width - 30
  height: document.height - 30
  renderer: 'line'
  series: seriesData
)

tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)
    graph.render()

setInterval(tryQtBridge, 1000)
