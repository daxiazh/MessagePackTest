//
// as3-msgpack (MessagePack for Actionscript3)
// Copyright (C) 2013 Lucas Teixeira (Disturbed Coder)
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
package org.msgpack
{
	import flash.utils.*;

	internal class ArrayWorker extends Worker
	{
		public static function checkType(byte:int):Boolean
		{
			return (byte & 0xf0) == 0x90 || byte == 0xdc || byte == 0xdd;
		}

		private var array:Array;
		private var workers:Array;
		private var count:int;

		public function ArrayWorker(factory:Factory, byte:int = -1)
		{
			super(factory, byte);
			array = [];
			workers = [];
			count = -1;
		}

		override public function assembly(data:*, destination:IDataOutput):void
		{
			var l:uint = data.length;

			if (l < 16)
			{
				// fix array
				destination.writeByte(0x90 | l);
			}
			else if (l < 65536)
			{
				// array 16
				destination.writeByte(0xdc);
				destination.writeShort(l);
			}
			else
			{
				// array 32
				destination.writeByte(0xdd);
				destination.writeUnsignedInt(l);
			}

			// write elements
			for (var i:uint = 0; i < l; i++)
			{
				var worker:Worker = factory.getWorkerByType(data[i]);
				worker.assembly(data[i], destination);
			}
		}

		override public function disassembly(source:IDataInput):*
		{
			if (count == -1)
			{
				if ((byte & 0xf0) == 0x90)
					count = byte & 0x0f
				else if (byte == 0xdc && source.bytesAvailable >= 2)
					count = source.readUnsignedShort();
				else if (byte == 0xdd && source.bytesAvailable >= 4)
					count = source.readUnsignedInt();
			}

			if (array.length < count)
			{
				var first:uint = array.length;

				for (var i:uint = first; i < count; i++)
				{
					if (!workers[i])
					{
						if (source.bytesAvailable == 0)
							break;

						workers.push(factory.getWorkerByByte(source));
					}

					var obj:* = workers[i].disassembly(source);

					if (obj != incomplete)
					{
						array.push(obj);
						continue;
					}

					break;
				}
			}

			if (array.length == count)
				return array;

			return incomplete;
		}
	}
}