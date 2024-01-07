/*
 * keyCodeTelnetCommands.cpp
 *
 *  Created on: Jan 5, 2024
 *      Author: mszkudli
 */

#include "../global.h"
#if CONFIG_TELNET_SERVER

#include "lib/Commander/src/Commander.h"
#include "Common_code/Network/telnetServer.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

bool send_ClearCodes(Commander &Cmdr);
bool send_addCode(Commander &Cmdr);
bool send_triggerCode(Commander &Cmdr);


bool send_ClearCodes(Commander &Cmdr)
{
  int Dst;
  if(Cmdr.getInt(Dst))
  {
      CommSenderProcess::Instance->Enqueue(Dst,MESSAGE_TYPE_CLEAR_CODES,0,NULL);
  }
  else
  {
    Cmdr.println(F("Usage: ClearCodes dst_dev_id"));
    return false;
  }

  return true;
}

bool send_addCode(Commander &Cmdr)
{
    int Dst;
    int type;
    uint32_t code;
    uint8_t codeDigits;
    int ButtonBitmap;
    int validStart = 0;
    int validEnd = 0;
    if(! Cmdr.getInt(Dst))
        goto error;
    if(! Cmdr.getInt(type))
        goto error;
    if(! Cmdr.getLong(code, &codeDigits))
        goto error;
    if(! Cmdr.getInt(ButtonBitmap))
        goto error;

//    if(! Cmdr.getInt(validStart))
//        goto finish;
//    if(! Cmdr.getInt(validEnd))
//        goto error;
//
  finish:
    tMessageTypeAddCode Msg;
    Msg.code = code;
    if (type)
    	Msg.size = codeDigits;
    else
    	Msg.size = 0;
    Msg.ValidEnd = validEnd;
    Msg.ValidStart = validStart;
    Msg.ButtonBitmap = ButtonBitmap;

    CommSenderProcess::Instance->Enqueue(Dst,MESSAGE_TYPE_ADD_CODE,sizeof(Msg),&Msg);

    return true;

  error:
        Cmdr.println(F("Usage: AddCode dev_id type code ButtonBitmap"));
        Cmdr.println(F("   type 0 - a dongle, 1 - keySequence"));
        Cmdr.println(F("   code a code in binary format"));
        Cmdr.println(F("   ButtonBitmap bitmap of simulated key Press when code is accepted"));
//        Cmdr.println(F("   [validStart validEnd] timestamps from-to the code is valid"));
        return false;
}

bool send_triggerCode(Commander &Cmdr)
{
	  int Dst;
	  int type;
	  uint32_t code;
	  uint8_t codeDigits;

	  if(! Cmdr.getInt(Dst))
		  goto error;
	  if(! Cmdr.getInt(type))
		  goto error;
	  if(! Cmdr.getLong(code, &codeDigits))
		  goto error;

	  tMessageTypeTriggerCode Msg;
	    if (type)
	    	Msg.size = codeDigits;
	    else
	    	Msg.size = 0;
	  Msg.code = code;
	  CommSenderProcess::Instance->Enqueue(Dst,MESSAGE_TYPE_TRIGGER_CODE,sizeof(Msg),&Msg);
	  return true;

	error:
	    Cmdr.println(F("Usage: TriggerCode dev_id type code"));
	    Cmdr.println(F("   type 0 - a dongle, 1 - keySequence"));
	    Cmdr.println(F("   code a code in binary format"));
	    return false;
}


#endif //CONFIG_TELNET_SERVER
