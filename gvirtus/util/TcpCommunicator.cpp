/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2010  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */

/**
 * @file   TcpCommunicator.cpp
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Thu Oct 8 12:08:33 2009
 *
 * @brief
 *
 *
 */

#include "TcpCommunicator.h"

# include <assert.h>

#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include <WinSock2.h>
static bool initialized = false;
#endif

#include <cstring>
#include <cstdlib>

using namespace std;

TcpCommunicator::TcpCommunicator(const std::string& communicator) {//传入的参数为"tcp://localhost:9988"
#ifdef _WIN32
	if(!initialized) {
		WSADATA data;
		if(WSAStartup(MAKEWORD(2, 2), &data) != 0)
			throw "Cannot initialized WinSock.";
		initialized = true;
	}
#endif
    const char *valueptr = strstr(communicator.c_str(), "://") + 3;//valueptr指向位置是"localhost:9988"的开头
    const char *portptr = strchr(valueptr, ':');//*portptr的值为":9988"
    if (portptr == NULL)
        throw "Port not specified.";
    /*
     * strtol函数用来将第一个参数（字符串型）转化为数字
     * mPort的值为9988
     */
    mPort = (short) strtol(portptr + 1, NULL, 10);
#ifdef _WIN32
    char *hostname = _strdup(valueptr);
#else
    char *hostname = strdup(valueptr);//strdup是字符串复制函数,*hostname的值为"localhost:9988"
#endif
    hostname[portptr - valueptr] = 0;//hostname的值变为了"localhost"
    mHostname = string(hostname);//mHostname表示服务器的主机IP地址
    struct hostent *ent = gethostbyname(hostname);//这是socket内容
    free(hostname);
    if (ent == NULL)
        throw "TcpCommunicator: Can't resolve hostname '" + mHostname + "'.";
    mInAddrSize = ent->h_length;//主机的IP地址的长度
    /*
     * mInAddr存储主机的IP地址，应该是网络字节序
     */
    mInAddr = new char[mInAddrSize];
    memcpy(mInAddr, *ent->h_addr_list, mInAddrSize);
}

TcpCommunicator::TcpCommunicator(const char *hostname, short port) {
    mHostname = string(hostname);
    struct hostent *ent = gethostbyname(hostname);
    if (ent == NULL)
        throw "TcpCommunicator: Can't resolve hostname '" + mHostname + "'.";
    mInAddrSize = ent->h_length;
    mInAddr = new char[mInAddrSize];
    memcpy(mInAddr, *ent->h_addr_list, mInAddrSize);
    mPort = port;
}

TcpCommunicator::TcpCommunicator(int fd, const char *hostname) {
    mSocketFd = fd;
    InitializeStream();
}

/*
 * Sandy 2016.05.17
 * 用来对zeromq方式进行初始化
 */
TcpCommunicator::TcpCommunicator(void* zmq, int fd,const char *hostname) {
    zeroSocketFd = zmq;
    mSocketFd = fd;
    InitializeStream();
}

TcpCommunicator::~TcpCommunicator() {
    delete[] mInAddr;
}

//2016.04.26 Sandy

