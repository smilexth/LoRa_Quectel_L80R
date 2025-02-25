var m = {};
var t = JSON.parse(msg.payload);

m.payload = {
    
        "Payload": t.object.Payload,
    }
    
return m;