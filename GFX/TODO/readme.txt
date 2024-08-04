"0x2A3DB4_file0008_PauseMenu.bin" should be the initial sprites for the pause menu.
Also has the bookmark types.

"0x2AABC4_file0018_PauseMenuLess.bin" has the pause menu titles again, they are used for highlights and restores.

In order to swap the sprites between 'shiori' and 'playerdata' to make enough space for 'bookmarks' write the following to the rom:

Patch 0x611AC with 0x9E to swap shiori with playerdata.
Patch 0x611CE with 0x04 to extend sprite for replaced shiori.

Patch 0x61254 with 0x88 to swap playerdata with shiori.
Patch 0x61276 with 0x02 to reduce sprite for replaced playerdata.

Adjust x pos, patch 0x6119E with 0x80, and 0x61246 with 0xB6.
