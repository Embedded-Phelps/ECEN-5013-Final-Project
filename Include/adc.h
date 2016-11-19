#ifndef __ADC_H
#define __ADC_H

#include "includes.h"

#define ADCR_VDD                (65535U)    // Maximum value when use 16b resolution
#define V_BG                    (1000U)     // BANDGAP voltage in mV (trim to 1.0V)
#define V_TEMP25                (716U)      // Typical converted value at 25 oC in mV
#define M                       (1620U)     // Typical slope:uV/oC
#define STANDARD_TEMP           (25)

#ifdef __cplusplus
extern "C" {
#endif

void app_ADCInit(void);
void temp_CalibrateParam(void);
int32_t read_OnChipTemperature(void);

#ifdef __cplusplus
}
#endif


#endif
