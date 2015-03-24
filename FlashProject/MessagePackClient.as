package  {
	
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
    import flash.net.*;
	import flash.events.*;
	import org.msgpack.*;
		
	public class MessagePackClient extends MovieClip {
		private var socket:Socket = new Socket();
		private var msgpack:MsgPack = new MsgPack();
				
		public function MessagePackClient() {
			socket.connect("127.0.0.1", 30006);
			socket.addEventListener( ProgressEvent.SOCKET_DATA, onSocketData );
		}
		
		function onSocketData(e:ProgressEvent ):void
		{
			var byteArray = new ByteArray();
			socket.readBytes(byteArray);
			
			var dataObject:* = msgpack.read(byteArray);
			if (dataObject == incomplete)
			{
				trace("Message is error.");
			}
			else
			{
				// 只要在C++端是MSGPACK_DEFINE来定义的发送数组，接收过来的一定是一个数组对象，发送回去也必须是一个对应C++元素顺序的数组
				
				// var _sendData:Array = [10, 40.0, "Zhanghua"];
				msgpack.write(dataObject, socket);
			}
					   
           socket.flush();
		}		
	}	
}
