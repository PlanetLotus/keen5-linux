<h2>Next steps</h2>

<ul>
    <li>Add items to level info</li>
    <li>Dropdown pause menu</li>
    <li>Upper-left corner HUD</li>
    <li>Stats: score, lives, ammo, vitalin</li>
    <li>Save/load game</li>
</ul>

<h2>Noteworthy progress</h2>

Level editor outputs a file that the game then reads in. Tile placement and tile collision info are included.
Level editor is also capable of loading saved files and overwriting them.

Completed the static foreground and background tiles for the first level of the game. Added unit info to the level.

Left/right movement, look up/down, jump, pogo, climb, shoot, hang from ledge, roll onto ledge, and death implemented with animations.

Camera scrolling - enables levels larger than the screen.

Basic collision implemented against the static tiles. Sloped tiles are also
mostly working but have a few visual problems. Moving platforms work fine but
have a couple minor problems. Can jump down from certain tiles and from moving
platforms.

Physics implemented using floats for integrating position and ints for rendering position.

Two enemies with basic AI implemented.
