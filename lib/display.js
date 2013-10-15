var bindings    = require("bindings")("presto_bindings.node"),
    k           = require('./constants');

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