

receiveTypeCount = function(typeCounts) {
  var div = document.createElement('div');
  div.innerHTML = JSON.stringify(typeCounts);
  document.body.appendChild(div);
};

rbkitClient.sendDatatoJs.connect(receiveTypeCount);

alert(rbkitClient);
