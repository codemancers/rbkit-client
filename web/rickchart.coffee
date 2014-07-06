palette = new Rickshaw.Color.Palette(scheme: 'classic9')

# This will create another series. One set of series data is of the following format:
# {
#   data: [ { x: 0, y: 100 } ]
#   color: new Color
#   name: optional Name
# }

class @SeriesData
  constructor: (objCount, className, color, counter) ->
    @data  =  objCount
    @className  = className
    @color = color || palette.color()
    @counter = counter
  toHash: ->
    data = new Object()
    data.x = @counter
    data.y = @data

    color: @color
    data: [ data ]
    name: @className

allCapturedSeries = [
  {
    color: palette.color(),
    data: [ x: 0, y: 0 ]
    name: 'none'
  }
]

class @Graph
  constructor: (element) ->
    @element = document.getElementById element
    @series = allCapturedSeries
  init: ->
    hash =
      element: @element
      width: '750',
      height: 450,
      renderer: 'bar',
      stroke: true,
      preserve: true,
      series: @allCapturedSeries
    @graph = new Rickshaw.Graph(hash)
  render: ->
    @graph.render()
  update: ->
    @graph.update()



graph = new @Graph('chart').init()

new Rickshaw.Graph.HoverDetail(
  graph: graph.graph,
  xFormatter: (x) ->
    new Date(x * 1000).toString()

)

new Rickshaw.Graph.Annotate(
  graph: graph.graph,
  element: document.getElementById('timeline')
)

ticksTreatment = 'glow'

# X-axis
new Rickshaw.Graph.Axis.Time(
  graph: graph.graph,
  ticksTreatment: ticksTreatment,
  timeFixture: new Rickshaw.Fixtures.Time.Local()
).render()


# Y-axis
yAxis = new Rickshaw.Graph.Axis.Y(
  graph: graph.graph,
  tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
  ticksTreatment: ticksTreatment
).render()


# Preview Axis
new Rickshaw.Graph.Axis.Time(
  graph: graph.graph,
  timeFixture: new Rickshaw.Fixtures.Time.Local(),
  ticksTreatment: ticksTreatment
).render()


window.counter = 0

receiveObjectData = (objectData) ->
  for className, count of objectData
    if classAlreadyRegistered(className)
      seriesToPushTo = findSeries(className)
      dataToPush = {
        x: counter,
        y: count
      }
      seriesToPushTo.data.push dataToPush
    else
      seriesData = new @SeriesData(count, className, counter)
      allCapturedSeries.push seriesData.toHash()

  window.counter += 1
  graph.update()

classAlreadyRegistered = (className) ->
  filteredClasses = allCapturedSeries.filter (element, index) ->
    element.className == className
  filteredClasses.length > 0

findSeries = (className) ->
  filteredClasses = allCapturedSeries.filter (element, index) ->
    element.className == className
  filteredClasses[0]

@tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)
    graph.render()

window.qtBridgeHandler = setInterval(tryQtBridge, 1000)
