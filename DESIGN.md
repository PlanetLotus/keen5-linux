Design is probably the wrong word. The goal of this document is to, uh,
DOCUMENT the steps I plan to take to get this program up and running.
This is expected to be a living document subject to frequent
modification (and delusional goals).

<h2>Next steps</h2>

<ul>
    <li>Add hanging from ledge</li>
    <li>"Jump down" from platformers that have top but not bottom collision</li>
    <li>Add sloped tiles and their collision properties</li>
    <li>Add camera scrolling (make level bigger than camera)</li>
    <li>Look up/down</li>
    <li>Write basic enemy AI (animation optional)</li>
    <li>Pit the player against the basic enemy AI</li>
</ul>

<h2>Noteworthy progress</h2>

Level editor outputs a file that the game then reads in. Tile placement and tile collision info are included.
Level editor is also capable of loading saved files and overwriting them.

Left/right movement, jump, pogo, climb and shoot implemented with animations.

Basic collision implemented against the static tiles. No slopes yet, no moving objects yet.

Physics implemented using floats for integrating position and ints for rendering position.
