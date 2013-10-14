var ps = require('../').init();

var game = ps.createContext();

var fs = require('fs');

var display = ps.createDisplay(800, 600, {resizable: false, title: "Hey Presto"});

var frames = 0;

var sprites = function() {
    
    var lb      = ps.loadBitmap,
        base    = __dirname + '/gfxlib-fuzed/Sprites/';

    var toLoad = {
        'hero.run_left'     : { src: 'gripe.run_left.png', mask: true },
        'hero.run_right'    : { src: 'gripe.run_right.png', mask: true }
    }

    var out = {};
    for (var k in toLoad) {
        var spec    = toLoad[k],
            sprite  = ps.loadBitmap(base + spec.src);
        if (spec.mask) {
            sprite.convertMaskToAlpha(ps.rgb(0xFF, 0x00, 0xFF));
        }
        out[k] = sprite;
    }
    
    return out;

}();

game.on('keydown', function(ev) {
    
});

game.on('mousemove', function(ev) {

});

game.on('mousedown', function(ev) {
    console.log(ev);
});

var frame = 0;

game.on('tick', function(delta) {

    display.use();

    ps.clearToColor(ps.rgbf(0,0,0));

    frame++;
    var ix = (Math.floor(frame / 5)) % 8;

    sprites['hero.run_left'].drawRegion(ix * 32, 0, 32, 32, 10, 10);
    sprites['hero.run_left'].drawRegion(ix * 32, 0, 32, 32, 50, 50);

    display.flip();

});

// Sets internal tick function to run at specified FPS
// Every internal tick performs the following:
// 1. drains the main event queue, dispatching events to any registered handlers
// 2. emits the 'tick' event
// The game loop will continue to run until `game.exit()` is called.
game.run(60);