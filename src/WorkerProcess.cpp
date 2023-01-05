#include "../global.h"
#include "WorkerProcess.h"
#include "OutputProcess.h"
#include "ResponseHandler.h"
#include "CommDefs.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

bool NodeScanTask::Process(uint32_t * pPeriod)
{
   *pPeriod = REQUEST_SENDING_PERIOD;

   if (mCurrentNodeID < MAX_NUM_OF_NODES)
   {
       // send a frame
       mCurrentNodeID++;
       Worker.SendMsgVersionRequest(mCurrentNodeID);  // staring from 1
   }
   else if (mCurrentNodeID == MAX_NUM_OF_NODES)
   {
      *pPeriod = RESPONSE_WAIT_PERIOD;
       mCurrentNodeID++;
   }
   else
   {
       // send result
       ResponseHandler::NodeScanResponse(mActiveNodesMap);
       return false;
   }

   return true;
}

void NodeScanTask::vVersionResponseHandler(uint8_t DevID, uint8_t Major, uint8_t Minor, uint8_t Patch)
{
   mActiveNodesMap |= 1 << (DevID-1);
}




// OVERVIEW STATE HANDSHAKE
bool WorkerProcess::SendMsgOverviewStateRequest(uint8_t RecieverID)
{
#ifdef CONTROLLER
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OVERVIEW_STATE_REQUEST");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OVERVIEW_STATE_REQUEST, 0, NULL);
#endif
  return true;
}

bool WorkerProcess::SendMsgOverviewStateResponse(uint8_t RecieverID, uint8_t  PowerState, uint8_t  TimerState)
{
  tMessageTypeOverviewStateResponse Msg;
  Msg.PowerState = PowerState;
  Msg.TimerState = TimerState;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE, sizeof(Msg), &Msg);
  return true;
}

// GET OUTPUT STATE HANDSHAKE
bool WorkerProcess::SendMsgOutputStateRequest(uint8_t RecieverID, uint8_t  OutputID)
{
#ifdef CONTROLLER
  tMessageTypeOutputStateRequest Msg;
  Msg.OutputID = OutputID;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OUTPUT_STATE_REQUEST");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OUTPUT_STATE_REQUEST, sizeof(Msg), &Msg);
#endif
  return true;
};

bool WorkerProcess::SendMsgOutputStateResponse(uint8_t RecieverID, uint8_t  OutputID, uint8_t  PowerState, uint16_t TimerValue, uint16_t DefaultTimer)
{
  tMessageTypeOutputStateResponse Msg;
  Msg.OutputID = OutputID;
  Msg.PowerState = PowerState;
  Msg.TimerValue = TimerValue;
  Msg.DefaultTimer = DefaultTimer;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OUTPUT_STATE_RESPONSE");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OUTPUT_STATE_RESPONSE, sizeof(Msg), &Msg);

  return true;
};

// SET OUTPUT
bool WorkerProcess::SendMsgSetOutput(uint8_t RecieverID, uint8_t  OutId, uint8_t  State, uint16_t Timer)
{
#ifdef CONTROLLER
  tMessageTypeSetOutput Message;
  Message.OutId = OutId;
  Message.State = State;
  Message.Timer = Timer;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_OUTPUT");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_SET_OUTPUT, sizeof(Message), &Message);
#endif
  return true;
}

// BUTTON  PRESS
bool WorkerProcess::SendMsgButtonPress(uint8_t RecieverID, uint8_t ForceSrcId, uint16_t ShortClick, uint16_t LongClick, uint16_t DoubleClick)
{
  tMessageTypeButtonPress Msg;
  Msg.ShortClick = ShortClick;
  Msg.LongClick  = LongClick;
  Msg.DoubleClick = DoubleClick;
  Msg.ForceSrcId = ForceSrcId;

  DEBUG_PRINTLN_3("===================>sending MESSAGE_BUTTON_PRESS");
  CommSender.Enqueue(RecieverID,MESSAGE_BUTTON_PRESS,sizeof(Msg),&Msg);
  return true;
};

bool WorkerProcess::SendMsgClearAllActions(uint8_t RecieverID)
{
#ifdef CONTROLLER
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_CLEAR_ACTIONS");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_CLEAR_ACTIONS,0,NULL);
#endif
  return true;
};

