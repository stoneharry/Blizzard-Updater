# Blizzard Installer Generator

### Quick Start

1. Put things to install in the "data" dir
2. run "createInstaller.bat"

### rules.txt

This file holds the ruleset for listgenerator. its format is pretty simple:
```
<realpath>;<replacement>;<patch>
```

## Old Stuff

1. Step

Open \MPQ

Go to patch.cmd. Open this with notepad. This should be self explanatory.
Go to patch.txt. Open this with notepad. This should be self explanatory.
Go to patch.html. Open this with notepad. This should be self explanatory.

Add your Item.DBC to \MPQ\Base\DBFilesClient
Add your WoW.exe to \MPQ\pc-game-hdfiles

Run HeaderGenerator.exe JUST ONCE. This will add a header to the Item.DBC and WoW.exe.

Now turn the contents of \MPQ into a MPQ.

2. Step

Bring your new MPQ to \Assemble folder.
Name your MPQ "mpq.mpq".
Run GenerateExe.bat.

Your Blizzard Installer should now work. You might want to test it.

3. Step

Make a new MPQ and add your Blizzard Installer to it.
Make a file called "prepatch.lst". Add these two lines to it:

extract test.exe
execute test.exe

test.exe = the name of your Blizzard Installer. Add this file to the MPQ.

Now you can send this MPQ to people logging into the server.
