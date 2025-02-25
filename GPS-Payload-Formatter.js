function decodeUplink(input) {
    // Get raw bytes
    var bytes = input.bytes;
  
    // Check we have at least 8 bytes
    if (bytes.length < 8) {
      return {
        errors: ["Not enough bytes. Expected 8."]
      };
    }
  
    // Decode 4 bytes into a signed 32-bit integer for latitude
    var rawLat = (bytes[0] << 24) | 
                 (bytes[1] << 16) | 
                 (bytes[2] << 8)  | 
                 (bytes[3]);
                 
    // Decode 4 bytes into a signed 32-bit integer for longitude
    var rawLon = (bytes[4] << 24) | 
                 (bytes[5] << 16) | 
                 (bytes[6] << 8)  | 
                 (bytes[7]);
  
    // Convert to decimal degrees
    var lat = rawLat / 1e5;
    var lon = rawLon / 1e5;
  
    // Return an object with meaningful field names
    return {
      data: {
        latitude:  lat,
        longitude: lon
      }
    };
  }
  