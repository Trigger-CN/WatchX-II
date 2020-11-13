/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : Tilt.c
* Author             : MEMS Application Team
* Version            : v1.0
* Date               : 15 Mar 2017  
* Description        : EKSTM32 main file
*
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "..\..\Acc\LIS2DS12_Driver__Example\trunk\LIS2DS12_Driver\LIS2DS12_ACC_driver.h"
#include "stm32f1xx_hal.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static u8_t WhoAmI, rst, tiltDetect;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Test Acquisition of sensor samples */
int example_main(void)
{
  /* Check device ID */  
  response = LIS2DS12_ACC_ReadReg(0, LIS2DS12_ACC_WHO_AM_I_REG, &WhoAmI, 1);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  if ( WhoAmI != LIS2DS12_ACC_WHO_AM_I ) 
    while(1); //manage here device not found  
  
  /* Restore default configuration */
  response = LIS2DS12_ACC_W_SOFT_RESET(0, LIS2DS12_ACC_SOFT_RESET_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  do 
  {
    response = LIS2DS12_ACC_R_SOFT_RESET(0, (LIS2DS12_ACC_SOFT_RESET_t*) &rst);
    if(response==MEMS_ERROR) while(1); //manage here comunication error
  }
  while (rst);
  
  /* Tilt Configuration */
  /* Enable Tilt */
  LIS2DS12_ACC_W_TILT_ON(0, LIS2DS12_ACC_TILT_ON_ON);  
  /* Set ACC full scale @ 4g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_4G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  
  /* Set INT notification mode */
  response = LIS2DS12_ACC_W_INT2_TILT(0, LIS2DS12_ACC_INT2_TILT_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  response = LIS2DS12_ACC_W_LIR(0, LIS2DS12_ACC_LIR_OFF);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Powre on axl */
  /* Set ACC ODR  HR_14bit 100Hz (pedometer always work at 25Hz)*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_14bit_100Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
   
  while(1)
  {
    /* Polling on int pin */
    if (HAL_GPIO_ReadPin(INT2_GPIO_Port, INT2_Pin))
    {
      /* Check for a tilt event */  
      LIS2DS12_ACC_R_TILT_INT(0, (LIS2DS12_ACC_TILT_INT_t*) &tiltDetect);
      if ( tiltDetect == LIS2DS12_ACC_TILT_INT_EV_ON)
      {
        /* Tilt detected */
      }
    }
  }
}

