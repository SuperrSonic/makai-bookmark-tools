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
It relies on checking if a character is FW (14px) or HW (7px) though some FW characters (like some bracket symbols) might be less than 14px.
