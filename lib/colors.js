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