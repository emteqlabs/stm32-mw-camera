#ifndef OV05C10_H
#define OV05C10_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ov05c10_reg.h"

typedef int32_t (*OV05C10_Init_Func)(void);
typedef int32_t (*OV05C10_DeInit_Func)(void);
typedef int32_t (*OV05C10_GetTick_Func)(void);
typedef int32_t (*OV05C10_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*OV05C10_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  OV05C10_Init_Func Init;
  OV05C10_DeInit_Func DeInit;
  uint16_t Address;
  OV05C10_WriteReg_Func WriteReg;
  OV05C10_ReadReg_Func ReadReg;
  OV05C10_GetTick_Func GetTick;
} OV05C10_IO_t;

typedef enum
{
  OV05C10_MIRROR_FLIP_NONE = 0,
  OV05C10_FLIP,
  OV05C10_MIRROR,
  OV05C10_MIRROR_FLIP
} OV05C10_MirrorFlip_t;

typedef struct
{
  OV05C10_IO_t IO;
  ov05c10_ctx_t Ctx;
  uint8_t IsInitialized;
  uint32_t Vts;
  int32_t AppliedGainMdB;
  int32_t AppliedExposureUs;
} OV05C10_Object_t;

#define OV05C10_OK                           0
#define OV05C10_ERROR                       -1

#define OV05C10_WIDTH                        2888U
#define OV05C10_HEIGHT                       1808U
#define OV05C10_FPS                          60U
#define OV05C10_RAW10                        10U
#define OV05C10_BAYER_PATTERN                0x01U /* ISP_DEMOS_TYPE_GRBG */

#define OV05C10_EXPOSURE_MIN_LINES           6U
#define OV05C10_EXPOSURE_MARGIN_LINES        33U
#define OV05C10_DEFAULT_VTS                  1860U

#define OV05C10_ANALOG_GAIN_MIN_REG          0x10U
#define OV05C10_ANALOG_GAIN_MAX_REG          0xF8U
#define OV05C10_DIGITAL_GAIN_MIN_REG         0x040U
#define OV05C10_DIGITAL_GAIN_MAX_REG         0x100U
#define OV05C10_GAIN_MIN_MDB                 0
#define OV05C10_ANALOG_GAIN_MAX_MDB          23807
#define OV05C10_GAIN_MAX_MDB                 35848

int32_t OV05C10_RegisterBusIO(OV05C10_Object_t *obj, OV05C10_IO_t *io);
int32_t OV05C10_Init(OV05C10_Object_t *obj, uint32_t resolution, uint32_t pixel_format);
int32_t OV05C10_DeInit(OV05C10_Object_t *obj);
int32_t OV05C10_Start(OV05C10_Object_t *obj);
int32_t OV05C10_Stop(OV05C10_Object_t *obj);
int32_t OV05C10_ReadID(OV05C10_Object_t *obj, uint32_t *id);
int32_t OV05C10_SetGain(OV05C10_Object_t *obj, int32_t gain_mdB);
int32_t OV05C10_SetExposure(OV05C10_Object_t *obj, int32_t exposure_us);
int32_t OV05C10_GetAppliedGain(OV05C10_Object_t *obj, int32_t *gain_mdB);
int32_t OV05C10_GetAppliedExposure(OV05C10_Object_t *obj, int32_t *exposure_us);
int32_t OV05C10_MirrorFlipConfig(OV05C10_Object_t *obj, OV05C10_MirrorFlip_t config);
int32_t OV05C10_SetTestPattern(OV05C10_Object_t *obj, int32_t mode);

#ifdef __cplusplus
}
#endif

#endif /* OV05C10_H */
