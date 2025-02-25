var t = JSON.parse(msg.payload);
var h = t.object.Payload;
var hum = parseInt(h.substr(4, 4), 16);

msg.payload = parseFloat(hum / 10)
    
return msg;