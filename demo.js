var presto = require('./');

// Initialise the specified Allegro subsystems then run the callback
// on Allegro's thread.
presto.main('keyboard', 'mouse', 'audio', function(game) {

    var display = presto.createDisplay(800, 600, {fullScreen: false});

    var frames = 0;

    game.on('keydown', function() {
        game.exit();
    });

    game.on('tick', function(delta) {
        console.log("tickin'", delta);
        if (++frames == 200) {
            game.exit();
        }
        // display.use();
        // display.clear('black');
    });

    // Sets internal tick function to run at specified FPS
    // Every internal tick performs the following:
    // 1. drains the main event queue, dispatching events to any registered handlers
    // 2. emits the 'tick' event
    // The game loop will continue to run until `game.exit()` is called.
    game.run(60);

});
