#ifndef OV05C10_H
#define OV05C10_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ov05c10_reg.h"
#include <stddef.h>

typedef int32_t (*OV05C10_Init_Func)    (void);
typedef int32_t (*OV05C10_DeInit_Func)  (void);
typedef int32_t (*OV05C10_GetTick_Func) (void);
typedef int32_t (*OV05C10_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV05C10_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV05C10_Init_Func          Init;
  OV05C10_DeInit_Func        DeInit;
  uint16_t                 Address;
  OV05C10_WriteReg_Func      WriteReg;
  OV05C10_ReadReg_Func       ReadReg;
  OV05C10_GetTick_Func       GetTick;
} OV05C10_IO_t;

typedef struct
{
  OV05C10_IO_t         IO;
  ov05c10_ctx_t        Ctx;
  uint8_t            IsInitialized;
  uint64_t 			 Pclk;  // Hz
} OV05C10_Object_t;

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
} OV05C10_Capabilities_t;

#define OV05C10_SENSOR_INFO_MAX_LENGTH      (32U)

typedef struct
{
  char name[OV05C10_SENSOR_INFO_MAX_LENGTH];
  uint8_t bayer_pattern;
  uint8_t color_depth;
  uint32_t width;
  uint32_t height;
  uint32_t gain_min;
  uint32_t gain_max;
  uint32_t exposure_min;
  uint32_t exposure_max;
} OV05C10_SensorInfo_t;

// flip/mirror
typedef enum {
  OV05C10_MIRROR_FLIP_NONE,
  OV05C10_FLIP,
  OV05C10_MIRROR,
  OV05C10_MIRROR_FLIP
} OV05C10_MirrorFlip_t;

typedef struct
{
  int32_t  (*Init              )(OV05C10_Object_t*, uint32_t, uint32_t);
  int32_t  (*DeInit            )(OV05C10_Object_t*);
  int32_t  (*ReadID            )(OV05C10_Object_t*, uint32_t*);
  int32_t  (*GetCapabilities   )(OV05C10_Object_t*, OV05C10_Capabilities_t*);
  int32_t  (*SetLightMode      )(OV05C10_Object_t*, uint32_t);
  int32_t  (*SetColorEffect    )(OV05C10_Object_t*, uint32_t);
  int32_t  (*SetBrightness     )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetSaturation     )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetContrast       )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetHueDegree      )(OV05C10_Object_t*, int32_t);
  int32_t  (*MirrorFlipConfig  )(OV05C10_Object_t*, OV05C10_MirrorFlip_t);
  int32_t  (*ZoomConfig        )(OV05C10_Object_t*, uint32_t);
  int32_t  (*SetResolution     )(OV05C10_Object_t*, uint32_t);
  int32_t  (*GetResolution     )(OV05C10_Object_t*, uint32_t*);
  int32_t  (*SetPixelFormat    )(OV05C10_Object_t*, uint32_t);
  int32_t  (*GetPixelFormat    )(OV05C10_Object_t*, uint32_t*);
  int32_t  (*NightModeConfig   )(OV05C10_Object_t*, uint32_t);
  int32_t  (*SetFrequency      )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetGain           )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetExposure       )(OV05C10_Object_t*, int32_t);
  int32_t  (*SetExposureMode   )(OV05C10_Object_t*, int32_t);
  int32_t  (*GetSensorInfo     )(OV05C10_Object_t *, OV05C10_SensorInfo_t *);
  int32_t  (*SetTestPattern    )(OV05C10_Object_t*, int32_t);
} OV05C10_CAMERA_Drv_t;


#define OV05C10_OK                      (0)
#define OV05C10_ERROR                   (-1)

// resolutions
#define OV05C10_RESOLUTION_1928_1082	1

// formats
#define OV05C10_RAW_GRBG10 1

/* CAMERA driver structure */
extern OV05C10_CAMERA_Drv_t   OV05C10_CAMERA_Driver;

int32_t OV05C10_RegisterBusIO(OV05C10_Object_t *pObj, OV05C10_IO_t *pIO);
int32_t OV05C10_Init(OV05C10_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t OV05C10_DeInit(OV05C10_Object_t *pObj);
int32_t OV05C10_Start(OV05C10_Object_t *pObj);
int32_t OV05C10_ReadID(OV05C10_Object_t *pObj, uint32_t *Id);
int32_t OV05C10_GetCapabilities(OV05C10_Object_t *pObj, OV05C10_Capabilities_t *Capabilities);
int32_t OV05C10_SetGain(OV05C10_Object_t *pObj, int32_t gain);
int32_t OV05C10_SetExposure(OV05C10_Object_t *pObj, int32_t exposure);
int32_t OV05C10_SetFrequency(OV05C10_Object_t *pObj, int32_t frequency);
int32_t OV05C10_SetFramerate(OV05C10_Object_t *pObj, int32_t framerate);
int32_t OV05C10_MirrorFlipConfig(OV05C10_Object_t *pObj, OV05C10_MirrorFlip_t Config);
int32_t OV05C10_GetSensorInfo(OV05C10_Object_t *pObj, OV05C10_SensorInfo_t *Info);

#ifdef __cplusplus
}
#endif

#endif
