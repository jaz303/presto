var bindings    = require("bindings")("presto_bindings.node"),
    k           = bindings.installConstants({});

function x() {
    for (var i = 0; i < arguments.length; ++i) {
        exports[arguments[i]] = bindings[arguments[i]];
    }
}

// Map JS event names to native Allegro event types
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

//
// Init/loop

function installSubSystems(systems) {

    if (bindings.ps_install() === false)
        throw "error initialising Allegro";

    if (systems.joystick && bindings.ps_install_joystick() === false)
        throw "error initialising joystick";

    if (systems.keyboard && bindings.ps_install_keyboard() === false)
        throw "error initialising keyboard";

    if (systems.mouse && bindings.ps_install_mouse() === false)
        throw "error initialising mouse";

    if (systems.audio && bindings.ps_install_audio() === false)
        throw "error initialising audio add-on";

    if (systems.font && bindings.ps_install_font() === false)
        throw "error initialising font add-on";

    if (systems.image && bindings.ps_install_image() === false)
        throw "error initialising image I/O add-on";

    if (systems.primitives && bindings.ps_install_primitives() === false)
        throw "error initialising primitives add-on";

}

function uninstallSubSystems(systems) {
    
    systems.primitives  && bindings.ps_uninstall_primitives();
    systems.image       && bindings.ps_uninstall_image();
    systems.font        && bindings.ps_uninstall_font();
    systems.audio       && bindings.ps_uninstall_audio();
    systems.mouse       && bindings.ps_uninstall_mouse();
    systems.keyboard    && bindings.ps_uninstall_keyboard();
    systems.joystick    && bindings.ps_uninstall_joystick();

    bindings.ps_uninstall();

}

function createContext() {

    var queue = bindings.createEventQueue();
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

function init() {

    var args = Array.prototype.slice.call(arguments, 0);

    function makeSubSystemMap(list) {
        var map = {},
            add = function(k) { map[k] = (list.indexOf(k) >= 0) || (list.length === 0); };

        add('joystick');
        add('mouse');
        add('keyboard');
        add('mouse');
        add('audio');
        add('font');
        add('image');
        add('primitives');

        return map;
    }

    var subSystems = makeSubSystemMap(args);

    installSubSystems(subSystems);

    return exports;
    
}

exports.init                = init;
exports.createContext       = createContext;

for (var kname in k) {
    var kval = k[kname];
    Object.defineProperty(exports, kname, {
        enumerable  : true,
        writable    : false,
        value       : k[kname]
    });
}

//
// Display

function createDisplay(width, height, opts) {

    opts = opts || {};

    var flags = 0;

    var mode = opts.mode || 'window';

    if (mode === 'window') {
        flags |= k.WINDOWED;
    } else if (mode === 'fullscreen') {
        flags |= k.FULLSCREEN;
    } else if (mode === 'fullscreen-window') {
        flags |= k.WINDOWED;
        flags |= k.FULLSCREEN_WINDOW;
    } else {
        throw "invalid mode; valid modes are: 'window', 'fullscreen' or 'fullscreen-window'"
    }

    if (opts.resizable) flags |= k.RESIZABLE;
    if (opts.openGL)    flags |= k.OPENGL;
    if (opts.noFrame)   flags |= k.NOFRAME;

    var args = [parseInt(width, 10), parseInt(height, 10), flags];
    if ('x' in opts && 'y' in opts) {
        args.push(parseInt(opts.x, 10));
        args.push(parseInt(opts.y, 10));
    }

    var display = bindings.createDisplay.apply(bindings, args);

    if ('title' in opts) {
        display.setTitle(opts.title);
    }

    return display;

}

exports.createDisplay       = createDisplay;
exports.inhibitScreensaver  = bindings.inhibitScreensaver;

//
// Events

x('createEventQueue');

//
// Bitmaps

function createBitmap(width, height, format, flags) {

    function parseCreateBitmapOptions(opts) {
        format = ('format' in opts) ? parseInt(opts.format, 10) : undefined;
        flags =  ('flags' in opts)  ? parseInt(opts.flags, 10)  : undefined;
    }

    if (typeof format === 'object') {
        parseCreateBitmapOptions(format);
    }

    if (typeof format !== 'number')
        format = k.PIXEL_FORMAT_ANY;

    if (typeof flags !== 'number')
        flags = k.VIDEO_BITMAP;

    return bindings.createBitmap(width, height, format, flags);

}

exports.createBitmap        = createBitmap;
exports.loadBitmap          = bindings.loadBitmap;

//
// Target

x(  'getTargetBitmap',
    'setTargetBitmap',
    'flip',
    'clear',
    'drawPixel',
    'putPixel',
    'putBlendedPixel',
    'getClippingRectangle',
    'setClippingRectangle',
    'resetClippingRectangle',
    'setBlender',
    'isBitmapCompatibleWithCurrentDisplay',
    'deferDrawing'  );

//
// Primitives

x(  'drawLine',
    'drawTriangle',
    'drawFilledTriangle',
    'drawRectangle',
    'drawFilledRectangle',
    'drawRoundedRectangle',
    'drawFilledRoundedRectangle',
    'drawEllipse',
    'drawFilledEllipse',
    'drawCircle',
    'drawFilledCircle',
    'drawArc',
    'drawSpline',
    'drawRibbon'    );

//
// Colors

function rgb(r, g, b) {
    return (0xFF << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

function rgbf(r, g, b) {
    return {r: r, g: g, b: b};
}

function rgba(r, g, b, a) {
    return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

function rgbaf(r, g, b, a) {
    return {r: r, g: g, b: b, a: a};
}

exports.rgb     = rgb;
exports.rgba    = rgba;
exports.rgbf    = rgbf;
exports.rgbaf   = rgbaf;