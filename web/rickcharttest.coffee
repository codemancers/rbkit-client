receiveObjectData = (objectData) -> 
  console.log objectData

tryQtBridge = ->
  if window.rbkitClient
    window.rbkitClient.sendDatatoJs.connect(receiveObjectData)

setInterval(tryQtBridge, 500)
