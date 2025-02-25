function Decode(fPort, bytes) {

    var b = [];
    var n = bytes.length;
    var i;
    var payload = "";
    for (i = 0; i < n; i++) {
        b[i] = bytes[i];
        b[i] = b[i].toString(16);
        b[i] = (b[i].toString().length < 2 ? "0" + b[i] : b[i]).toString();
        payload += b[i];
    }

  
    var temp = b[0].toString(16) + b[1].toString(16);
    temp = parseInt(temp, 16) / 100;
  
    var hum = b[2].toString(16) + b[3].toString(16);
    hum = parseInt(hum, 16) / 100;
  
    return {
      temperature:temp,
      humidity:hum
    };
}