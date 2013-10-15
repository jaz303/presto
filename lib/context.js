// TODO: this should be an entirely different module

var ps  = require('./api'),
    k   = require('./constants');

var EVENT_HANDLERS = {
    'joystickmove'          : k.EVENT_JOYSTICK_AXIS,
    'joystickdown'          : k.EVENT_JOYSTICK_BUTTON_DOWN,
    'joystickup'            : k.EVENT_JOYSTICK_BUTTON_UP,
    'joystickconfig'        : k.EVENT_JOYSTICK_CONFIGURATION,
    'keydown'               : k.EVENT_KEY_DOWN,
    'keyup'                 : k.EVENT_KEY_UP,
    'keychar'               : k.EVENT_KEY_CHAR,
    'mousemove'             : k.EVENT_MOUSE_AXES,
    'mousewarp'             : k.EVENT_MOUSE_WARPED,
    'mousedown'             : k.EVENT_MOUSE_BUTTON_DOWN,
    'mouseup'               : k.EVENT_MOUSE_BUTTON_UP,
    'mouseenter'            : k.EVENT_MOUSE_ENTER_DISPLAY,
    'mouseleave'            : k.EVENT_MOUSE_LEAVE_DISPLAY,
    'displayexpose'         : k.EVENT_DISPLAY_EXPOSE,
    'displayresize'         : k.EVENT_DISPLAY_RESIZE,
    'displayclose'          : k.EVENT_DISPLAY_CLOSE,
    'displaylost'           : k.EVENT_DISPLAY_LOST,
    'displayfound'          : k.EVENT_DISPLAY_FOUND,
    'displayswitchout'      : k.EVENT_DISPLAY_SWITCH_OUT,
    'displayswitchin'       : k.EVENT_DISPLAY_SWITCH_IN,
    'displayorientation'    : k.EVENT_DISPLAY_ORIENTATION
};

function createContext() {

    var queue = ps.createEventQueue();
    queue.installMouseEventSource();
    queue.installKeyboardEventSource();

    var tickFn = function(delta) {};
    var running = false;

    var handlers = {};
    for (var k in EVENT_HANDLERS) {
        handlers[EVENT_HANDLERS[k]] = [];
    }

    function tick(delta) {
        var evt = queue.getNextEvent();
        while (evt !== null) {
            handlers[evt.type].forEach(function(cb) { cb(evt); });
            evt = queue.getNextEvent();
        }
        if (running) {
            tickFn(delta);    
        }
    }

    var game = {queue: queue};

    game.exit = function() {
        running = false;
    }

    game.on = function(ev, handler) {
        if (ev === 'tick') {
            tickFn = handler;
        } else {
            var nativeEventType = EVENT_HANDLERS[ev];
            if (!nativeEventType) {
                throw new Error("unknown event type: " + ev);
            }
            handlers[nativeEventType].push(handler);
        }
    }

    game.run = function(fps) {
        running = true;
        var timer = setInterval(function() {
            if (!running) {
                clearInterval(timer);
            } else {
                tick(1000 / fps);    
            }
        }, 1000 / fps);
    }

    return game;

}

exports.createContext = createContext;