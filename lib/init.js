var bindings    = require("bindings")("presto_bindings.node"),
    k           = require('./constants');

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

function init() {

    var args = Array.prototype.slice.call(arguments, 0);

    function makeSubSystemMap(list) {
        var map = {},
            add = function(k) { map[k] = (list.indexOf(k) >= 0) || (list.length === 0); };

        add(k.SYSTEM_JOYSTICK);
        add(k.SYSTEM_MOUSE);
        add(k.SYSTEM_KEYBOARD);
        add(k.SYSTEM_AUDIO);
        add(k.SYSTEM_FONT);
        add(k.SYSTEM_IMAGE);
        add(k.SYSTEM_PRIMITIVES);

        return map;
    }

    var subSystems = makeSubSystemMap(args);

    installSubSystems(subSystems);

    return true;
    
}

exports.init = init;