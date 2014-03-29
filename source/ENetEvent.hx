class ENetEvent
{
	public var type:Int;
	public var channel:Int; //-1 if null
	public var message:String = null;
	public var address:String = null;
	public var port:Int;
	
	static public inline var E_NONE = 0;
	static public inline var E_CONNECT = 1;
	static public inline var E_DISCONNECT = 2;
	static public inline var E_RECEIVE = 3;
	
	public function new(EventFromC:Dynamic):Void
	{
		type = ENet.event_type(EventFromC);
		
		channel = ENet.event_channel(EventFromC);
		
		if (type > E_NONE)
		{
			//Setting address and port
			var _addbuff:String = ENet.event_peer(EventFromC);
			var _addbuff2:Array<String> = _addbuff.split(":");
			address = _addbuff2[0];
			port = Std.parseInt(_addbuff2[1]);
			
			if (type == E_RECEIVE)
			{
				message = ENet.event_message(EventFromC);
			}
		}
	}
}