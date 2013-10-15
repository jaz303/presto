var bindings    = require("bindings")("presto_bindings.node"),
    k           = require('./constants');

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

exports.createBitmap    = createBitmap;
exports.loadBitmap      = bindings.loadBitmap;