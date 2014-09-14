var jsBridgeReceiveLiveData = function(data) {
  Rbkit.receiveLiveData(data);
};


var pollIntervalTimeout = 1000;

var pollAndEstablishQtBridge = function() {
  if (window.jsBridge === undefined) {
    setTimeout(pollAndEstablishQtBridge, pollIntervalTimeout);
  } else {
    window.jsBridge.jsEvent.connect(jsBridgeReceiveLiveData);
  }
};

pollAndEstablishQtBridge();
