Here be Haxe bindings for ENet!

ENet is a really solid reliable/unreliable UDP implementation written in C:
https://github.com/lsalzman/enet

Some familiarity with ENet might help:
http://enet.bespin.org/

With these wrappers, you should be able to release games for Haxe's C++ target.
I wrote this with HaxeFlixel in mind, but you should be able to use just about any other game engine that supports the C++ target.

To use this in your project, grab ENet.hx and ENetEvent.hx and put that in your source directory.
Also, copy the .ndll from my export/cpp/windows/bin directory to the directory containing your application.

This is my first adventure in C/C++, so that part of the code might not be stellar.

The .ndll provided has been compiled for Windows. I honestly don't know how to provide support for Linux and Mac, sorry. If someone could help for that it would be great.

TODO:
make sure everything is garbage collected?
Linux + Mac + Android? ndlls