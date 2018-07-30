//Herramientas necesarias para el servidor
var http = require('http');
var express = require('express');
var socketIo = require('socket.io');
var app = express();
var server = http.createServer(app);
var io = socketIo.listen(server);

app.use(express.static(__dirname + '/public'));
server.listen(3000, () => console.log('Initializing server on port 3000'));

//Toma de datos en el puerto serial de arduino
var Serialport = require('serialport');
var ReadLine = Serialport.parsers.Readline;

var port = new Serialport('/dev/cu.usbmodemFA131', {
  baudRate: 9600
});

var parser = port.pipe(new ReadLine({delimeter: '\r\n'}));

parser.on('data', function(data){
  console.log(data);
  io.emit('temp_hum', data);
});
port.on('err', function(err){
  console.log(err);
})
