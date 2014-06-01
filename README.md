# Here be Haxe bindings for [ENet](https://github.com/lsalzman/enet)!

ENet is a really solid reliable/unreliable UDP implementation written in C.

Some familiarity with ENet might help.

With these wrappers, you should be able to release online multiplayer games for Haxe's C++ target.
I wrote this with HaxeFlixel in mind, but you should be able to use just about any other game engine that supports the C++ target.

## Instructions:

* To use this in your project, grab the /source/enet directory and put that in your source directory.
* Also, copy the .ndll from my /export/cpp/windows/bin directory to the directory containing your executable. (32-bit ndll)
* All you need to send/receive messages is the collection of static methods found in ENet.hx, and the event class from ENetEvent.hx.
* But if you want a higher-level structure, use Client.hx and Server.hx. Take a look at MenuState.hx, TestClient.hx and TestServer.hx to see how to use these classes.

This is my first adventure in C/C++, so that part of the code might not be stellar.

The .ndll provided has been compiled for Windows. I honestly don't know how to provide support for Linux and Mac, sorry. If someone could help for that it would be great.

## TODO:
* Linux + Mac + Android + iOS? ndlls
