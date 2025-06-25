#ifndef OV2740_H
#define OV2740_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ov2740_reg.h"
#include <stddef.h>

typedef int32_t (*OV2740_Init_Func)    (void);
typedef int32_t (*OV2740_DeInit_Func)  (void);
typedef int32_t (*OV2740_GetTick_Func) (void);
typedef int32_t (*OV2740_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV2740_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV2740_Init_Func          Init;
  OV2740_DeInit_Func        DeInit;
  uint16_t                  Address;
  OV2740_WriteReg_Func      WriteReg;
  OV2740_ReadReg_Func       ReadReg;
  OV2740_GetTick_Func       GetTick;
} OV2740_IO_t;

typedef struct
{
  OV2740_IO_t         IO;
  ov2740_ctx_t        Ctx;
  uint8_t            IsInitialized;
  uint64_t 			 Pclk;  // Hz
} OV2740_Object_t;

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
} OV2740_Capabilities_t;

#define OV2740_SENSOR_INFO_MAX_LENGTH      (32U)

typedef struct
{
  char name[OV2740_SENSOR_INFO_MAX_LENGTH];
  uint8_t bayer_pattern;
  uint8_t color_depth;
  uint32_t width;
  uint32_t height;
  uint32_t gain_min;
  uint32_t gain_max;
  uint32_t exposure_min;
  uint32_t exposure_max;
} OV2740_SensorInfo_t;

// flip/mirror
typedef enum {
  OV2740_MIRROR_FLIP_NONE,
  OV2740_FLIP,
  OV2740_MIRROR,
  OV2740_MIRROR_FLIP
} OV2740_MirrorFlip_t;

typedef struct
{
  int32_t  (*Init              )(OV2740_Object_t*, uint32_t, uint32_t);
  int32_t  (*DeInit            )(OV2740_Object_t*);
  int32_t  (*ReadID            )(OV2740_Object_t*, uint32_t*);
  int32_t  (*GetCapabilities   )(OV2740_Object_t*, OV2740_Capabilities_t*);
  int32_t  (*SetLightMode      )(OV2740_Object_t*, uint32_t);
  int32_t  (*SetColorEffect    )(OV2740_Object_t*, uint32_t);
  int32_t  (*SetBrightness     )(OV2740_Object_t*, int32_t);
  int32_t  (*SetSaturation     )(OV2740_Object_t*, int32_t);
  int32_t  (*SetContrast       )(OV2740_Object_t*, int32_t);
  int32_t  (*SetHueDegree      )(OV2740_Object_t*, int32_t);
  int32_t  (*MirrorFlipConfig  )(OV2740_Object_t*, OV2740_MirrorFlip_t);
  int32_t  (*ZoomConfig        )(OV2740_Object_t*, uint32_t);
  int32_t  (*SetResolution     )(OV2740_Object_t*, uint32_t);
  int32_t  (*GetResolution     )(OV2740_Object_t*, uint32_t*);
  int32_t  (*SetPixelFormat    )(OV2740_Object_t*, uint32_t);
  int32_t  (*GetPixelFormat    )(OV2740_Object_t*, uint32_t*);
  int32_t  (*NightModeConfig   )(OV2740_Object_t*, uint32_t);
  int32_t  (*SetFrequency      )(OV2740_Object_t*, int32_t);
  int32_t  (*SetGain           )(OV2740_Object_t*, int32_t);
  int32_t  (*SetExposure       )(OV2740_Object_t*, int32_t);
  int32_t  (*SetExposureMode   )(OV2740_Object_t*, int32_t);
  int32_t  (*GetSensorInfo     )(OV2740_Object_t *, OV2740_SensorInfo_t *);
  int32_t  (*SetTestPattern    )(OV2740_Object_t*, int32_t);
} OV2740_CAMERA_Drv_t;


#define OV2740_OK                      (0)
#define OV2740_ERROR                   (-1)

// resolutions
#define OV2740_RESOLUTION_1932_1092	1

// formats
#define OV2740_RAW_GRBG10 1

/* CAMERA driver structure */
extern OV2740_CAMERA_Drv_t   OV2740_CAMERA_Driver;

int32_t OV2740_RegisterBusIO(OV2740_Object_t *pObj, OV2740_IO_t *pIO);
int32_t OV2740_Init(OV2740_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t OV2740_DeInit(OV2740_Object_t *pObj);
int32_t OV2740_Start(OV2740_Object_t *pObj);
int32_t OV2740_ReadID(OV2740_Object_t *pObj, uint32_t *Id);
int32_t OV2740_GetCapabilities(OV2740_Object_t *pObj, OV2740_Capabilities_t *Capabilities);
int32_t OV2740_SetGain(OV2740_Object_t *pObj, int32_t gain);
int32_t OV2740_SetExposure(OV2740_Object_t *pObj, int32_t exposure);
int32_t OV2740_SetFrequency(OV2740_Object_t *pObj, int32_t frequency);
int32_t OV2740_SetFramerate(OV2740_Object_t *pObj, int32_t framerate);
int32_t OV2740_MirrorFlipConfig(OV2740_Object_t *pObj, OV2740_MirrorFlip_t Config);
int32_t OV2740_GetSensorInfo(OV2740_Object_t *pObj, OV2740_SensorInfo_t *Info);

#ifdef __cplusplus
}
#endif

#endif
