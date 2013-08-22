var bindings = require("bindings")("presto_bindings.node");

function installSubSystems(systems) {
    console.log("init systems: " + JSON.stringify(systems));
}

function uninstallSubSystems(systems) {
    console.log("deinit systems: " + JSON.stringify(systems));
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

        add('keyboard');
        add('mouse');
        add('audio');

        return map;
    }

    var fn          = args.pop(),
        subSystems  = makeSubSystemMap(args);

    bindings.run(function() {
        installSubSystems(subSystems);    
        try {
            fn(newContext());
        } finally {
            uninstallSubSystems(subSystems);    
        }
    });
}

function createDisplay(width, height, opts) {
    // TODO: call function on binding
}

exports.main = main;
exports.createDisplay = createDisplay;