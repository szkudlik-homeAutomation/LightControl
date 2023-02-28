#pragma once

#include "../global.h"

class ResponseHandler
{
public:
  ResponseHandler() { pNext = pFirst ; pFirst = this; }

  virtual ~ResponseHandler();


  static void OverviewStateResponseHandler(uint8_t SenderID, uint8_t PowerState, uint8_t  TimerState);
  static void OutputStateResponseHandler(uint8_t SenderID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer);
  static void EepromCRCResponseHandler(uint8_t SenderID, uint8_t NumOfActions, uint16_t EepromCRC);
  static void NodeScanResponse(uint32_t ActiveNodesMap);
  static void DefaultTimerResponseHandler(uint8_t SenderID,uint8_t OutputID,uint16_t DefTimerValue);

protected:


  virtual void vOverviewStateResponseHandler(uint8_t DevID, uint8_t PowerState, uint8_t  TimerState) {}
  virtual void vOutputStateResponseHandler(uint8_t DevID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer) {}
  virtual void vEepromCRCResponseHandler(uint8_t DevID, uint8_t NumOfActions, uint16_t EepromCRC) {}
  virtual void vDefaultTimerResponseHandler(uint8_t DevID,uint8_t OutputID,uint16_t DefTimerValue) {}

  virtual void vNodeScanResponse(uint32_t ActiveNodesMap) {}

private:
  static ResponseHandler* pFirst;
  ResponseHandler* pNext;
};
