# GTA Trilogy Chaos Mod: Reborn
Welcome to the official **GTA Trilogy Chaos Mod: Reborn** repository!

This repository, together with this README, is still very much **WIP**.

# Why "Reborn"?
The mod had some issues and was my first C++ project, thus I did not have a lot of experience on how to best do a lot of things right.

The **"Reborn"** project aims to fix this by pretty much **rewriting the majority of the mod from scratch.** (except code for drawing effects and a few other things)

It will also be easier for interested developers to contribute to the project be it with their own effects or with help on the general codebase.

# How can I help / add new effects?
Check out the [CONTRIBUTING](CONTRIBUTING.md) file for more information.

# What does this mod do?
Similar to the old [Vice City Chaos% mod](https://github.com/Lighnat0r-pers/gta-vc-chaos) by [Lightnat0r](https://github.com/Lighnat0r), it activates effects in the corresponding game every X seconds / minutes (cooldown can be defined in the GUI).

# Which GTA games does it support?
This mod Only Support **GTA San-Andreas** for now.

# Which versions of the games does it support?
**GTA San Andreas**: v1.0 US  

# How do I downgrade my game to the required version?
There's a San Andreas downgrader by gamerzworld in the GTAForums here:  
https://gtaforums.com/topic/927016-san-andreas-downgrader/

# What ASI Loader is recommended to play the mod with?
I **highly** recommend using the [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) by [ThirteenAG](https://github.com/ThirteenAG) purely due to the fact that it supports saving CrashDumps.

You can find the latest release here: https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/latest  

Download the `Ultimate-ASI-Loader.zip` file (not the `_x64.zip` one)  
It will contain a file called `dinput8.dll` - move the file to your GTA directory.  
Now rename it to `vorbisFile.dll` and replace the original.  
(Good for people that use the `dinput8.dll` mouse fix. Also the only way I personally got the ASI Loader to work)

Silent's ASI Loader works as well, but if you want to open an issue with some crashes I'll be closing any that do not have crash dumps.

Depending on which ASI Loader you picked, the files differ in size a bit:
- `vorbisFile.dll` (Ultimate ASI Loader): 2,102KB / 2,152,448 bytes
- `vorbisFile.dll` (Silent's ASI Loader): 53KB / 53,760 bytes

The reason why the Ultimate ASI Loader is **WAY BIGGER** in size is because it supports more than just the original `vorbisFile.dll`, comes with an additional windowed mode feature, a ["d3d8 to d3d9" pseudo-driver](https://github.com/crosire/d3d8to9) and a crash handler.

If you don't need any of that, Silent's ASI Loader will be all you need.

# How do I download the mod then?
Head on over to the the `Releases` tab or [click here](https://github.com/AthallahDzaki/Trilogy-ASI-Script/releases/latest).

Installation is as simple as installing an ASI Loader (see previous section) and copying all the files into your game directory.

To Work with My Custom Effect, you need to download my NodeJS Websocket Server [here](https://github.com/AthallahDzaki/Trilogy-Node-Script) and run it.

# ... and how do I use it?
Simple:
1. Start the game
2. Run NodeJS Websocket Server

# I'm getting an "Error: 126" when I try to start the game!
You are most likely missing the DirectX 9 runtime.  
Please download it from here and then try starting the game again:

https://www.microsoft.com/en-us/download/details.aspx?id=8109

If that doesn't fix it then make sure you have the `bass.dll` in your game directory.  
It comes bundled with the Chaos Mod archive you downloaded.

# Is there a list of all effects?
Of course! Here's a [spreadsheet](http://bit.ly/gta-sa-chaos-mod) that has all of them! (Old)

# The effect `SUPER. HOT.` isn't working correctly!
It seems that mouse fixes such as `dinput8.dll` can cause issues with this (and maybe other) effects.
If you're playing modded already I **highly** recommend using SilentPatch instead seeing as it also includes a mouse fix.

# How do I load the last autosave when ingame?
Hold down `Left CTRL` when hovering over the `Load Game` option in the menu.

# Where are the autosaves being saved to?
Inside a sub-directory called `chaos_mod` in your corresponding GTA User Files folder.

# How do I setup the Tiktok Voting?
Refer to the NodeJS Websocket Server [here](https://github.com/AthallahDzaki/Trilogy-Node-Script).

# My game's crashing!
Great! Create an issue with the `.log` and `.dmp` files from the `CrashDumps` folder. (Best to archive them into a `.zip` file or similar)

# I have a suggestion for an effect!
Also great! Feel free to create an issue with that as well, best to explain what it should do in detail.

If you're lucky someone might be able to implement it and contribute it to the mod!

Or better yet: **Create the effect yourself and do a pull request!**

That way you are helping with making the mod even better (or worse, depending on how you look at it).

Check out the [CONTRIBUTING](CONTRIBUTING.md) file for more information.

# Credits
## [DK22Pac](https://github.com/DK22Pac) and everyone involved in the [plugin-sdk](https://github.com/DK22Pac/plugin-sdk)

## [Parik](https://github.com/Parik27)
who has been helping me with a **LOT** of things in both the original mod and this rewrite 💖

## Early v3.0 beta testers
- [English\_Ben](https://twitch.tv/english_ben)
- [hugo_one](https://twitch.tv/hugo_one)
- [Joshimuz](https://twitch.tv/Joshimuz)
- [Riekelt](https://twitch.tv/Riekelt)

## Everyone in the v3.0 release race that was held on the 9th of July, 2022
2D_SVD, Abbygail, BladeXRG, Crucial, DennisOnTheInternet, DoubleDubbel, English\_Ben, Fearful_was_taken, Floh284, GamingMaster,
gian_97, glomerius, Gukol, Husker, Jersukai, Jugachi, Kamikaza404, Kato0909, KinzyKenzie, Kristof, Kuba4ful, LotsOfS,
LuckyLucketts, MoRTaL, mossfis, Mytherious, Nardiko, NateHotshot, NotCreativeBush, prosto_big, Riekelt, Rose, Saymite,
Scemmer_Joke, Simbe, SimonDiamond, Stickheadz32, Tepiloxtl, TonyKaatib, Voskos, WaterArko, YPOC, zoton2
___

<details>
<summary>v1.0 Credits</summary>

## Early v1.0 beta testers
- [English\_Ben](https://twitch.tv/english_ben)
- [GTA_Madman](https://twitch.tv/gta_madman)
- [Heinki](https://twitch.tv/heinki)
- [Joshimuz](https://twitch.tv/joshimuz)
- [Jugachi](https://twitch.tv/jugachi)
- [Naro](https://twitch.tv/naro)
- [Tezur0](https://twitch.tv/tezur0)

## Everyone in the v1.0 release race that was held on the 30th of August, 2019
- [123robot](https://twitch.tv/123robot)
- [DomoTheRussian](https://twitch.tv/domotherussian)
- [English\_Ben](https://twitch.tv/english_ben)
- [Gaël](https://twitch.tv/gaeldemarseille)
- [Jessey](https://twitch.tv/jesseysnipenl)
- [Nate](https://twitch.tv/natehotshot)
- [PizzaFourLife](https://twitch.tv/pizzafourlife)
- [RoK](https://twitch.tv/rok_24)
- [SpuG3](https://twitch.tv/spug3)
- [Terrtail](https://twitch.tv/terrtail)
- [zoton2](https://twitch.tv/zoton2)
- [wariscoach](https://twitch.tv/wariscoach)
</details>