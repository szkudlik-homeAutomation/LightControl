#include "../global.h"
#include "Common_code/OutputProcess.h"

class tOutputProcess_lightControl : public tOutputProcess {
public:
	tOutputProcess_lightControl(Scheduler &manager) : tOutputProcess(manager) {}

   virtual void setup()
   {
     #if CONFIG_CENTRAL_NODE
       Output[0].SetPin(28,0);
       Output[1].SetPin(29,0);
       Output[2].SetPin(30,0);
       Output[3].SetPin(31,0);
       Output[4].SetPin(32,0);
       Output[5].SetPin(33,0);
       Output[6].SetPin(34,0);
       Output[7].SetPin(35,0);
     #else
       uint8_t OutputPolarity;
       EEPROM.get(EEPROM_OUTPUT_POLARITY_OFFSET,OutputPolarity);

       Output[0].SetPin(A5,((OutputPolarity &  (1 << 0)) == 0) ? 0 : 1);
       Output[1].SetPin(A4,((OutputPolarity &  (1 << 1)) == 0) ? 0 : 1);
       Output[2].SetPin(A3,((OutputPolarity &  (1 << 2)) == 0) ? 0 : 1);
       Output[3].SetPin(A2,((OutputPolarity &  (1 << 3)) == 0) ? 0 : 1);
       Output[4].SetPin(A1,((OutputPolarity &  (1 << 4)) == 0) ? 0 : 1);
       Output[5].SetPin(A0,((OutputPolarity &  (1 << 5)) == 0) ? 0 : 1);
     #endif
   }
};

extern tOutputProcess_lightControl OutputProcess;
