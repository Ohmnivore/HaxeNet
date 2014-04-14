package ;
import enet.Message;
import enet.NetBase;

/**
 * ...
 * @author Ohmnivore
 */
class Msg
{
	public static var msg:enet.Message;
	public static var msg2:enet.Message;
	
	public static function initMsg() :Void
	{
		msg = new Message( 2, ["test", "test2", "test3", "test4"]);
		msg2 = new Message( 192, ["test"]);
	}
	
	public static function addToHost(Host:NetBase):Void
	{
		Host.addMessage(msg);
		Host.addMessage(msg2);
	}
}