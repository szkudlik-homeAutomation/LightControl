#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP
#if CONFIG_OUTPUT_PROCESS
#include "../Common_code/tOutputProcess.h"

class tLightControlOutputProcess : public tOutputProcess {
public:
	tLightControlOutputProcess() : tOutputProcess() {}

   virtual void setup()
   {
     uint8_t OutputPolarity;
     #if CONFIG_LIGHT_CONTROL_EEPROM_OUTPUTS_POLARITY
       EEPROM.get(EEPROM_OUTPUT_POLARITY_OFFSET,OutputPolarity);
     #else
       OutputPolarity = 0;
	 #endif

#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 0
       Output[0].SetPin(CONFIG_OUTPUT_PROCESS_PIN0,((OutputPolarity &  (1 << 0)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 1
       Output[1].SetPin(CONFIG_OUTPUT_PROCESS_PIN1,((OutputPolarity &  (1 << 1)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 2
       Output[2].SetPin(CONFIG_OUTPUT_PROCESS_PIN2,((OutputPolarity &  (1 << 2)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 3
       Output[3].SetPin(CONFIG_OUTPUT_PROCESS_PIN3,((OutputPolarity &  (1 << 3)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 4
       Output[4].SetPin(CONFIG_OUTPUT_PROCESS_PIN4,((OutputPolarity &  (1 << 4)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 5
       Output[5].SetPin(CONFIG_OUTPUT_PROCESS_PIN5,((OutputPolarity &  (1 << 5)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 6
       Output[6].SetPin(CONFIG_OUTPUT_PROCESS_PIN6,((OutputPolarity &  (1 << 6)) == 0) ? 0 : 1);
#endif
#if CONFIG_OUTPUT_PROCESS_NUM_OF_PINS > 7
       Output[7].SetPin(CONFIG_OUTPUT_PROCESS_PIN7,((OutputPolarity &  (1 << 7)) == 0) ? 0 : 1);
#endif
   }
protected:
    virtual void onMessage(uint8_t type, uint16_t data, void *pData);

};
#endif CONFIG_OUTPUT_PROCESS
#endif CONFIG_LIGHT_CONTROL_APP
