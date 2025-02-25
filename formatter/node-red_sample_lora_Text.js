var m = {};
var t = JSON.parse(msg.payload);
var h = t.object.Payload;
var temp = parseInt(h.substr(0, 4), 16);
var hum = parseInt(h.substr(4, 4), 16);

m.payload = {

    "Payload": t.object.Payload,
    "hum": parseFloat(hum / 10),
    "temp": parseFloat(temp / 10),
    "rssi": t.rxInfo[0].rssi,
    "snr": t.rxInfo[0].loRaSNR
}

return m;