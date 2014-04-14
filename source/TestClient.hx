package ;
import enet.Client;
import enet.ENet;
import enet.ENetEvent;

/**
 * ...
 * @author Ohmnivore
 */
class TestClient extends Client
{
	public var serverID:Int;
	
	public function new(IP:String, Port:Int, Channels:Int = 2, Players:Int = 1) 
	{
		super(IP, Port, Channels, Players);
	}
	
	override public function onPeerConnect(e:ENetEvent):Void 
	{
		super.onPeerConnect(e);
		
		trace("Client->Connected to server.");
		serverID = e.ID;

		Msg.msg2.data.set("test", "Come on baby, light my fire ");
		sendMsg(serverID, Msg.msg2.ID, 1, ENet.ENET_PACKET_FLAG_RELIABLE);
	}
	
	override public function onPeerDisonnect(e:ENetEvent):Void 
	{
		super.onPeerDisonnect(e);
		
		trace("Client->Disconnected from server.");
	}
	
	override public function onReceive(MsgID:Int, E:ENetEvent):Void 
	{
		super.onReceive(MsgID, E);
		
		if (MsgID == Msg.msg.ID)
		{
			trace('Client->Received message $MsgID: ',
				Msg.msg.data.get("test"),
				Msg.msg.data.get("test2"),
				Msg.msg.data.get("test3"),
				Msg.msg.data.get("test4")
				);
			peerDisconnect(serverID, false);
		}
	}
}