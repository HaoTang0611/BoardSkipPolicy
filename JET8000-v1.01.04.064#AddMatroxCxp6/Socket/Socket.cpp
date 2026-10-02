/* 
   Socket.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#include "stdafx.h"
#include "Socket.h"
#include <iostream>
#include <ws2tcpip.h>

// link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int Socket::nofSockets_= 0;

void Socket::Start() {
  if (!nofSockets_) {
    WSADATA info;
    if (WSAStartup(MAKEWORD(2,0), &info)) {
      throw "Could not start WSA";
    }
  }
  ++nofSockets_;
}

void Socket::End() {
  WSACleanup();
}

Socket::Socket() : s_(0) {
  Start();
  // UDP: use SOCK_DGRAM instead of SOCK_STREAM
  s_ = socket(AF_INET,SOCK_STREAM,0);

  if (s_ == INVALID_SOCKET) {
    throw "INVALID_SOCKET";
  }

  refCounter_ = new int(1);
}

Socket::Socket(SOCKET s) : s_(s) {
  Start();
  refCounter_ = new int(1);
};

Socket::~Socket() {
  if (! --(*refCounter_)) {
    Close();
    delete refCounter_;
  }

  --nofSockets_;
  if (!nofSockets_) End();
}

Socket::Socket(const Socket& o) {
  refCounter_=o.refCounter_;
  (*refCounter_)++;
  s_         =o.s_;

  nofSockets_++;
}

Socket& Socket::operator=(Socket& o) {
  (*o.refCounter_)++;

  refCounter_=o.refCounter_;
  s_         =o.s_;

  nofSockets_++;

  return *this;
}

void Socket::Close() {
  closesocket(s_);
}

 int Socket::ReceiveBytes(std::string &rBuff)
 {
	rBuff.clear();
	const int kbuferlen = 8192;
	char buf[kbuferlen];
 
	int nStatus = -1;
	while (1)
	{
		u_long arg = 0;
		if (ioctlsocket(s_, FIONREAD, &arg) != 0)
		{
			nStatus = 0;
			break;
		}
	
		if (0 == arg)
		{
			nStatus = 1;
			break;
		}

		if (arg > kbuferlen) arg = kbuferlen;

		int rv = recv (s_, buf, arg, 0);
		if (rv <= 0)
		{
			nStatus = 2;
			break;
		};

		std::string t;

		t.assign (buf, rv);
		rBuff += t;
	}
 
  return nStatus;
}

 std::string Socket::ReceiveBytes()
 {
	 std::string ret("");
	 const int kbuferlen = 8192;
	 char buf[kbuferlen];

	 while (1)
	 {
		 u_long arg = 0;
		 if (ioctlsocket(s_, FIONREAD, &arg) != 0)
			 break;

		 if (0 == arg)
			 break;

		 if (arg > kbuferlen) arg = kbuferlen;

		 int rv = recv(s_, buf, arg, 0);
		 if (rv <= 0) return ret;

		 std::string t;

		 t.assign(buf, rv);
		 ret += t;		 
	 }

	 return ret;
 }

 std::string Socket::ReceiveBytesEx()
 {
	 std::string ret("");
	 while (1)
	 {
		 std::string tmp = ReceiveBytes();

		 if (tmp.empty())
			 break;
		 else
			 ret += tmp;
		 Sleep(1);
	 }
	 return ret;
 }

std::string Socket::ReceiveLine() {
  std::string ret;
  
  while (1) {
    char r;

    switch(recv(s_, &r, 1, 0)) {
      case 0: // not connected anymore;
              // ... but last line sent
              // might not end in \n,
              // so return ret anyway.
        return ret;
      case -1:
        return "";
//      if (errno == EAGAIN) {
//        return ret;
//      } else {
//      // not connected anymore
//      return "";
//      }
    }

    ret += r;
    if (r == '\n')  return ret;
  }  
}

void Socket::SendLine(std::string s) {
  s += '\n';
  send(s_,s.c_str(),s.length(),0);
}

void Socket::SendBytes(const std::string& s) {
  send(s_,s.c_str(),s.length(),0);
}

int Socket::GetAddress(std::string &rAddr)
{
	sockaddr_in SockAddr;
	int addrlen = sizeof(SockAddr);

	if (getsockname(s_, (LPSOCKADDR)&SockAddr, &addrlen) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	char myIP[16];
	inet_ntop(AF_INET, &SockAddr.sin_addr, myIP, sizeof(myIP));

	rAddr = myIP;

	return 0;
}

SocketServer::SocketServer(int port, int connections, TypeSocket type)
{  
  //s_ = socket(AF_INET, SOCK_STREAM, 0);
  s_ = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (s_ == INVALID_SOCKET)
  {
    throw "INVALID_SOCKET";
  }  

  sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  /* bind the socket to the internet address */  
  if (::bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
  {
    closesocket(s_);
    throw "INVALID_SOCKET";
  }
  
  listen(s_, connections);

  if (type == NonBlockingSocket)
  {
	  u_long arg = 1;
	  ioctlsocket(s_, FIONBIO, &arg);
  }
}

