class @ObjectCount
  constructor: ->
    @currentObjectCount = {"String": 0}
    @otherObjects = {}

  resetObjectCount: ->
    @currentObjectCount = {"String": 0}

  addToCurrentObjects: (liveObjectCount) ->
    for objectType, count of liveObjectCount
      @currentObjectCount[objectType] = count

  # return time series data
  timeSeries: ->
    sortedData = _.object(_.sortBy(_.pairs(@currentObjectCount), (element) -> element[1]*-1 ))
    otherCount = 0
    atomicData = 0
    finalData = {}
    for objectType, count of sortedData
      if @otherObjects[objectType]
        otherCount += count
      else
        if atomicData < 10
          finalData[objectType] = count
          atomicData += 1
        else
          otherCount += count
          @otherObjects[objectType] = true
    if otherCount > 0
      finalData["Other"] = otherCount
    finalData
