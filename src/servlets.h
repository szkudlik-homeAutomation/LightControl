#pragma once

#include "../global.h"
#if CONFIG_CENTRAL_NODE

#include "Common_code/Network/httpServer.h"
#include "http_binaries.h"
#include "tLightControlOutgoingFrames.h"
#include "Common_code/tMessageReciever.h"


class tjavaScriptServlet :  public tHttpServlet
{
public:
  tjavaScriptServlet() : tHttpServlet() {}
  virtual ~tjavaScriptServlet() {}

  virtual bool ProcessAndResponse();
};


/**
 * output servlet - controlling or checking state of an output
 */
class tOutputSetServlet :  public tHttpServlet
{
public:
  tOutputSetServlet() : tHttpServlet() {}
  virtual ~tOutputSetServlet() {}

  virtual bool ProcessAndResponse();
};

/**
 * force button servlet - send a button press
 */
class tForceButtonPressServlet :  public tHttpServlet
{
public:
  tForceButtonPressServlet() : tHttpServlet() {}
  virtual ~tForceButtonPressServlet() {}

  virtual bool ProcessAndResponse();
};

/**
 * output servlet - controlling or checking state of an output
 */
class tSetTimerServlet :  public tHttpServlet
{
public:
  tSetTimerServlet() : tHttpServlet() {}
  virtual ~tSetTimerServlet() {}

  virtual bool ProcessAndResponse();
};

class tOutputStateServlet : public tHttpServlet, public tMessageReciever
{
  public:
  tOutputStateServlet() :  tHttpServlet(), mRequestSent(false)
  {
	  RegisterMessageType(MessageType_SerialFrameRecieved);
  }

  virtual ~tOutputStateServlet() {}
  virtual void onMessage(uint8_t type, uint16_t data, void *pData);
  virtual bool ProcessAndResponse();

protected:
  uint8_t mExpectedDevID;
  uint8_t mExpectedOutputID;
  unsigned long mStartTimestamp;
  uint8_t   mPowerState;
  uint16_t  mTimerValue;
  uint16_t  mDefaultTimer;

  static uint8_t const STATUS_WAIT = 0;
  static uint8_t const STATUS_TIMEOUT = 1;
  static uint8_t const STATUS_COMPLETE = 2;
  static unsigned long const REQUEST_TIMEOUT = 5000;

  void SendOutputStateRequest(uint8_t DevID, uint8_t OutputID);
  uint8_t CheckStateRequest();
  bool ProcessOutputState(const char * caption);

  bool mRequestSent;
};

#endif // CENTRAL_NODE
