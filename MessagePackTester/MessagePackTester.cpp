// MessagePackTester.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>　	///加载需要的头文件　关于输入输出
#include <winsock2.h>	///socket套接字头文件
using namespace std;	///使std命名空间全部展开不用每次在写std::前缀了

#include "include/msgpack.hpp"

WSADATA wsaData;		//WSADATA 是一个结构　说明需要装载的dll的版本相关信息
SOCKET listeningSocket;	//监听连接的socket
SOCKET newConnection;	//已经连接的socket
SOCKADDR_IN serverAddr;	//服务器的相关信息
SOCKADDR_IN clientAddr;	//连接端的相关信息
char message[256];		//用来读取客户的发送来的消息

void SendAndReciveData();

int _tmain(int argc, _TCHAR* argv[])
{
	//装载dll文件
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建一个新的嵌套字来监听客户端连接，使用TCP协议
	listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//建立SOCKADDR_IN结构，为bind做好准备
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(30006);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listeningSocket, (SOCKADDR*)(&(serverAddr)), sizeof(serverAddr));
	listen(listeningSocket, 5);  //开始监听，最多可以处理的连接请求为５ 
	int ClientAddrLen = sizeof(struct sockaddr_in);   //struct sockaddr_in结构的大小

	printf("Wait for connecting ...\n");

	//****等待连接，一直阻塞到获得连接 
	//返回连接的socket
	//参数１　被绑定的socket
	//参数２　当发生请求时被填充的结构
	//参数３　第２个参数的结构大小　这里必须给的是地址
	newConnection = accept(listeningSocket, (SOCKADDR*)(&clientAddr), &ClientAddrLen);

	printf("Test send and recive data ...\n");

	SendAndReciveData();

	printf("Test complete.\n");

	//关闭socket
	closesocket(newConnection);
	closesocket(listeningSocket);
	WSACleanup();
	system("PAUSE"); ///让屏幕出现按任意键结束的提示没有此句程序运行直接退出

	return EXIT_SUCCESS; ///返回
}

template<typename T>
void TestSendAndReciveData()
{
	T _SendData;
	
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, _SendData);

	send(newConnection, sbuf.data(), sbuf.size(), 0);

	char szRecvData[512];
	int nRecvSize = recv(newConnection, szRecvData, 512, 0);

	msgpack::unpacked msg;
	
	msgpack::unpack(&msg, szRecvData, nRecvSize);

	msgpack::object obj = msg.get();

	// you can convert object to myclass directly
	T _RevData;
	obj.convert(&_RevData);
}

// 描述：	用于测试消息传递的结构
class CTestSendAndReciveData
{
	int		m_nPara1;
	float	m_fPara1;
	std::string m_strName;
public:
	CTestSendAndReciveData()
	{
		m_nPara1 = 5;
		m_fPara1 = 10.2f;
		m_strName = "XiongMin";
	}

	bool operator==(CTestSendAndReciveData& other)
	{
		return m_nPara1 == other.m_nPara1 &&
			m_fPara1 == other.m_fPara1;
	}

	MSGPACK_DEFINE(m_nPara1, m_fPara1, m_strName);
};



// 功能：	在这里测试发送与接收AS3的消息包
void SendAndReciveData()
{
	TestSendAndReciveData<CTestSendAndReciveData>();		// 要测试新的类型，只需要修改类的类型即可
}