bool WorkerProcess::SendMsgAddAction(uint8_t RecieverID, uint8_t OutId, uint8_t SenderDevID, uint8_t ButtonId, uint8_t TriggerType, uint8_t ActionType, uint16_t Timer, uint8_t  OutputsMask, uint8_t  OutputsStates)
{
#ifdef CONTROLLER
  tMessageTypeSetAction Message;
  Message.OutId = OutId;
  Message.SenderDevID = SenderDevID;
  Message.ButtonId = ButtonId;
  Message.TriggerType = TriggerType;
  Message.ActionType = ActionType;
  Message.Timer = Timer;
  Message.OutputsMask = OutputsMask;
  Message.OutputsStates = OutputsStates;

  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_ACTION");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_SET_ACTION,sizeof(Message),&Message);
#endif
  return true;
};

// EEPROM CRC Handshake
bool WorkerProcess::SendMsgEepromCrcRequest(uint8_t RecieverID)
{
#ifdef CONTROLLER
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_REQUEST");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_REQUEST,0,NULL);
#endif
  return true;
}

bool WorkerProcess::SendMsgEepromCrcResponse(uint8_t RecieverID,  uint8_t NumOfActions, uint16_t EepromCRC)
{
  tMessageTypeEepromCRCResponse Msg;
  Msg.NumOfActions = NumOfActions;
  Msg.EepromCRC = EepromCRC;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_RESPONSE");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_RESPONSE,sizeof(Msg),&Msg);
  return true;
};


// VERSION HANDSHAKE
bool WorkerProcess::SendMsgVersionRequest(uint8_t RecieverID)
{
#ifdef CONTROLLER
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_FW_VERSION_REQUEST");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_FW_VERSION_REQUEST,0,NULL);
#endif
  return true;
}

bool WorkerProcess::SendMsgVersionResponse(uint8_t RecieverID, uint8_t Major, uint8_t Minor, uint8_t Patch)
{
  tMessageTypeFwVesionResponse Msg;
  Msg.Major = Major;
  Msg.Minor = Minor;
  Msg.Patch = Patch;

  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_FW_VERSION_RESPONSE,sizeof(Msg),&Msg);
};

bool WorkerProcess::SendMsgReset(uint8_t RecieverID)
{
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_FORCE_RESET");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_FORCE_RESET,0,NULL);
}


bool WorkerProcess::SendMsgSetDefaultTimer(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeSetDefaultTimer Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_DEFAULT_TIMER");
  Msg.OutputID = OutputID;
  Msg.DefaultTimerValue = DefTimerValue;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_SET_DEFAULT_TIMER,sizeof(Msg),&Msg);
}

bool WorkerProcess::SendMsgDefaultTimerRequest(uint8_t RecieverID, uint8_t OutputID)
{
  tMessageTypeDefaultTimerRequest Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_REQUEST");
  Msg.OutputID = OutputID;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_REQUEST,sizeof(Msg),&Msg);
}

bool WorkerProcess::SendMsgDefaultTimerResponse(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeDefaultTimerResponse Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE");
  Msg.OutputID = OutputID;
  Msg.DefTimerValue = DefTimerValue;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE,sizeof(Msg),&Msg);
}

bool WorkerProcess::triggerNodesScan()
{
#ifdef CONTROLLER
   Enqueue(new NodeScanTask());
#endif
  return true;
}

void WorkerProcess::service()
{
  #ifdef CONTROLLER
   if (NULL == pCurrentWorkerTask)
   {
      if (mQueue.isEmpty())
      {
         // go to sleep
         disable();
         return;
      }

      pCurrentWorkerTask = mQueue.dequeue();
   }

   uint32_t nextPeriod;
   bool continueTask;
   continueTask = pCurrentWorkerTask->Process(&nextPeriod);
   if (continueTask)
   {
      // continue after requested time
      setPeriod(nextPeriod);
      return;
   }

   // task finished
   delete (pCurrentWorkerTask);
   pCurrentWorkerTask = NULL;
   setPeriod(SERVICE_CONSTANTLY);   // next iteration will go for next queue item or disable the task if the queue is empty
#else
   disable();
#endif
}

void WorkerProcess::Enqueue(WorkerTask *pWorkerTask)
{
   mQueue.enqueue(pWorkerTask);

   if (! isEnabled())
   {
     setPeriod(SERVICE_CONSTANTLY);
     setIterations(RUNTIME_FOREVER);
     enable();
   }
}
