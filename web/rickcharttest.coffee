counter = 0
mappedClasses = []
colorPalette = new Rickshaw.Color.Palette()
seriesData = [
  { color: colorPalette.color(), data: [x: 0, y: 0], name: '' }
]

# To make stacked series in rickshaw happy, we need to ensure that all the older
# data points are initialized with zero. This does the same. If the data array
# for the new series is zero, we just return the empty array because that gets
# filled with the data later. Otherwise we instantiate the series with zero
# values up till the current counter value.
generateFillerSeries = (count) ->
  return [] if count == 0

  dummyData = []
  for num in [0..count-1]
    dummyData.push { x: num, y: 0 }
  dummyData

isRegistered = (klass) ->
  names = mappedClasses.filter (element) ->
    element.name == klass
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
      mappedClasses.push color: newColor, name: klass

      fillerSeries = generateFillerSeries(counter)
      fillerSeries.push { x: counter, y: count }

      seriesData.push({ color: newColor, data: fillerSeries, name: klass })


      # For the rest of the already registered classes and those which are not
      # present in the objectData object, increment the data by 1 item
      for series in seriesData
        if series.data.length < counter
          series.data.push { x: counter, y: 0 }


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

setInterval(tryQtBridge, 1000)
