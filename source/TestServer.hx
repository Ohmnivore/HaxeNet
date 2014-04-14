package ;
import enet.ENet;
import enet.ENetEvent;
import enet.Server;

/**
 * ...
 * @author Ohmnivore
 */
class TestServer extends Server
{
	public var clientID:Int;
	
	public function new(IP:String = null, Port:Int = 0, Channels:Int = 2, Players:Int = 32) 
	{
		super(IP, Port, Channels, Players);
	}
	
	override public function onPeerConnect(e:ENetEvent):Void 
	{
		super.onPeerConnect(e);
		
		trace("Server->peer connected: ID = ", e.ID);
		clientID = e.ID;
	}
	
	override public function onPeerDisonnect(e:ENetEvent):Void 
	{
		super.onPeerDisonnect(e);
		
		trace("Server->Client disconnected: ID = ", e.ID);
	}
	
	override public function onReceive(MsgID:Int, E:ENetEvent):Void 
	{
		super.onReceive(MsgID, E);
		
		if (MsgID == Msg.msg2.ID)
		{
			var peerID:Int = E.ID;
			var channel:Int = E.channel;
			trace('Server->Received message $MsgID from peer $peerID on channel $channel: ',
				Msg.msg2.data.get("test")
				);

			Msg.msg.data.set("test", "The time to hesitate is through ");
			Msg.msg.data.set("test2", "No time to wallow in the mire ");
			Msg.msg.data.set("test3", "Try now we can only lose ");
			Msg.msg.data.set("test4", "And our love become a funeral pyre ");
			sendMsg(clientID, Msg.msg.ID, 0, ENet.ENET_PACKET_FLAG_RELIABLE);
		}
	}
}