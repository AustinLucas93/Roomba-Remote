function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}

// Function to send a message to the Pebble using AppMessage API
function sendMessage() {
	Pebble.sendAppMessage({"status": 0});
	
	// PRO TIP: If you are sending more than one message, or a complex set of messages, 
	// it is important that you setup an ackHandler and a nackHandler and call 
	// Pebble.sendAppMessage({ /* Message here */ }, ackHandler, nackHandler), which 
	// will designate the ackHandler and nackHandler that will be called upon the Pebble 
	// ack-ing or nack-ing the message you just sent. The specified nackHandler will 
	// also be called if your message send attempt times out.
}


// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
							function(e) {     
                if (e.payload.message == "S"){
                  Pebble.showSimpleNotificationOnPebble("Select Key"," Received "); 
                } else if (e.payload.message == "U") {
                  HTTPGET("http://192.168.1.2/L");
                } else if (e.payload.message == "D"){
                  HTTPGET("http://192.168.1.2/R");
                }
                
								console.log("Received Status: " + e.payload.status);
                //Pebble.showSimpleNotificationOnPebble("Payload Notification", e.payload.message);
                
                //HTTPGET("http://192.168.1.2/F");
                sendMessage();
							});