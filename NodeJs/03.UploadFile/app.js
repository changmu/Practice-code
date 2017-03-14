var http = require('http');
var fs = require('fs');

http.createServer(function(request, response) {
    var newFile = fs.createWriteStream('file.txt');
    request.pipe(newFile);
    request.on('end', function() {
        response.end('upload succeed.');
    });
}).listen(8080);

console.log('listening on 8080');