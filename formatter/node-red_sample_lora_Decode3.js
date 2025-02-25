var m = {};
var t = JSON.parse(msg.payload);
var h = t.object.Payload;
var temp = parseInt(h.substr(0, 4), 16);
var hum = parseInt(h.substr(4, 4), 16);

m.payload = [
    {
        measurement: "payload",
        fields: {
            "Payload": t.object.Payload,
            "hum": parseFloat(hum / 10),
            "temp": parseFloat(temp / 10)
    }, tags: {
        "applicationID": t.applicationID,
        "applicationName": t.applicationName,
        "deviceName": t.deviceName,
        "devEUI": t.devEUI,
        "gatewayID": t.rxInfo[0].gatewayID,
        "uplinkID": t.rxInfo[0].uplinkID,
        "gwname": t.rxInfo[0].name,
        "gwtime": t.rxInfo[0].time,
        "rssi": t.rxInfo[0].rssi,
        "snr": t.rxInfo[0].loRaSNR,
        "gatewayLatitude": t.rxInfo[0].location.latitude,
        "gatewayLongitude": t.rxInfo[0].location.longitude,
        "gatewayAltitude": t.rxInfo[0].location.altitude,

        "frequency": t.txInfo.frequency,
        "dr": t.txInfo.dr,
        "adr": t.adr,
        "fCnt": t.fCnt,
        "fPort": t.fPort,
        "Data": t.data
    }}
];
    
return m;