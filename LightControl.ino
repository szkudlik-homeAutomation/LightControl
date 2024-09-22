

#include "global.h"
#include "src/Common_code/tApplication.h"
#include "src/LightControl/tLightControlOutputProcess.h"

#if CONFIG_LIGHT_CONTROL_APP
tLightControlOutputProcess LightControlOutputProcess;
#endif CONFIG_LIGHT_CONTROL_APP

class tHomeAutomation: public tApplication {
public:
	tHomeAutomation() : tApplication() {}

protected:

	virtual void AppSetupBefore() {
		// set default values for eeprom
		SetDefaultEEPromValues();
	}
};

tHomeAutomation HomeAutomation;
