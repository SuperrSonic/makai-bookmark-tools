# Makai no Bookmark Translation Tools

Makai AIO is a Windows command-line program.
To run it you need to open command prompt, use the cd command to enter the dir. where Makai AIO is stored.


To encode a script from the repo, do the following:

makai_aio.exe -o "script.bin" -l English -e "script.txt"

This means that the output is named script.bin, the language code to parse is "en", and that you want to encode the file script.txt, it should print your settings on the console and begin the conversion process.

You can also do: makai_aio -t "This is a..."

To print the raw values to the console.


Encoding a script is also useful to test every line for the dialogue screen limit.

Even though there's no way to tell what line is character dialogue, the console will print the limit regardless.
It relies on checking if a character is FW (14px) or HW (7px).


Makai AIO can also patch a ROM by reading the assets from this repo.
To use this feature, run Makai AIO from the root/initial dir of the repo:
"makai_aio -i (path to the ROM)" this will report everything being overwritten or repointed,
and output makai_patched.gba to the same directory Makai AIO is in.

Alternatively, move makai_aio.exe to the root, and drag-n-drop the ROM into "patch_rom.cmd" to avoid using command prompt.
