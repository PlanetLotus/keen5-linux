<h2>Next steps</h2>

<ul>
    <li>Add hanging from ledge</li>
    <li>"Jump down" from platforms that have top but not bottom collision</li>
    <li>Moving platforms</li>
</ul>

<h2>Noteworthy progress</h2>

Level editor outputs a file that the game then reads in. Tile placement and tile collision info are included.
Level editor is also capable of loading saved files and overwriting them.

Left/right movement, look up/down, jump, pogo, climb, shoot, and death implemented with animations.

Camera scrolling - enables levels larger than the screen. Only actually renders what fits on the screen to save on processing cycles.

Basic collision implemented against the static tiles. Sloped tiles are also mostly working but have a few visual problems.

Physics implemented using floats for integrating position and ints for rendering position.

One enemy with basic AI implemented.
