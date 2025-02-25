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

    return {
        Payload: payload
    };
}