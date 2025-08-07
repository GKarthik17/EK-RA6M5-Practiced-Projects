#include "r_iic_master.h"
#include <stdbool.h>
#include "hal_data.h"

volatile bool rx_done = false;
void TMP512_Initialize(i2c_master_ctrl_t * const p_api_ctrl, i2c_master_cfg_t const * const p_cfg);
void TMP512_Read(i2c_master_ctrl_t *const p_api_ctrl, uint8_t reg, uint8_t *const  p_dest, uint32_t const  bytes);

void IIC_Receive(i2c_master_callback_args_t *p_args)
{
    if(p_args->event ==  I2C_MASTER_EVENT_RX_COMPLETE){
        rx_done = true;
        p_args->event = I2C_MASTER_EVENT_NONE;
    }
}

void TMP512_Initialize(i2c_master_ctrl_t * const p_api_ctrl, i2c_master_cfg_t const * const p_cfg)
{
    fsp_err_t err = R_IIC_MASTER_Open(p_api_ctrl, p_cfg);
    if(err == FSP_SUCCESS){
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_07, BSP_IO_LEVEL_HIGH);
    }
}

static volatile fsp_err_t err2;

static volatile fsp_err_t err1;

void TMP512_Read(i2c_master_ctrl_t *const p_api_ctrl, uint8_t reg, uint8_t *const  p_dest, uint32_t const  bytes)
{
//    err1 = R_IIC_MASTER_Write(p_api_ctrl, &reg, 1, true);
    rx_done = false;
    err2 = R_IIC_MASTER_Read(p_api_ctrl, p_dest, bytes, false);
    if(err2 == FSP_SUCCESS){
          R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_06, BSP_IO_LEVEL_HIGH);
     }
    while(!rx_done);
}
