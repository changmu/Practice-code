var http = require('http');

http.createServer(function(request, response) {
    response.writeHead(200);
    response.write("hello dog. running <br>");
    setTimeout(function() {
        response.write("hello dog. done\r\n");
        response.end();
    }, 2000);
}).listen(8080);

console.log("listening on 8080");