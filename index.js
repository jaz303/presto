var bindings = require("bindings")("presto_bindings.node");

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

function newContext() {

    // TODO: create event queue

    return {
        
        on: function(event, handler) {

        },

        run: function() {

        }
    
    };

}

function main() {

    var args = Array.prototype.slice.call(arguments, 0);

    if (args.length === 0 || typeof args[args.length-1] !== 'function') {
        throw "presto.main() requires a callback as its last argument";
    }

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

    var fn          = args.pop(),
        subSystems  = makeSubSystemMap(args);

    bindings.run(function() {
        try {
            installSubSystems(subSystems);
            fn(newContext());
        } finally {
            uninstallSubSystems(subSystems);    
        }
    });
}

function createDisplay(width, height, opts) {
    bindings.ps_create_display();
    // TODO: call function on binding
}

exports.main = main;

//
// Display

exports.createDisplay       = createDisplay;
exports.inhibitScreensaver  = bindings.ps_inhibit_screensaver;