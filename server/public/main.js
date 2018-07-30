var socket = io();

var temperatureDisplay = document.getElementById('temp_humi');

socket.on('temp_hum', function (data) {
  console.log(data);
  temp_humi.innerHTML = `${data}`;
});
