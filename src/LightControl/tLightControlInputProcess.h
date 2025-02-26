/*
 * tLightControlInputProcess.h
 *
 *  Created on: 24 lut 2025
 *      Author: szkud
 */

#pragma once

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP
#if CONFIG_INPUT_PROCESS
#include "../Common_code/tInputProcess.h"

class tLightControlInputProcess final : public tInputProcess
{
public:
	tLightControlInputProcess() : tInputProcess() {};

    virtual void setup()
    {

        uint16_t InputPolarity;
#if CONFIG_LIGHT_CONTROL_EEPROM_OUTPUTS_POLARITY
        EEPROM.get(EEPROM_INPUT_POLARITY_OFFSET,InputPolarity);
#else
        InputPolarity = 0;	// all active low
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 0
        mButton[0].SetPin(CONFIG_INPUT_PROCESS_PIN0,  ((InputPolarity &  (1 << 0)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 1
        mButton[1].SetPin(CONFIG_INPUT_PROCESS_PIN1,  ((InputPolarity &  (1 << 1)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 2
        mButton[2].SetPin(CONFIG_INPUT_PROCESS_PIN2,  ((InputPolarity &  (1 << 2)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 3
        mButton[3].SetPin(CONFIG_INPUT_PROCESS_PIN3,  ((InputPolarity &  (1 << 3)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 4
        mButton[4].SetPin(CONFIG_INPUT_PROCESS_PIN4,  ((InputPolarity &  (1 << 4)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 5
        mButton[5].SetPin(CONFIG_INPUT_PROCESS_PIN5,  ((InputPolarity &  (1 << 5)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 6
        mButton[6].SetPin(CONFIG_INPUT_PROCESS_PIN6,  ((InputPolarity &  (1 << 6)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 7
        mButton[7].SetPin(CONFIG_INPUT_PROCESS_PIN7,  ((InputPolarity &  (1 << 7)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 8
        mButton[8].SetPin(CONFIG_INPUT_PROCESS_PIN8,  ((InputPolarity & (1 << 8)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 9
        mButton[9].SetPin(CONFIG_INPUT_PROCESS_PIN9,  ((InputPolarity & (1 << 9)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 10
        mButton[10].SetPin(CONFIG_INPUT_PROCESS_PIN10,((InputPolarity & (1<<10)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 11
        mButton[11].SetPin(CONFIG_INPUT_PROCESS_PIN11,((InputPolarity & (1<<11)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 12
        mButton[11].SetPin(CONFIG_INPUT_PROCESS_PIN12,((InputPolarity & (1<<12)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 13
        mButton[11].SetPin(CONFIG_INPUT_PROCESS_PIN13,((InputPolarity & (1<<13)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 14
        mButton[11].SetPin(CONFIG_INPUT_PROCESS_PIN14,((InputPolarity & (1<<14)) == 0) ? LOW : HIGH);
#endif
#if CONFIG_INPUT_PROCESS_NUM_OF_PINS > 15
        mButton[11].SetPin(CONFIG_INPUT_PROCESS_PIN15,((InputPolarity & (1<<15)) == 0) ? LOW : HIGH);
#endif
    }
};

#endif //CONFIG_INPUT_PROCESS
#endif // CONFIG_LIGHT_CONTROL_APP
