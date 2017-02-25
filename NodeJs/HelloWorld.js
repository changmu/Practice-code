var net = require("net");

var server = net.createServer(function (socket) {
    socket.setNodelay();
    socket.write("hello world of nodeJs.\n");
});
server.listen(8086);
