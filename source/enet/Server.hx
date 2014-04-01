package enet;

/**
 * ...
 * @author Ohmnivore
 */
class Server extends enet.NetBase
{
	public var ip:String;
	public var port:Int;
	
	public function new(IP:String = null, Port:Int = 0, Channels:Int = 2, Players:Int = 32) 
	{
		super();
		
		_host = ENet.server(IP, Port, Channels, Players);
		
		ip = IP;
		port = Port;
	}
}