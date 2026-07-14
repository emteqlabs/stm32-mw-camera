#ifndef CMW_OV05C10
#define CMW_OV05C10

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "cmw_camera.h"
#include "cmw_sensors_if.h"
#include "cmw_errno.h"
#include "ov05c10.h"
#include "isp_api.h"

typedef struct
{
  uint16_t Address;
  uint32_t ClockInHz;
  OV05C10_Object_t ctx_driver;
  ISP_HandleTypeDef hIsp;
  ISP_AppliHelpersTypeDef appliHelpers;
  DCMIPP_HandleTypeDef *hdcmipp;
  uint8_t IsInitialized;
  int32_t (*Init)(void);
  int32_t (*DeInit)(void);
  int32_t (*WriteReg)(uint16_t, uint16_t, uint8_t*, uint16_t);
  int32_t (*ReadReg) (uint16_t, uint16_t, uint8_t*, uint16_t);
  int32_t (*GetTick) (void);
  void (*Delay)(uint32_t delay_in_ms);
  void (*ShutdownPin)(int value);
  void (*EnablePin)(int value);
} CMW_OV05C10_t;

int CMW_OV05C10_Probe(CMW_OV05C10_t *io_ctx, CMW_Sensor_if_t *ov05c10_if);
void CMW_OV05C10_SetDefaultSensorValues(CMW_OV05C10_config_t *ov05c10_config);

#ifdef __cplusplus
}
#endif

#endif
