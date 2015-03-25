# MessagePackTest
测试AS3与C++间通过MessagePack打包或拆包的正确性
搭了一个简单的测试C++与AS3间消息发送与接收消息体测试的环境

注意：
只要在C++端是MSGPACK_DEFINE来定义的自定义结构，接收过来的一定是一个数组对象，发送回去也必须是一个对应C++元素顺序的数组对象
