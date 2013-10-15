function merge(lib) {
    var exp = require('./' + lib);
    for (var k in exp) {
        exports[k] = exp[k];
    }
}

merge('bitmap');
merge('colors');
merge('context');
merge('display');
merge('event_queue');
merge('init');
merge('keyboard');
merge('mouse');
merge('primitives');
merge('target');

var k = require('./constants');

for (var kname in k) {
    var kval = k[kname];
    Object.defineProperty(exports, kname, {
        enumerable  : true,
        writable    : false,
        value       : k[kname]
    });
}