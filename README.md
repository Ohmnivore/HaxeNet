# Here be Haxe bindings for [ENet](https://github.com/lsalzman/enet)!

HaxeNet was developed for my game [SkullRush](https://github.com/Ohmnivore/SkullRush).

ENet is a really solid reliable/unreliable UDP implementation written in C.

Some familiarity with ENet might help.

With these wrappers, you should be able to release online multiplayer games for Haxe's C++ target.
I wrote this with HaxeFlixel in mind, but you should be able to use just about any other game engine that supports the C++ target.

## PC Instructions:

* To use this in your project, grab the /source/enet directory and put that in your source directory.
* Also, copy the .ndll from my /export/cpp/windows/bin directory to the directory containing your executable. (32-bit ndll)
* All you need to send/receive messages is the collection of static methods found in ENet.hx, and the event class from ENetEvent.hx.
* But if you want a higher-level structure, use Client.hx and Server.hx. Take a look at MenuState.hx, TestClient.hx and TestServer.hx to see how to use these classes.

This is my first adventure in C/C++, so that part of the code might not be stellar.

The .ndll provided has been compiled for Windows. I honestly don't know how to provide support for Linux and Mac, sorry. If someone could help for that it would be great.

## Mac Instructions

Mac support is a bit weak right now, but it's coming together.

* when you compile your executable via "lime test mac", you'll want to navigate to the .app file that is generated and make sure you copy c_source_mac/bin/Debug/EnetTesting.dylib into the .app file.  
* copy c_source_mac/bin/Debug/EnetTesting.dylib and paste it into export/mac64/cpp/bin/ENETWRAP.app/Contents (remember you can right click ENETWRAP.app and select Show Package Contents to browse the .app's contents!). There's probably a way to ensure the dylib is compiled into the .app by lime or haxe, but my knowledge of this process is currently pretty limited.
* if for whatever reason your mac architecture is not x86-64, you'll probably need to recompile EnetTesting.dylib. In the c_source_mac folder, you'll find a Codeblocks project file (EnetTesting.cbp) which you can use to recompile the EnetTesting.dylib.  You'll probably need to recompile the enet library for you current system setup, and replace the compiled enet dylib found at c_source_mac/libraries/libenet.dylib.

## TODO:
* Linux + Android + iOS? ndlls
* Better Mac support - on the c++ side of this, certain features have been turned off in c_source_mac/main.cpp because it was originally dependent on some Windows-only libraries. Someone in the know should make a pass at this file and replace the windows-only functionality with something that is cross-platform. the bulk of the problematic code is in the function enet_get_printable_ip
* would be rad if the client/server output were printed to the screen and not just traced to the command line.  this will make testing on mobile devices a bit easier, and it will also not appear to fail silently if you forget to add the -debug flag when compiling.
