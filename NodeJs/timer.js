var EventEmitter = require('events').EventEmitter;
var event = new EventEmitter();

var timerBegin = function() {
    setTimeout(timerExpired, 2000);
};

event.on('timerBegin', timerBegin);

var timerExpired = function()
{
    console.log('hello');
    event.emit('timerBegin');
};

timerBegin();
