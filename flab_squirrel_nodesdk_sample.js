'use strict';

var util = require('util');
var SfDeviceClient = require('smartfactory-device-sdk').SfDeviceClient;

//set MPU & MCU bridge
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var serialPort = new SerialPort("/dev/ttyS0", {
    baudrate: 57600,
    //parser: serialport.parsers.raw
    parser: serialport.parsers.readline("\n")
});

var deviceID = 'ContosoFLabSquirrel';
var equipID = "FlabSquirrel";
var devicePassword = '1';
var certificatePath = 'C:\\temp\\';

var sfDeviceClient = SfDeviceClient.createSfDeviceClient(deviceID, devicePassword, certificatePath);

//define sensor value
var status = 0;
var messageCatalogId = 30;
serialPort.on("open", function() {

    // By omitting the base `elevation` property, the values
    // received will be relative to your present elevation
    console.log("ready");
    console.log('serial connected!');
    console.log('Connecting Azure...');
    var connectCallback = function(err) {
        if (err) {
            console.error('Could not connect: ' + err.message);

        } else {
            //client listen to cloud
            sfDeviceClient.on("message", function(msg) {
				console.log('Id: ' + msg.messageId + ' Body: ' + msg.data);
                var jsonSensor = JSON.parse(msg.data);
                var command = jsonSensor["command"];
                if (command == "appear" && status == 0) {
                    console.log("Appear");
                    serialPort.write('3'); //send order 2 arduino
                }
                sfDeviceClient.complete(msg, printResultFor('completed,'));

            });
            serialPort.on('data', function(data) {
                //console.log(data);
                //start job
                if (data == 1) {
                    status = 1;
                    sendData();
                }
                //stop job
                else if (data == 0) {
                    status = 0;
                    sendData();
                }
            });
            var sendInterval = setInterval(function() {
                sendData();
            }, 1000);
				 sfDeviceClient.on('error', function (err) {
				console.error(err.message);
			});

			sfDeviceClient.on('disconnect', function () {
				clearInterval(sendInterval);
				sfDeviceClient.removeAllListeners();
				sfDeviceClient.open(connectCallback);
			});
		}
    };
    //open iot hub client
	sfDeviceClient.open(connectCallback);
    console.log("open iot hub client");

    function printResultFor(op) {
        return function printResult(err, res) {
            if (err) console.log(op + ' error: ' + err.toString());
            if (res) console.log(op + ' status: ' + res.constructor.name);
        };
    }

    function getDateTime() {
        var date = new Date();
        //var offset = 8 * 60 * 60 * 1000;
        //var date_utc8 = new Date(date.getTime() + offset);
        return date.toISOString().replace(/\..+/, 'Z');
    }

    function sendData() {

        var now = getDateTime();
		var curAction="";
		if(status==1){
			curAction="Moving";
		}else{
			curAction="Standby";
		}
		
        var deviceMessage = {
			companyId:5,
			msgTimestamp:getDateTime(),
			equipmentId:equipID,
			equipmentRunStatus:1,
			squirrelActionStatus:curAction
        };
		console.log(deviceMessage);
        sfDeviceClient.sendEvent(messageCatalogId, deviceMessage, printResultFor('send'));
    }

});
