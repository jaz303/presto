var presto = require('./').init();

var game = presto.createContext();

var fs = require('fs');

var display = presto.createDisplay(300, 600, {resizable: false, x: 0, y: 0, title: "My First Game"});
// display.resize(400, 400);
// display.setPosition(0, 0);

var frames = 0;

var image = presto.loadBitmap('guardian.jpg');
console.log(image);

game.on('keydown', function() {
    game.exit();
});

game.on('tick', function(delta) {

    //console.log(display.x);
    

    // fs.readFile('README.md', function(err, data) {
    //     console.log("file contents: " + data);
    //     game.exit();
    // });


    // display.use();
    // display.clear('black');
});

// Sets internal tick function to run at specified FPS
// Every internal tick performs the following:
// 1. drains the main event queue, dispatching events to any registered handlers
// 2. emits the 'tick' event
// The game loop will continue to run until `game.exit()` is called.
game.run(60);