class @GcStat
  constructor: ->
    @gcStartTime = null
    @gcEndTime = null
    @lastGcDuration = null
    @gcDurations = []

  gcStarted: (gcEvent) ->
    @gcStartTime = gcEvent.timestamp

  gcStopped: (gcEvent) ->
    @gcEndTime = gcEvent.timestamp
    @gcDurations.push(@gcEndTime - @gcStartTime)
