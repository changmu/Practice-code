var myserver = require('./myserver');
var router = require('./router');
var requestHandlers = require('./requestHandler');

var handle = {};
handle['/'] = requestHandlers.start;
handle['/start'] = requestHandlers.start;
handle['/upload'] = requestHandlers.upload;
handle['/show'] = requestHandlers.show;

myserver.start(router.route, handle);
