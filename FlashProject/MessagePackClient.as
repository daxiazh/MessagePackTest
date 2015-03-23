package  {
	
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
    import flash.net.Socket;
	
	public class MessagePackClient extends MovieClip {
		private var socket:Socket = new Socket();
		private var byteArray:ByteArray = new ByteArray();
		
		
		public function MessagePackClient() {
			socket.connect("127.0.0.1", 30006);
			socket.addEventListener( ProgressEvent.SOCKET_DATA, onSocketData );
		}
		
		function onSocketData(e:ProgressEvent ):void
		{
           socket.readBytes(byteArray);
           trace("c++过来的参数:" + byteArray);
		   
		   
           socket.writeUTFBytes("good bye");
		   
           socket.flush();
		}
	}	
}