void TcpCommunicator::Serve() {
    struct sockaddr_in socket_addr;


    if ((mSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        throw "**TcpCommunicator: Can't create socket.";

    memset((char *) &socket_addr, 0, sizeof(struct sockaddr_in));

    socket_addr.sin_family = AF_INET;//设置通信协议为IPv4
    socket_addr.sin_port = htons(mPort);//设置端口号,htons表示把16位值从主机字节序转换成网络字节序
    socket_addr.sin_addr.s_addr = INADDR_ANY;//把IP地址设置为本机IP地址

    /*
     * 设置socket选项，SO_REUSEADDR （BOOL型？）    允许套接口和一个已在使用中的地址捆绑（参见bind()）
     */
    char on = 1;
    setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

    /*
     * 绑定
     */
    int result = bind(mSocketFd, (struct sockaddr *) & socket_addr,
            sizeof (struct sockaddr_in));
     if( result != 0)
        throw "TcpCommunicator: Can't bind socket.";

     /*
      * 侦听，Listen函数使socket处于被动的监听模式，并为该socket建立一个输入数据队列，5代表最多有5个等待的申请
      */
    if (listen(mSocketFd, 5) != 0)
        throw "AfUnixCommunicator: Can't listen from socket.";
}

//2016.04.26 Sandy
const Communicator * const TcpCommunicator::Accept() const {
    unsigned client_socket_fd;
    struct sockaddr_in client_socket_addr;
#ifndef _WIN32
    unsigned client_socket_addr_size;
#else
	int client_socket_addr_size;
#endif
    client_socket_addr_size = sizeof (struct sockaddr_in);
    if ((client_socket_fd = accept(mSocketFd,
            (sockaddr *) & client_socket_addr,
            &client_socket_addr_size)) == 0)
        throw "TcpCommunicator: Error while accepting connection.";
    else
    {
    	cout<<"received connect request from:"<<inet_ntoa(client_socket_addr.sin_addr)<<endl;
    }

    /*
     * client_socket_fd是用来处理数据传输的socket句柄
     * client_socket_addr是客户程序的地址信息
     * client_socket_addr.sin_addr是客户程序的IP地址
     * inet_ntoa();//将long类型的网络字节序转换成IP地址字符串
     */
    return new TcpCommunicator(client_socket_fd,
            inet_ntoa(client_socket_addr.sin_addr));
}
/*
 * 这个函数是客户端的socket代码，包含了客户端socket的联系建立过程
 * 这个函数只会在前端的TcpCommunicator类会调用
 */

//Sandy 2016.04.26

void TcpCommunicator::Connect() {
    struct sockaddr_in remote;

    if ((mSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        throw "TcpCommunicator: Can't create socket.";

    remote.sin_family = AF_INET;
    remote.sin_port = htons(mPort);
    memcpy(&remote.sin_addr, mInAddr, mInAddrSize);

    if (connect(mSocketFd, (struct sockaddr *) & remote,
            sizeof (struct sockaddr_in)) != 0)
        throw "TcpCommunicator: Can't connect to socket.";
    else
    {
    	cout<<"connected with "<<inet_ntoa(remote.sin_addr)<<endl;
    }
    InitializeStream();		//在这里，因为客户端只会调用到Connect()这一个函数，所以，必须在这里初始化流文件
}

void TcpCommunicator::Close() {
}
/*
 * 从输入流mpInput中读入size大小的数据，放入到buffer中
 */
size_t TcpCommunicator::Read(char* buffer, size_t size) {
    mpInput->read(buffer, size);//read()是流自带函数
    if (mpInput->bad() || mpInput->eof())
        return 0;
    return size;
}

size_t TcpCommunicator::Write(const char* buffer, size_t size) {
    mpOutput->write(buffer, size);
    return size;
}

void TcpCommunicator::Sync() {
    mpOutput->flush();
}

void TcpCommunicator::InitializeStream() {
	cout<<"Function InitializeStream called.."<<endl;		//Sandy
#ifdef _WIN32
        FILE *i = _fdopen(mSocketFd, "r");
	FILE *o = _fdopen(mSocketFd, "w");
	mpInputBuf = new filebuf(i);
	mpOutputBuf = new filebuf(o);
#else
	mpInputBuf = new __gnu_cxx::stdio_filebuf<char>(mSocketFd, ios_base::in);		//???????  __gnu_cxx是一个名字空间
	mpOutputBuf = new __gnu_cxx::stdio_filebuf<char>(mSocketFd, ios_base::out);		//?????
	//mpInputBuf = new __gnu_cxx::stdio_filebuf<char>(  ((FILE *)zeroSocketFd ), ios_base::in);		//zeromq
	//mpOutputBuf = new __gnu_cxx::stdio_filebuf<char>( ((FILE *)zeroSocketFd ), ios_base::out);		//zeromq
#endif
	mpInput = new istream(mpInputBuf);		//????
	mpOutput = new ostream(mpOutputBuf);	//?????
	cout<<"Function InitializeStream Over."<<endl;	//Sandy
}

