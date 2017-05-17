'use strict';

var iothub = require('azure-iothub');

var connectionString = 'HostName=LucasRicciardi.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=VG//Rx8COy/gNr7HzHs/+C43XKqt1kPXBPYum94FaeI=';
var registry = iothub.Registry.fromConnectionString(connectionString);

var device = new iothub.Device(null);
device.deviceId = 'myFirstNodeDevice';
registry.create(device, function(err, deviceInfo, res)
{
	if (err)
	{
		registry.get(device.deviceId, printDeviceInfo);
	}
	if (deviceInfo)
	{
		printDeviceInfo(err, deviceInfo, res);
	}
});

function printDeviceInfo(err, deviceInfo, res)
{
	if (deviceInfo)
	{
		console.log('Device ID: ' + deviceInfo.deviceId);
		console.log('Device Key: ' + deviceInfo.authentication.symmetricKey.primaryKey);
	}
}