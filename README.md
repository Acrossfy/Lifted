# Lifted

Attempt at integrating and demonstrating gamelift into UE4.

Note that you need to be using a source build to build a dedicated server, it can't be done using the launcher version.

Paths are hardcoded into my build scripts, you'll need to update them.

Windows support only for right now.

Execute buildserver.bat to build the dedicated server build.
Then execute foramazon.ps1 to create the folder structure for gamelift, it will be at PackagedServer\ForAmazon.

Upload to gamelift using the aws cli, as in the Amazon docs:
https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-build-cli-uploading.html

Issues I ran into that I managed to solve:
* UE4 won't start because the amazon windows VM template doesn't have the right dependencies. 
Solved by packaging the UE4PrereqSetup_x64.exe installer and executing it at setup time.
* Gamelift doesn't allow you to change the firewall / access rules for a fleet after it's been created until it has fully started up and is in a ready status.
This is a real pain when trying to debug a server that won't start. When you create your fleet, be sure to add the access rules for remote desktop, as well as the UE4 ports.



Issues I ran into that I didn't manage to solve:
* There's no way to cancel or delete a fleet that isn't in a clean state either. If your fleet fails to start, you have to just let it keep going until Gamelift decides that it has failed, which takes several hours.
* The gamelift infrastructure claims that the process doesn't call InitSDK. My logs show that it definitely is called, and returns success.
Connecting to the game server from a client using "open <ip>" works! But Gamelift wasn't working.
