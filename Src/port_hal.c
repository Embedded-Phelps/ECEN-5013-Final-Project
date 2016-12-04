
#include "fsl_port_hal.h"


/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PORT_HAL_SetLowGlobalPinCtrl
 * Description   : Configure low half of pin control register for the same settings,
 *                 this function operates pin 0 -15 of one specific port.
 *
 *END**************************************************************************/
void PORT_HAL_SetLowGlobalPinCtrl(PORT_Type * base, uint16_t lowPinSelect, uint16_t config)
{
    uint32_t combine = lowPinSelect;
    combine = (combine << 16) + config;
    PORT_GPCLR_REG(base) = combine;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PORT_HAL_SetHighGlobalPinCtrl
 * Description   : Configure high half of pin control register for the same
 *                 settings, this function operates pin 16 -31 of one specific port.
 *
 *END**************************************************************************/
void PORT_HAL_SetHighGlobalPinCtrl(PORT_Type * base, uint16_t highPinSelect, uint16_t config)
{
    uint32_t combine = highPinSelect;
    combine = (combine << 16) + config;
    PORT_GPCHR_REG(base) = combine;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