Socket* SocketServer::Accept()
{
  SOCKET new_sock = accept(s_, 0, 0);

  if (new_sock == INVALID_SOCKET)
  {
    int rc = WSAGetLastError();
    if(rc==WSAEWOULDBLOCK) {
      return 0; // non-blocking call, no request pending
    }
    else {
      throw std::exception("Invalid Socket");
    }
  }

  Socket* r = new Socket(new_sock);
  return r;
}

SocketClient::SocketClient(const std::string& host, int port) : Socket() {
  std::string error;

  /*
  hostent *he;
  if ((he = gethostbyname(host.c_str())) == 0) {
    error = strerror(errno);
    throw error;
  }
  
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = *((in_addr *)he->h_addr);
  memset(&(addr.sin_zero), 0, 8);
  */
  //gethostbyname by getaddrinfo replacement
  ADDRINFO hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_flags = AI_ALL;
  hints.ai_family = PF_INET;
  hints.ai_protocol = IPPROTO_IPV4;
  ADDRINFO* pResult = NULL;

  int errcode = getaddrinfo((LPCSTR)host.c_str(), NULL, &hints, &pResult);

  if (errcode != 0)
  {
	  const size_t errmsglen = 256;
	  char errmsg[errmsglen];
	  error = strerror_s(errmsg, errmsglen, errno);	  
	  throw error;
  }

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = *((ULONG*)&(((sockaddr_in*)pResult->ai_addr)->sin_addr));
  addr.sin_port = htons(port);

  //
  if (::connect(s_, (sockaddr *) &addr, sizeof(sockaddr))) {
    //error = strerror(WSAGetLastError());
	const size_t errmsglen = 256;
	char errmsg[errmsglen];
	error = strerror_s(errmsg, errmsglen, WSAGetLastError());
    throw error;
  }
}

SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type) {
  FD_ZERO(&fds_);
  
  FD_ZERO(&fds_w);

  FD_SET(const_cast<Socket*>(s1)->s_,&fds_);

  if(s2) {
    FD_SET(const_cast<Socket*>(s2)->s_,&fds_);
  }     

  TIMEVAL tval;
  tval.tv_sec  = 0;
  tval.tv_usec = 1;

  TIMEVAL *ptval;
  if(type==NonBlockingSocket) {
    ptval = &tval;
  }
  else { 
    ptval = 0;
  }

  //if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR) 
  //	throw "Error in select";
  if (select(0, &fds_, &fds_w, (fd_set*)0, ptval) == SOCKET_ERROR)
	  throw "Error in select";
}

bool SocketSelect::Readable(Socket const* const s) {
  if (FD_ISSET(s->s_, &fds_)) return true;
     return false;
}
