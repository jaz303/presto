var bindings = require("bindings")("presto_bindings.node")

module.exports = function(exportz, fns) {
    for (var i = 0; i < fns.length; ++i) {
        exportz[fns[i]] = bindings[fns[i]];
    }
}
