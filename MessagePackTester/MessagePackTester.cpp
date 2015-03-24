// MessagePackTester.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>��	///������Ҫ��ͷ�ļ��������������
#include <winsock2.h>	///socket�׽���ͷ�ļ�
using namespace std;	///ʹstd�����ռ�ȫ��չ������ÿ����дstd::ǰ׺��

#include "include/msgpack.hpp"

WSADATA wsaData;		//WSADATA ��һ���ṹ��˵����Ҫװ�ص�dll�İ汾�����Ϣ
SOCKET listeningSocket;	//�������ӵ�socket
SOCKET newConnection;	//�Ѿ����ӵ�socket
SOCKADDR_IN serverAddr;	//�������������Ϣ
SOCKADDR_IN clientAddr;	//���Ӷ˵������Ϣ
char message[256];		//������ȡ�ͻ��ķ���������Ϣ

void SendAndReciveData();

int _tmain(int argc, _TCHAR* argv[])
{
	//װ��dll�ļ�
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//����һ���µ�Ƕ�����������ͻ������ӣ�ʹ��TCPЭ��
	listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//����SOCKADDR_IN�ṹ��Ϊbind����׼��
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(30006);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listeningSocket, (SOCKADDR*)(&(serverAddr)), sizeof(serverAddr));
	listen(listeningSocket, 5);  //��ʼ�����������Դ������������Ϊ�� 
	int ClientAddrLen = sizeof(struct sockaddr_in);   //struct sockaddr_in�ṹ�Ĵ�С

	printf("Wait for connecting ...\n");

	//****�ȴ����ӣ�һֱ������������� 
	//�������ӵ�socket
	//�����������󶨵�socket
	//������������������ʱ�����Ľṹ
	//���������ڣ��������Ľṹ��С�������������ǵ�ַ
	newConnection = accept(listeningSocket, (SOCKADDR*)(&clientAddr), &ClientAddrLen);

	printf("Test send and recive data ...\n");

	SendAndReciveData();

	printf("Test complete.\n");

	//�ر�socket
	closesocket(newConnection);
	closesocket(listeningSocket);
	WSACleanup();
	system("PAUSE"); ///����Ļ���ְ��������������ʾû�д˾��������ֱ���˳�

	return EXIT_SUCCESS; ///����
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

// ������	���ڲ�����Ϣ���ݵĽṹ
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



// ���ܣ�	��������Է��������AS3����Ϣ��
void SendAndReciveData()
{
	TestSendAndReciveData<CTestSendAndReciveData>();		// Ҫ�����µ����ͣ�ֻ��Ҫ�޸�������ͼ���
}