package enet;

/**
 * ...
 * @author Ohmnivore
 */
class Client extends enet.NetBase
{
	public function new(IP:String = null, Port:Int = 0, Channels:Int = 2, Players:Int = 1) 
	{
		super();
		
		_host = ENet.client(IP, Port, Channels, Players);
	}
}