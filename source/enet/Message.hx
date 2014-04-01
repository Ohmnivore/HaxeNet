package enet;
import haxe.Serializer;
import haxe.Unserializer;

/**
 * ...
 * @author Ohmnivore
 */
class Message
{
	public var ID:Int;
	
	public var data:Map<String, Dynamic>;
	
	public var isServerSideOnly:Bool;
	
	private var fields:Array<String>;
	
	private var _arr:Array<Dynamic>;
	
	public function new(id:Int, Fields:Array<String>, IsServerSideOnly:Bool = false)
	{
		ID = id;
		isServerSideOnly = IsServerSideOnly;
		fields = Fields;
		_arr = new Array();
		data = new Map<String, Dynamic>();
		
		for (f in fields)
		{
			data.set(f, null);
		}
	}
	
	public function serialize():String
	{
		_arr.splice(0, _arr.length);
		
		var res:String = "";
		
		res += Std.string(ID) + ".";
		
		for (f in fields)
		{
			_arr.push(data.get(f));
		}
		
		return res += Serializer.run(_arr);
	}
	
	public function unserialize(S:String):Void
	{
		_arr.splice(0, _arr.length);
		
		_arr = Unserializer.run(S);
		
		var x:Int = 0;
		
		for (f in fields)
		{
			data.set(f, _arr[x]);
		}
	}
}