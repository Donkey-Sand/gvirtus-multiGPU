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
 * @file   TcpCommunicator.h
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Thu Oct 8 12:08:33 2009
 * 
 * @brief  
 * 
 * 
 */

#ifndef _TCPCOMMUNICATOR_H
#define	_TCPCOMMUNICATOR_H

#ifdef _WIN32
#include <fstream>
#else
#include <ext/stdio_filebuf.h>
#endif

#include "Communicator.h"
# include <zmq.h>

/**
 * TcpCommunicator implements a Communicator for the TCP/IP socket.
 */
class TcpCommunicator : public Communicator {
public:
    TcpCommunicator(const std::string & communicator);
    TcpCommunicator(const char *hostname, short port);
    TcpCommunicator(int fd, const char *hostname);
    virtual ~TcpCommunicator();
    void Serve();
    const Communicator * const Accept() const;
    void Connect();
    size_t Read(char *buffer, size_t size);
    size_t Write(const char *buffer, size_t size);
    void Sync();
    void Close();
private:
    void InitializeStream();
    std::istream *mpInput;
    std::ostream *mpOutput;
    /*
     * 这四个成员都是在构造函数中初始化的
     */
    std::string mHostname;//表示主机的地址（IP地址或者是localhost）
    char * mInAddr;//表示主机的IP地址,应该是网络字节序
    int mInAddrSize;//表示主机的IP地址的长度
    short mPort;//表示server端使用的端口号

    int mSocketFd;//表示使用的socket句柄
    void *zeroSocketFd;//只用zeromq的句柄  Sanyd 2016.05.12
#ifdef _WIN32
    std::filebuf *mpInputBuf;
    std::filebuf *mpOutputBuf;
#else
    __gnu_cxx::stdio_filebuf<char> *mpInputBuf;
    __gnu_cxx::stdio_filebuf<char> *mpOutputBuf;
#endif
};

#endif	/* _TCPCOMMUNICATOR_H */

