counter = 0
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

# To make stacked series in rickshaw happy, we need to ensure that all the older
# data points are initialized with zero. This does the same.
generateFillerSeries = (count) ->
  return [] if count == 0

  dummyData = []
  for num in [0..count-1]
    dummyData.push { x: num, y: 0 }
  dummyData

isRegistered = (klass) ->
  names = mappedClasses.filter (element) ->
    element.klass == klass
  names.length

#TODO: All data series arrays should be pushed with {0, counter} to ensure 
# it won't throw error due to series mismatch
receiveObjectData = (objectData) ->
  for klass, count of objectData
    # if the klass is registered, push the count to that class
    # and push zero data to all the others
    #
    # if the klass is not registered, initialize the series with blank data
    # up till the current count and push the data in to the last column
    # fill the rest of the series with blank data
    if isRegistered(klass)
      for series in seriesData
        if series.name == klass
          series.data.push { x: counter, y: count }
        else
          series.data.push { x: counter, y: 0 }
    else
      newColor = colorPalette.color()
      mappedClasses.push klass: klass, color: newColor, name: klass

      console.log "counter is #{counter}"
      fillerSeries = generateFillerSeries(counter)
      fillerSeries.push { x: counter, y: count }
      console.log "new series data count is #{fillerSeries.length}"

      for series in seriesData
        if series.data.length < counter
          series.data.push { x: counter, y: 0 }
        console.log "old series data count is #{series.data.length}"

      seriesData.push({ color: newColor, data: fillerSeries, name: klass })

  counter += 1

graph = new Rickshaw.Graph(
  element: document.querySelector('#chart')
  width: document.width - 30
  height: document.height - 30
  renderer: 'area'
  series: seriesData
)

new Rickshaw.Graph.Axis.Time(graph: graph)
new Rickshaw.Graph.Axis.Y(graph: graph)

tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)
    graph.render()

setInterval(tryQtBridge, 100)
