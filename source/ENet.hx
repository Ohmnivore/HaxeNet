package ;

import cpp.Lib;

/**
 * ...
 * @author Ohmnivore
 */
class ENet
{
	static public inline var ENET_PACKET_FLAG_RELIABLE = (1 << 0); //packet must be received by the target peer
	                                                        //and resend attempts should be made until
															//the packet is delivered
	
	static public inline var ENET_PACKET_FLAG_UNSEQUENCED = (1 << 1); //packet will not be sequenced with other 
	                                                            //packets not supported for reliable packets
	
	static public inline var ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT = (1 << 3);  //packet will be fragmented using 
																		//unreliable (instead of reliable)
																		//sends if it exceeds the MTU
	
	static public function init():Int
	{
		return _enet_init();
	}
	
	static public function server(IP:String = null, Port:Int = 0, Channels:Int = 2, Players:Int = 32):Dynamic
	{
		return _enet_create_server(IP, Port, Channels, Players);
	}
	
	static public function client(IP:String = null, Port:Int = 0, Channels:Int = 2, Players:Int = 1):Dynamic
	{
		return _enet_create_client(IP, Port, Channels, Players);
	}
	
	static public function poll(Host:Dynamic, Timeout:Float = 0):ENetEvent
	{
		var e:Dynamic = _enet_poll(Host, Timeout);
		
		return new ENetEvent(e);
	}
	
	static public function event_type(Event:Dynamic):Int
	{
		return _enet_event_type(Event);
	}
	
	static public function event_channel(Event:Dynamic):Int
	{
		return _enet_event_channel(Event);
	}
	
	static public function event_message(Event:Dynamic):String
	{
		return _enet_event_message(Event);
	}
	
	static public function event_peer(Event:Dynamic):String
	{
		return _enet_event_peer(Event);
	}
	
	static public function sendMsg(Host:Dynamic, Address:String, Port:Int,
		Content:String, Channel:Int = 0, Flags:Int = 0):Void
	{
		_enet_event_send(Host, Address + ':' + Std.string(Port), Channel, Content, Flags);
	}
	
	static var _enet_init = Lib.load("EnetTesting", "enet_init", 0);
	static var _enet_create_server = Lib.load("EnetTesting", "enet_create_server", 4);
	static var _enet_create_client = Lib.load("EnetTesting", "enet_create_client", 4);
	static var _enet_poll = Lib.load("EnetTesting", "enet_poll", 2);
	
	static var _enet_event_type = Lib.load("EnetTesting", "enet_event_type", 1);
	static var _enet_event_channel = Lib.load("EnetTesting", "enet_event_channel", 1);
	static var _enet_event_message = Lib.load("EnetTesting", "enet_event_message", 1);
	static var _enet_event_peer = Lib.load("EnetTesting", "enet_event_peer", 1);
	
	static var _enet_event_send = Lib.load("EnetTesting", "enet_send_packet", 5);
}