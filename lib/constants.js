var bindings = require("bindings")("presto_bindings.node");
    
bindings.installConstants(exports);

exports.SYSTEM_JOYSTICK     = 'joystick';
exports.SYSTEM_MOUSE        = 'mouse';
exports.SYSTEM_KEYBOARD     = 'keyboard';
exports.SYSTEM_AUDIO        = 'audio';
exports.SYSTEM_FONT         = 'font';
exports.SYSTEM_IMAGE        = 'image';
exports.SYSTEM_PRIMITIVES   = 'primitives';