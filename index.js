var bindings    = require("bindings")("presto_bindings.node"),
    k           = bindings.installConstants({});

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
    var tickFn = function(delta) {};
    var running = false;

    queue.installMouseEventSource();
    queue.installKeyboardEventSource();

    function tick(delta) {
        var evt = queue.getNextEvent();
        while (evt !== null) {
            console.log(evt);
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

    game.on = function(event, handler) {
        if (event === 'tick') {
            tickFn = handler;
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

function createDisplay(width, height, opts) {

    opts = opts || {};

    var flags = 0;

    var mode = opts.mode || 'window';

    if (mode === 'window') {
        flags |= k.ALLEGRO_WINDOWED;
    } else if (mode === 'fullscreen') {
        flags |= k.ALLEGRO_FULLSCREEN;
    } else if (mode === 'fullscreen-window') {
        flags |= k.ALLEGRO_WINDOWED;
        flags |= k.ALLEGRO_FULLSCREEN_WINDOW;
    } else {
        throw "invalid mode; valid modes are: 'window', 'fullscreen' or 'fullscreen-window'"
    }

    if (opts.resizable) flags |= k.ALLEGRO_RESIZABLE;
    if (opts.openGL)    flags |= k.ALLEGRO_OPENGL;
    if (opts.noFrame)   flags |= k.ALLEGRO_NOFRAME;

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

//
//

exports.constants           = k;
exports.init                = init;
exports.createContext       = createContext;

//
// Display

exports.createDisplay       = createDisplay;
exports.inhibitScreensaver  = bindings.inhibitScreensaver;

//
// Events

exports.createEventQueue    = bindings.createEventQueue;