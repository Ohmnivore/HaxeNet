package;

import enet.Message;
import flixel.FlxG;
import flixel.FlxSprite;
import flixel.FlxState;
import flixel.text.FlxText;
import flixel.ui.FlxButton;
import flixel.util.FlxMath;
import enet.ENet;
import enet.ENetEvent;
import enet.Server;
import enet.Client;
import enet.NetBase;
import cpp.vm.Thread;

/**
 * A FlxState which can be used for the game's menu.
 */
class MenuState extends FlxState
{
	public var server:TestServer = null;
	public var client:TestClient = null;
	
	/**
	 * Function that is called up when to state is created to set it up. 
	 */
	override public function create():Void
	{
		// Set a background color
		FlxG.cameras.bgColor = 0xff131c1b;
		// Show the mouse (in case it hasn't been disabled)
		#if !FLX_NO_MOUSE
		FlxG.mouse.visible = true;
		#end
		
		super.create();
		FlxG.autoPause = false;
		
		var t:FlxText = new FlxText(0, 0, FlxG.width, "", 12);
		t.text = "Open the flixel debugger, and open the log. Then, press S to initiate a server. Open another instance of this program, open the debugger and the log, then press C to open a client. You should see lots of pretty stuff being printed to both logs.";
		add(t);

		ENet.init();
		Msg.initMsg();
	}
	
	/**
	 * Function that is called when this state is destroyed - you might want to 
	 * consider setting all objects this state uses to null to help garbage collection.
	 */
	override public function destroy():Void
	{
		super.destroy();
	}

	/**
	 * Function that is called once every frame.
	 */
	override public function update():Void
	{
		super.update();
		
		if (FlxG.keys.pressed.S && server == null)
		{
			server = new TestServer("localhost", 1234, 2, 32);
			Msg.addToHost(server);
			Thread.create(updateServer);
			trace("Launched server.");
		}
		
		if (FlxG.keys.pressed.C && client == null)
		{
			client = new TestClient("localhost", 1234, 2, 1);
			Msg.addToHost(client);
			Thread.create(updateClient);
			trace("Launched client.");
		}
	}

	//Threading stuff
	public function updateClient():Void
	{
		while (true)
		{
			updateHost(client);
		}
	}

	public function updateServer():Void
	{
		while (true)
		{
			updateHost(server);
		}
	}

	public function updateHost(Host:enet.NetBase):Void
	{
		Host.poll();
		Sys.sleep(0.001);
	}
}