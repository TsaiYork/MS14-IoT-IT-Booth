//var clientFromConnectionString = require('azure-iot-device-mqtt').clientFromConnectionString;
//var Message = require('azure-iot-device').Message;
//var configs = require('./configs.js');
//var connectingString = 'HostName=TemperatureLab.azure-devices.net;DeviceId=Linkit7688;SharedAccessKey=UuedpHhVxcBb7l5AhElnn8fMH9RPtJBqKVu2W153+Gk=';
//var client = clientFromConnectionString(connectingString);

var com = require("serialport");
var serialPort = new com.SerialPort("/dev/ttyS0", {
    baudrate: 9600,
    parser: com.parsers.readline('\r\n')
  });

//function printResultFor(op) {
//  return function printResult(err, res) {
//    if (err) console.log(op + ' error: ' + err.toString());
//    if (res) console.log(op + ' status: ' + res.constructor.name);
//  };
//}


var connectCallback = function (err) {
  if (err) {
    console.log('Could not connect: ' + err);
  } else {
    console.log('Client connected');
  }
};

//function getDateTime() {
//    var date = new Date();
//    var hour = date.getHours();
//    hour = (hour < 10 ? "0" : "") + hour;
//    var min  = date.getMinutes();
//    min = (min < 10 ? "0" : "") + min;
//    var sec  = date.getSeconds();
//    sec = (sec < 10 ? "0" : "") + sec;
//    var month = date.getMonth() + 1;
//    month = (month < 10 ? "0" : "") + month;
//    var day  = date.getDate();
//    day = (day < 10 ? "0" : "") + day;
//    return day +"/"+ hour +":"+ min +":"+ sec;
//}



//serialPort.on('data', function(data) {
//    var jobj = JSON.parse(data);
    
//    var temperature = jobj.Temperature;
//    var humidity = jobj.Humidity;
//    var heatIndex = jobj.HeatIndex;  
//    var timestamp = getDateTime();          
//    var data = JSON.stringify({ deviceId: 'Linkit7688', temperature: temperature, humidity: humidity, heatIndex:heatIndex, timestamp:timestamp });
//    var message = new Message(data);
//    message.properties.add('temperatureAlert', (temperature > 30) ? 'true' : 'false');
//    //console.log("Sending message: " + message.getData());
// //   client.sendEvent(message, printResultFor('send'));

// //   console.log("JSON[T]=",jobj.Temperature);
// //   console.log("JSON[H]=",jobj.Humidity);
// //   console.log("JSON[HeatIndex]=",jobj.HeatIndex);
//}); 

var data = JSON.stringify({ deviceId: 'Linkit7688', temperature: 22, humidity: 11, heatIndex: 22, timestamp: 33 });

serialPort.on('open',function() {
    console.log('Port open...');
    var count = 1;
    setInterval(function () {
        var data = JSON.stringify({ deviceId: 'Linkit7688', temperature: count, humidity: 11, heatIndex: 22, timestamp: 33 });
        serialPort.write(data, function (err) {
            if (err) {
                return console.log('Error on write: ', err.message);
            }
            console.log("Sending message: " + data);
            count += 1;
        });
    }, 5000);

});

//client.open(connectCallback);


