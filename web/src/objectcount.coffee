class @ObjectCount
  constructor: ->
    @currentObjectCount = {"String": 0}

  resetObjectCount: ->
    @currentObjectCount = {"String": 0}

  addToCurrentObjects: (liveObjectCount) ->
    for objectType, count of liveObjectCount
      @currentObjectCount[objectType] = count

  # return time series data
  timeSeries: ->
    sortedData = _.sortBy(_.pairs(@currentObjectCount), (element) -> element[1])
    top8 = _.object(_.last(sortedData, 8))
    otherCount = 0
    for objectType, count of @currentObjectCount
      unless top8[objectType]
        otherCount += 0
    top8["Other"] = otherCount
    top8
