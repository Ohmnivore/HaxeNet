package enet;
import haxe.xml.Parser.S;

/**
 * ...
 * @author Ohmnivore
 */
class NetBase
{
	public var peers:Map<String, Dynamic>;
	
	private var _host:Dynamic;
	
	private var messages:Map <Int, Message>;
	
	public function new() 
	{
		peers = new Map();
		messages = new Map();
	}
	
	public function addMessage(M:Message):Void
	{
		messages.set(M.ID, M);
	}
	
	private function separateMessage(Str:String):Array<Dynamic>
	{
		var _res:Array<Dynamic> = [];
		
		var sep:Int = Str.indexOf(".");
		
		_res.push(Std.parseInt(Str.substr(0, sep - 2)));
		
		_res.push(Str.substr(sep + 1));
		
		return _res;
	}
	
	public function poll(Timeout:Float = 0):ENetEvent
	{
		var e:ENetEvent =  ENet.poll(_host, Timeout);
		
		if (e.type == ENetEvent.E_RECEIVE)
		{
			var res:Array<Dynamic> = separateMessage(e.message);
			
			var m:Message = messages.get(res[0]);
			
			m.unserialize(res[1]);
		}
		
		else if (e.type == ENetEvent.E_CONNECT)
		{
			onPeerConnect(e);
		}
		
		else if (e.type == ENetEvent.E_DISCONNECT)
		{
			onPeerDisonnect(e);
		}
	}
	
	public function onPeerConnect(e:ENetEvent):Void
	{
		
	}
	
	public function onPeerDisonnect(e:ENetEvent):Void
	{
		
	}
	
	public function sendMsg(Address:String, Port:Int,
		MsgID:Int, Channel:Int = 0, Flags:Int = 0):Int
	{
		return ENet.sendMsg(_host, ENet.peerKey(Address, Port), Channel,
			messages.get(MsgID).serialize(), Flags);
	}
	
	public function punchNAT(Address:String, Port:Int, Data:String):Bool
	{
		return ENet.punchNAT(_host, Address, Port, Data);
	}
	
	public function peerDisconnect(Address:String, Port:Int, Force:Bool):Void
	{
		ENet.peerDisconnect(_host, Address, Port, Force);
	}
}