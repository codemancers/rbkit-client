
receiveTypeCount = function(typeCounts) {
  var div = document.createElement('div');
  div.innerHTML = JSON.stringify(typeCounts);
  document.body.appendChild(div);
};

function establishBridge() {
    console.info("Calling establish bridge");
  if(rbkitClient) {
    clearInterval(interval);
    rbkitClient.sendDatatoJs.connect(receiveTypeCount);
  }
}
var interval = setInterval(establishBridge, 1000);

