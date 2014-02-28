# Blizzard Installer Generator

### Quick Start

1. Put things to install in the "data" dir
2. Open "res/data" dir and edit patch.cmd to set the target and source version if updating the wow.exe 
3. Open "res/data" dir and edit patch.html and patch.txt with your custom notes.
2. run "createInstaller.bat"

### rules.txt

This file holds the ruleset for listgenerator. its format is pretty simple:
```
<realpath>;<replacement>;<patch>
```

## Making a patch for use with launcher

Make a new MPQ and add your Blizzard Installer to it.
Make a file called "prepatch.lst". Add these two lines to it:
(Replace test.exe with your installer name)

extract test.exe
execute test.exe

Add this file to the MPQ.

Now you can send this MPQ to people logging into the server.
