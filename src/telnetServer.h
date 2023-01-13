#pragma once

#include "../global.h"
#include "Common_code/Network/tcpServer.h"
#include "Common_code/Logger.h"
#include <Commander.h>

class tTelnetSession : public tTcpSession, public tLogTransport
{
public:
  tTelnetSession(EthernetClient aEthernetClient);
  virtual ~tTelnetSession();

protected:
  virtual bool doProcess();

  virtual void Log(uint8_t str);
private:
  static const uint16_t TELNET_SESSION_TIMEOUT = 65535; // 10 minutes
};



class tTelnetServer : public tTcpServer
{
public:
  tTelnetServer() : tTcpServer(23) {}

protected:
  virtual tTcpSession* NewSession(EthernetClient aEthernetClient) { return new tTelnetSession(aEthernetClient); }
};

