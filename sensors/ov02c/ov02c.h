#ifndef OV02C_H
#define OV02C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ov02c_reg.h"
#include <stddef.h>

typedef int32_t (*OV02C_Init_Func)    (void);
typedef int32_t (*OV02C_DeInit_Func)  (void);
typedef int32_t (*OV02C_GetTick_Func) (void);
typedef int32_t (*OV02C_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV02C_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV02C_Init_Func          Init;
  OV02C_DeInit_Func        DeInit;
  uint16_t                 Address;
  OV02C_WriteReg_Func      WriteReg;
  OV02C_ReadReg_Func       ReadReg;
  OV02C_GetTick_Func       GetTick;
} OV02C_IO_t;

typedef struct
{
  OV02C_IO_t         IO;
  ov02c_ctx_t        Ctx;
  uint8_t             IsInitialized;
} OV02C_Object_t;

typedef struct
{
  uint32_t Config_Resolution;
  uint32_t Config_LightMode;
  uint32_t Config_SpecialEffect;
  uint32_t Config_Brightness;
  uint32_t Config_Saturation;
  uint32_t Config_Contrast;
  uint32_t Config_HueDegree;
  uint32_t Config_Gain;
  uint32_t Config_Exposure;
  uint32_t Config_MirrorFlip;
  uint32_t Config_Zoom;
  uint32_t Config_NightMode;
  uint32_t Config_ExposureMode;
  uint32_t Config_SensorInfo;
  uint32_t Config_TestPattern;
  uint32_t Config_ColorEffect;
} OV02C_Capabilities_t;

#define OV02C_SENSOR_INFO_MAX_LENGTH      (32U)

typedef struct
{
  char name[OV02C_SENSOR_INFO_MAX_LENGTH];
  uint8_t bayer_pattern;
  uint8_t color_depth;
  uint32_t width;
  uint32_t height;
  uint32_t gain_min;
  uint32_t gain_max;
  uint32_t exposure_min;
  uint32_t exposure_max;
} OV02C_SensorInfo_t;

typedef struct
{
  int32_t  (*Init              )(OV02C_Object_t*, uint32_t, uint32_t);
  int32_t  (*DeInit            )(OV02C_Object_t*);
  int32_t  (*ReadID            )(OV02C_Object_t*, uint32_t*);
  int32_t  (*GetCapabilities   )(OV02C_Object_t*, OV02C_Capabilities_t*);
  int32_t  (*SetLightMode      )(OV02C_Object_t*, uint32_t);
  int32_t  (*SetColorEffect    )(OV02C_Object_t*, uint32_t);
  int32_t  (*SetBrightness     )(OV02C_Object_t*, int32_t);
  int32_t  (*SetSaturation     )(OV02C_Object_t*, int32_t);
  int32_t  (*SetContrast       )(OV02C_Object_t*, int32_t);
  int32_t  (*SetHueDegree      )(OV02C_Object_t*, int32_t);
  int32_t  (*MirrorFlipConfig  )(OV02C_Object_t*, uint32_t);
  int32_t  (*ZoomConfig        )(OV02C_Object_t*, uint32_t);
  int32_t  (*SetResolution     )(OV02C_Object_t*, uint32_t);
  int32_t  (*GetResolution     )(OV02C_Object_t*, uint32_t*);
  int32_t  (*SetPixelFormat    )(OV02C_Object_t*, uint32_t);
  int32_t  (*GetPixelFormat    )(OV02C_Object_t*, uint32_t*);
  int32_t  (*NightModeConfig   )(OV02C_Object_t*, uint32_t);
  int32_t  (*SetFrequency      )(OV02C_Object_t*, int32_t);
  int32_t  (*SetGain           )(OV02C_Object_t*, int32_t);
  int32_t  (*SetExposure       )(OV02C_Object_t*, int32_t);
  int32_t  (*SetExposureMode   )(OV02C_Object_t*, int32_t);
  int32_t  (*GetSensorInfo     )(OV02C_Object_t *, OV02C_SensorInfo_t *);
  int32_t  (*SetTestPattern    )(OV02C_Object_t*, int32_t);
} OV02C_CAMERA_Drv_t;


#define OV02C_OK                      (0)
#define OV02C_ERROR                   (-1)

int32_t OV02C_RegisterBusIO(OV02C_Object_t *pObj, OV02C_IO_t *pIO);
int32_t OV02C_Init(OV02C_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t OV02C_DeInit(OV02C_Object_t *pObj);
int32_t OV02C_ReadID(OV02C_Object_t *pObj, uint32_t *Id);
int32_t OV02C_GetCapabilities(OV02C_Object_t *pObj, OV02C_Capabilities_t *Capabilities);
int32_t OV02C_SetGain(OV02C_Object_t *pObj, int32_t gain);
int32_t OV02C_SetExposure(OV02C_Object_t *pObj, int32_t exposure);
int32_t OV02C_SetFrequency(OV02C_Object_t *pObj, int32_t frequency);
int32_t OV02C_SetFramerate(OV02C_Object_t *pObj, int32_t framerate);
int32_t OV02C_MirrorFlipConfig(OV02C_Object_t *pObj, uint32_t Config);
int32_t OV02C_GetSensorInfo(OV02C_Object_t *pObj, OV02C_SensorInfo_t *Info);
int32_t OV02C_SetTestPattern(OV02C_Object_t *pObj, int32_t mode);

// resolutions
#define OV02C_RESOLUTION_1920_1080	1

// formats
#define OV02C_RAW_GRBG10 1

/* CAMERA driver structure */
extern OV02C_CAMERA_Drv_t   OV02C_CAMERA_Driver;

#ifdef __cplusplus
}
#endif

#endif
