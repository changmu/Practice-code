var fs = require("fs");
var net = require("net");

var LOCAL_PORT = 8181;
var timeout = 30000;

fs.readFile("/etc/lbase.conf", "utf-8", function (err, data) {
	if (err) {
		console.log("read lbase.conf error");
	} else {
		try {
			var lbase = JSON.parse(data);
            
            console.log(lbase);

			var server = net.createServer(function (socket) {

				socket.setNoDelay();
				socket.setTimeout(timeout, function () {
					socket.destroy();
				});

				var isRemoteConnected = false;

				var serviceSocket = new net.Socket();

				socket.on("data", function(msg) {
					if (isRemoteConnected == false) {
						try {
							var len = msg.readUInt32LE(0);
							var jsonMsg = msg.toString("utf-8", 4);
							var handshakeReq = JSON.parse(jsonMsg);

                            console.log(jsonMsg);

							if (handshakeReq["addr"] === lbase["addr"]) {

								var remoteAddr = handshakeReq["ip_target"];
								var remotePort = parseInt(handshakeReq["port_target"]);
								var localAddr = handshakeReq["ip_src"];

								serviceSocket.connect({"port": remotePort, "host": remoteAddr, "localAddress": localAddr}, function () {
									console.log("remote connetion ok");
									serviceSocket.setNoDelay();
									isRemoteConnected = true;
									var buffer = new Buffer(6);
									buffer.writeUInt32LE(6, 0);
									buffer.write("{}", 4);
									socket.write(buffer);
								});

								serviceSocket.on("data", function (data) {
									socket.write(data);
								});

								serviceSocket.on("close", function() {
									socket.destroy();
								});

								serviceSocket.on("error", function() {
								});

							} else {
								if (lbase["addr"].substr(0, lbase["addr"].indexOf("-")) == 
										handshakeReq["addr"].substr(0, handshakeReq["addr"].indexOf("-"))) {
									fs.readFile("/dev/shm/route/lb_node_route.txt", "utf-8", function (err, data) {
										if (err) {
											console.log("read lb_node_route.txt error");
											socket.destroy();
										} else {
											var lines = data.split("\n");
											for (var i = 0; i < lines.length; i++) {
												var words = lines[i].split(" ");
												if (words[3] == handshakeReq["addr"] && words[5] == 2) {

													var remoteAddr = words[13];
													var remotePort = LOCAL_PORT;
													var localAddr = words[11];

													serviceSocket.connect(
															{"port": remotePort, "host": remoteAddr, "localAddress": localAddr}, 
															function () {
																serviceSocket.setNoDelay();
																isRemoteConnected = true;
																serviceSocket.write(msg);
															});

													serviceSocket.on("data", function (data) {
														socket.write(data);
													});

													serviceSocket.on("close", function() {
														socket.destroy();
													});

													serviceSocket.on("error", function() {
													});
												}
											}
										}
									});
								} else {
									fs.readFile("/dev/shm/route/lb_net_route.txt", "utf-8", function (err, data) {
										if (err) {
											console.log("read lb_net_route.txt error");
											socket.destroy();
										} else {
											var high_found = false;

											var lines = data.split("\n");

											for (var i = 0; i < lines.length; i++) {
												var words = lines[i].split(" ");
												if (words[3] == handshakeReq["addr"].substr(0, handshakeReq["addr"].indexOf("-")) && words[5] == 2) {

													remoteAddr = words[13];
													remotePort = LOCAL_PORT;
													localAddr = words[11];
													high_found = true;

													serviceSocket.connect(
															{"port": remotePort, "host": remoteAddr, "localAddress": localAddr}, 
															function () {
																serviceSocket.setNoDelay();
																isRemoteConnected = true;
																serviceSocket.write(msg);
															});

													serviceSocket.on("data", function (data) {
														socket.write(data);
													});

													serviceSocket.on("close", function() {
														socket.destroy();
													});

													serviceSocket.on("error", function() {
													});
												}
											}

											if (high_found == false) {
												for (var i = 0; i < lines.length; i++) {
													var words = lines[i].split(" ");
													if (words[3] == handshakeReq["addr"].substr(0, handshakeReq["addr"].indexOf("-"))) {
														remoteAddr = words[13];
														remotePort = LOCAL_PORT;
														localAddr = words[11];
														high_found = true;
														serviceSocket.connect(
																{"port": remotePort, "host": remoteAddr, "localAddress": localAddr}, 
																function () {
																	serviceSocket.setNoDelay();
																	isRemoteConnected = true;
																	serviceSocket.write(msg);
																});
													}
												}
											}
										}
									});
								}
							}
						} catch (e)
						{
						}
					} else {
						serviceSocket.write(msg);
					}
				});

				socket.on("close", function() {
					serviceSocket.destroy();
				});

				socket.on("error", function() {
				});
			});

			server.listen(LOCAL_PORT);

			console.log("server listening on " + LOCAL_PORT);
		} catch (e) {
			console.log("parse lbase.conf error");
		}
	}
});
