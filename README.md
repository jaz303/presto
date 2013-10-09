# presto

[Allegro 5](http://alleg.sourceforge.net/) bindings for node.

This is completely unfinished and in flux. It is, however, somewhat functional.

## Project Aims

To provide a simple and idiomatic interface to the Allegro game-programming library.

## Installation

  * Install Allegro 5
  * <del>`npm install presto`</del>

As it turns out we need a patched version of node because Allegro requires thread-hoisting gymnastics and `uv_run()` is not officially re-entrant. I'm not saying it's unsolvable with vanilla node but I'd already spent way too long on it when the working solution is "good enough". I'll create an official patched version of node later, and probably build the presto extensions into it directly. From there I'll create a binary `.app` distribution similar to Lua's Love engine.

## Example

This is what it will look like:

    var presto = require('presto').init();

    var game = presto.createContext();

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

## TODO

  * <del>Initialise Allegro</del>
  * <del>Integrate Allegro event loop with libuv</del>
  * <del>Event queues</del>
  * <del>System</del>
  * <del>Display</del>
  * <del>Colors</del>
  * <del>Bitmaps</del>
  * <del>Image I/O</del>
  * Primitives
  * Fonts
  * Mouse
  * Joystick
  * Keyboard
  * OpenGL
  * Audio
  * Native dialogs

## Cut Corners

I've skipped implementing some stuff that's not immediately useful to me. None of this is hard to implement, just tedious:

  * `al_set_new_display_option()` and associated constants
  * `as_set_new_display_refresh_rate()`
  * `al_get_current_display()`
  * `al_lock_bitmap()`, `al_lock_bitmap_region()`, `al_unlock_bitmap()`
  * `al_get_pixel_size()`, `al_get_pixel_format_bits()`
  * A few of the display flags are not recognised at the JS wrapper level: `ALLEGRO_OPENGL_3_0`, `ALLEGRO_OPENGL_FORWARD_COMPATIBLE`, `ALLEGRO_GENERATE_EXPOSE_EVENTS`
  * Enumeration and interrogation of available display modes
  * There are a lot of missing constants
  * Image I/O works with filenames only, not file descriptors
  * PNG/JPG saving is untested - I don't seem to have support compiled into Allegro

## Limitations

Presto does not, and probably will never, support the following Allegro features:

  * Timers
  * User events
  * Configuration files
  * Time
  * UTF-8
  * Filesystem
  * File IO
  * Memory
  * Path
  * Direct3D

Most of the above are well addressed by existing npm modules - use these instead!

The following features are not currently supported, but may be added in the future. Patches gladly accepted!

  * OpenGL
  * Fixed point math
  * Monitors
  * Threads (with some suitable abstraction)
