Design is probably the wrong word. The goal of this document is to, uh,
DOCUMENT the steps I plan to take to get this program up and running.
This is expected to be a living document subject to frequent
modification (and delusional goals).

<h2>Next steps</h2>

<ol>
    <li>Add player 'shoot' capability</li>
    <li>Add pogo stick jumping (and idling)</li>
    <li>Add hanging from ledge</li>
    <li>Add pole climbing</li>
    <li>Shoot from poles</li>
    <li>"Jump down" from platformers that have top but not bottom collision</li>
    <li>Add sloped tiles and their collision properties</li>
    <li>Add camera scrolling (make level bigger than camera)</li>
    <li>Look up/down</li>
    <li>Write basic enemy AI (animation optional)</li>
    <li>Pit the player against the basic enemy AI</li>
</ol>

<h2>Noteworthy progress</h2>

Level editor outputs a file that the game then reads in. Tile placement and tile collision info are included.
Level editor is also capable of loading saved files and overwriting them.

Left/right movement and jump implemented.

Basic collision implemented against the static tiles. No slopes yet, no moving objects yet.

Basic physics (gravity) implemented.
