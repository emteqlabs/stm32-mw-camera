#include "ov05c10.h"

#include <math.h>
#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define OV05C10_UPDATE_CONTROLS             0x01U
#define OV05C10_UPDATE_MODE                 0x02U

typedef struct
{
  uint8_t address;
  uint8_t value;
  uint8_t delay_ms;
} OV05C10_RegValue_t;

/*
 * OV05C10R1A_AE02.ovd, linear V1.13 profile:
 * 24 MHz ECLK, 2888x1808, two-lane MIPI RAW10, 60 fps.
 * Panther commands with device IDs 0x102 and 0x110 configure the evaluation
 * platform rather than the image sensor and are intentionally not included.
 */
static const OV05C10_RegValue_t ov05c10_2888x1808_60fps[] =
{
  {0xfd, 0x00, 0},
  {0x20, 0x00, 0},
  {0xfd, 0x00, 0},
  {0x20, 0x0b, 0},
  {0xc1, 0x09, 0},
  {0x21, 0x06, 0},
  {0x14, 0x78, 0},
  {0xe7, 0x03, 0},
  {0xe7, 0x00, 0},
  {0x21, 0x00, 1},

  {0xfd, 0x01, 0},
  {0x03, 0x00, 0},
  {0x04, 0x06, 0},
  {0x07, 0x08, 0},
  {0x1b, 0x01, 0},
  {0x24, 0xff, 0},
  {0x42, 0x5d, 0},
  {0x43, 0x08, 0},
  {0x44, 0x81, 0},
  {0x46, 0x5f, 0},
  {0x48, 0x18, 0},
  {0x49, 0x04, 0},
  {0x5c, 0x18, 0},
  {0x5e, 0x13, 0},
  {0x70, 0x15, 0},
  {0x77, 0x35, 0},
  {0x79, 0x00, 0},
  {0x7b, 0x08, 0},
  {0x7d, 0x08, 0},
  {0x7e, 0x08, 0},
  {0x7f, 0x08, 0},
  {0x90, 0x37, 0},
  {0x91, 0x05, 0},
  {0x92, 0x18, 0},
  {0x93, 0x27, 0},
  {0x94, 0x05, 0},
  {0x95, 0x38, 0},
  {0x9b, 0x00, 0},
  {0x9c, 0x06, 0},
  {0x9d, 0x28, 0},
  {0x9e, 0x06, 0},
  {0xb2, 0x0f, 0},
  {0xb3, 0x29, 0},
  {0xbf, 0x3c, 0},
  {0xc2, 0x04, 0},
  {0xc4, 0x00, 0},
  {0xca, 0x20, 0},
  {0xcb, 0x20, 0},
  {0xcc, 0x28, 0},
  {0xcd, 0x28, 0},
  {0xce, 0x20, 0},
  {0xcf, 0x20, 0},
  {0xd0, 0x2a, 0},
  {0xd1, 0x2a, 0},

  {0xfd, 0x0f, 0},
  {0x00, 0x00, 0},
  {0x01, 0xa0, 0},
  {0x02, 0x48, 0},
  {0x07, 0x8e, 0},
  {0x08, 0x70, 0},
  {0x09, 0x01, 0},
  {0x0b, 0x40, 0},
  {0x0d, 0x07, 0},
  {0x11, 0x33, 0},
  {0x12, 0x77, 0},
  {0x13, 0x66, 0},
  {0x14, 0x65, 0},
  {0x15, 0x37, 0},
  {0x16, 0xbf, 0},
  {0x17, 0xff, 0},
  {0x18, 0xff, 0},
  {0x19, 0x12, 0},
  {0x1a, 0x10, 0},
  {0x1c, 0x77, 0},
  {0x1d, 0x77, 0},
  {0x20, 0x0f, 0},
  {0x21, 0x0f, 0},
  {0x22, 0x0f, 0},
  {0x23, 0x0f, 0},
  {0x2b, 0x20, 0},
  {0x2c, 0x20, 0},
  {0x2d, 0x04, 0},

  {0xfd, 0x03, 0},
  {0x9d, 0x0f, 0},
  {0x9f, 0x40, 0},
  {0xfd, 0x00, 0},
  {0x20, 0x1b, 0},

  {0xfd, 0x04, 0},
  {0x19, 0x60, 0},
  {0xfd, 0x02, 0},
  {0x75, 0x05, 0},
  {0x7f, 0x06, 0},
  {0x9a, 0x03, 0},
  {0xa2, 0x07, 0},
  {0xa3, 0x10, 0},
  {0xa5, 0x02, 0},
  {0xa6, 0x0b, 0},
  {0xa7, 0x48, 0},

  {0xfd, 0x07, 0},
  {0x42, 0x00, 0},
  {0x43, 0x80, 0},
  {0x44, 0x00, 0},
  {0x45, 0x80, 0},
  {0x46, 0x00, 0},
  {0x47, 0x80, 0},
  {0x48, 0x00, 0},
  {0x49, 0x80, 0},
  {0x00, 0xf7, 0},
  {0xfd, 0x00, 0},
  {0xe7, 0x03, 0},
  {0xe7, 0x00, 0},

  {0xfd, 0x00, 0},
  {0x93, 0x18, 0},
  {0x94, 0xff, 0},
  {0x95, 0xbd, 0},
  {0x96, 0x1a, 0},
  {0x98, 0x04, 0},
  {0x99, 0x08, 0},
  {0x9b, 0x10, 0},
  {0x9c, 0x3f, 0},
  {0xa1, 0x05, 0},
  {0xa4, 0x2f, 0},
  {0xc0, 0x0c, 0},
  {0xc1, 0x08, 0},
  {0xc2, 0x00, 0},
  {0xb6, 0x20, 0},
  {0xbb, 0x80, 0},
  {0xfd, 0x00, 0},
  {0xa0, 0x01, 0},
};

static int32_t OV05C10_ReadRegWrap(void *handle, uint8_t reg, uint8_t *data, uint16_t length)
{
  OV05C10_Object_t *obj = (OV05C10_Object_t *)handle;
  return obj->IO.ReadReg(obj->IO.Address, reg, data, length);
}

static int32_t OV05C10_WriteRegWrap(void *handle, uint8_t reg, uint8_t *data, uint16_t length)
{
  OV05C10_Object_t *obj = (OV05C10_Object_t *)handle;
  return obj->IO.WriteReg(obj->IO.Address, reg, data, length);
}

static int32_t OV05C10_Delay(OV05C10_Object_t *obj, uint32_t delay_ms)
{
  uint32_t start;

  if ((obj->IO.GetTick == NULL) || (delay_ms == 0U))
  {
    return (delay_ms == 0U) ? OV05C10_OK : OV05C10_ERROR;
  }

  start = (uint32_t)obj->IO.GetTick();
  while (((uint32_t)obj->IO.GetTick() - start) < delay_ms)
  {
  }
  return OV05C10_OK;
}

static int32_t OV05C10_SelectPage(OV05C10_Object_t *obj, uint8_t page)
{
  return ov05c10_write_reg(&obj->Ctx, OV05C10_REG_PAGE_SELECT, &page, 1U);
}

static int32_t OV05C10_WritePageReg(OV05C10_Object_t *obj, uint8_t page, uint8_t reg,
                                    uint8_t *data, uint16_t length)
{
  if (OV05C10_SelectPage(obj, page) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return ov05c10_write_reg(&obj->Ctx, reg, data, length);
}

static int32_t OV05C10_ReadPageReg(OV05C10_Object_t *obj, uint8_t page, uint8_t reg,
                                   uint8_t *data, uint16_t length)
{
  if (OV05C10_SelectPage(obj, page) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return ov05c10_read_reg(&obj->Ctx, reg, data, length);
}

static int32_t OV05C10_WriteTable(OV05C10_Object_t *obj, const OV05C10_RegValue_t *table,
                                  uint32_t count)
{
  uint32_t i;

  for (i = 0U; i < count; i++)
  {
    uint8_t value = table[i].value;
    if (ov05c10_write_reg(&obj->Ctx, table[i].address, &value, 1U) != OV05C10_OK)
    {
      return OV05C10_ERROR;
    }
    if ((table[i].delay_ms != 0U) &&
        (OV05C10_Delay(obj, table[i].delay_ms) != OV05C10_OK))
    {
      return OV05C10_ERROR;
    }
  }
  return OV05C10_OK;
}

static int32_t OV05C10_Trigger(OV05C10_Object_t *obj, uint8_t trigger)
{
  return OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_UPDATE, &trigger, 1U);
}

static uint32_t OV05C10_LinesToUs(const OV05C10_Object_t *obj, uint32_t lines)
{
  uint64_t denominator = (uint64_t)OV05C10_FPS * obj->Vts;
  return (uint32_t)(((uint64_t)lines * 1000000ULL + denominator / 2ULL) / denominator);
}

static uint32_t OV05C10_UsToLines(const OV05C10_Object_t *obj, uint32_t exposure_us)
{
  return (uint32_t)(((uint64_t)exposure_us * OV05C10_FPS * obj->Vts + 500000ULL) / 1000000ULL);
}

int32_t OV05C10_RegisterBusIO(OV05C10_Object_t *obj, OV05C10_IO_t *io)
{
  if ((obj == NULL) || (io == NULL) || (io->ReadReg == NULL) || (io->WriteReg == NULL))
  {
    return OV05C10_ERROR;
  }

  obj->IO = *io;
  obj->Ctx.ReadReg = OV05C10_ReadRegWrap;
  obj->Ctx.WriteReg = OV05C10_WriteRegWrap;
  obj->Ctx.handle = obj;

  if (obj->IO.Init == NULL)
  {
    return OV05C10_ERROR;
  }
  return obj->IO.Init();
}

int32_t OV05C10_ReadID(OV05C10_Object_t *obj, uint32_t *id)
{
  uint8_t data[4];

  if ((obj == NULL) || (id == NULL) ||
      (OV05C10_ReadPageReg(obj, OV05C10_PAGE_SYSTEM, OV05C10_REG_CHIP_ID, data, 4U) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  *id = ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
        ((uint32_t)data[2] << 8) | data[3];
  return OV05C10_OK;
}

int32_t OV05C10_Stop(OV05C10_Object_t *obj)
{
  uint8_t value;

  if (obj == NULL)
  {
    return OV05C10_ERROR;
  }

  value = 0x5bU;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_SYSTEM, 0x20U, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  value = 0x02U;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, 0x33U, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return OV05C10_Trigger(obj, OV05C10_UPDATE_MODE);
}

int32_t OV05C10_Init(OV05C10_Object_t *obj, uint32_t resolution, uint32_t pixel_format)
{
  uint8_t vts[3];

  (void)resolution;
  if ((obj == NULL) || (pixel_format != OV05C10_RAW10))
  {
    return OV05C10_ERROR;
  }
  if (obj->IsInitialized != 0U)
  {
    return OV05C10_OK;
  }

  if (OV05C10_WriteTable(obj, ov05c10_2888x1808_60fps,
                         (uint32_t)ARRAY_SIZE(ov05c10_2888x1808_60fps)) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_Stop(obj) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_ReadPageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_VTS, vts, 3U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }

  obj->Vts = ((uint32_t)vts[0] << 16) | ((uint32_t)vts[1] << 8) | vts[2];
  if (obj->Vts <= (OV05C10_EXPOSURE_MIN_LINES + OV05C10_EXPOSURE_MARGIN_LINES))
  {
    return OV05C10_ERROR;
  }

  /* The vendor mode table initializes P1:0x24 to its saturated 15.5x value. */
  obj->AppliedGainMdB = OV05C10_ANALOG_GAIN_MAX_MDB;
  obj->AppliedExposureUs = (int32_t)OV05C10_LinesToUs(obj, OV05C10_EXPOSURE_MIN_LINES);
  obj->IsInitialized = 1U;
  return OV05C10_OK;
}

int32_t OV05C10_Start(OV05C10_Object_t *obj)
{
  uint8_t value;

  if ((obj == NULL) || (obj->IsInitialized == 0U))
  {
    return OV05C10_ERROR;
  }

  value = 0x01U;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_SYSTEM, 0xa0U, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  value = 0x03U;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, 0x33U, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_Trigger(obj, OV05C10_UPDATE_MODE) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  value = 0x1fU;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_SYSTEM, 0x20U, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return OV05C10_SelectPage(obj, OV05C10_PAGE_CIS);
}

int32_t OV05C10_DeInit(OV05C10_Object_t *obj)
{
  int32_t ret;

  if (obj == NULL)
  {
    return OV05C10_ERROR;
  }
  ret = (obj->IsInitialized != 0U) ? OV05C10_Stop(obj) : OV05C10_OK;
  if (ret == OV05C10_OK)
  {
    obj->IsInitialized = 0U;
  }
  return ret;
}

int32_t OV05C10_SetExposure(OV05C10_Object_t *obj, int32_t exposure_us)
{
  uint32_t lines;
  uint32_t max_lines;
  uint8_t data[3];

  if ((obj == NULL) || (obj->IsInitialized == 0U))
  {
    return OV05C10_ERROR;
  }

  max_lines = obj->Vts - OV05C10_EXPOSURE_MARGIN_LINES;
  lines = (exposure_us <= 0) ? OV05C10_EXPOSURE_MIN_LINES :
          OV05C10_UsToLines(obj, (uint32_t)exposure_us);
  if (lines < OV05C10_EXPOSURE_MIN_LINES)
  {
    lines = OV05C10_EXPOSURE_MIN_LINES;
  }
  if (lines > max_lines)
  {
    lines = max_lines;
  }

  data[0] = (uint8_t)(lines >> 16);
  data[1] = (uint8_t)(lines >> 8);
  data[2] = (uint8_t)lines;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_EXPOSURE, data, 3U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_Trigger(obj, OV05C10_UPDATE_CONTROLS) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }

  obj->AppliedExposureUs = (int32_t)OV05C10_LinesToUs(obj, lines);
  return OV05C10_OK;
}

int32_t OV05C10_SetGain(OV05C10_Object_t *obj, int32_t gain_mdB)
{
  float requested;
  float analog;
  float digital;
  float applied;
  uint32_t analog_reg;
  uint32_t digital_reg;
  uint8_t analog_data;
  uint8_t digital_data[2];

  if ((obj == NULL) || (obj->IsInitialized == 0U))
  {
    return OV05C10_ERROR;
  }

  if (gain_mdB < OV05C10_GAIN_MIN_MDB)
  {
    gain_mdB = OV05C10_GAIN_MIN_MDB;
  }
  if (gain_mdB > OV05C10_GAIN_MAX_MDB)
  {
    gain_mdB = OV05C10_GAIN_MAX_MDB;
  }

  requested = powf(10.0f, (float)gain_mdB / 20000.0f);
  analog = (requested < 15.5f) ? requested : 15.5f;
  analog_reg = (uint32_t)lroundf(analog * 16.0f);
  if (analog_reg < OV05C10_ANALOG_GAIN_MIN_REG)
  {
    analog_reg = OV05C10_ANALOG_GAIN_MIN_REG;
  }
  if (analog_reg > OV05C10_ANALOG_GAIN_MAX_REG)
  {
    analog_reg = OV05C10_ANALOG_GAIN_MAX_REG;
  }

  analog = (float)analog_reg / 16.0f;
  digital = requested / analog;
  if (digital < 1.0f)
  {
    digital = 1.0f;
  }
  if (digital > 4.0f)
  {
    digital = 4.0f;
  }
  digital_reg = (uint32_t)lroundf(digital * 64.0f);
  if (digital_reg < OV05C10_DIGITAL_GAIN_MIN_REG)
  {
    digital_reg = OV05C10_DIGITAL_GAIN_MIN_REG;
  }
  if (digital_reg > OV05C10_DIGITAL_GAIN_MAX_REG)
  {
    digital_reg = OV05C10_DIGITAL_GAIN_MAX_REG;
  }

  analog_data = (uint8_t)analog_reg;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_ANALOG_GAIN,
                           &analog_data, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  digital_data[0] = (uint8_t)(digital_reg >> 8);
  digital_data[1] = (uint8_t)digital_reg;
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_DIGITAL_GAIN_H,
                           digital_data, 2U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_Trigger(obj, OV05C10_UPDATE_CONTROLS) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }

  applied = analog * ((float)digital_reg / 64.0f);
  obj->AppliedGainMdB = (int32_t)lroundf(20000.0f * log10f(applied));
  return OV05C10_OK;
}

int32_t OV05C10_GetAppliedGain(OV05C10_Object_t *obj, int32_t *gain_mdB)
{
  if ((obj == NULL) || (gain_mdB == NULL))
  {
    return OV05C10_ERROR;
  }
  *gain_mdB = obj->AppliedGainMdB;
  return OV05C10_OK;
}

int32_t OV05C10_GetAppliedExposure(OV05C10_Object_t *obj, int32_t *exposure_us)
{
  if ((obj == NULL) || (exposure_us == NULL))
  {
    return OV05C10_ERROR;
  }
  *exposure_us = obj->AppliedExposureUs;
  return OV05C10_OK;
}

int32_t OV05C10_MirrorFlipConfig(OV05C10_Object_t *obj, OV05C10_MirrorFlip_t config)
{
  uint8_t value;
  uint8_t bits;

  if ((obj == NULL) || (config > OV05C10_MIRROR_FLIP))
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_ReadPageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_MIRROR_FLIP, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }

  bits = 0U;
  if ((config == OV05C10_MIRROR) || (config == OV05C10_MIRROR_FLIP))
  {
    bits |= 0x01U;
  }
  if ((config == OV05C10_FLIP) || (config == OV05C10_MIRROR_FLIP))
  {
    bits |= 0x02U;
  }
  value = (uint8_t)((value & 0xfcU) | bits);
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_CIS, OV05C10_REG_MIRROR_FLIP, &value, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return OV05C10_Trigger(obj, OV05C10_UPDATE_MODE);
}

int32_t OV05C10_SetTestPattern(OV05C10_Object_t *obj, int32_t mode)
{
  uint8_t control = (mode != 0) ? 0x02U : 0x00U;
  uint8_t enable = (mode != 0) ? 0x01U : 0x00U;

  if ((obj == NULL) || ((mode != 0) && (mode != 1)))
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_ISP, 0xf3U, &control, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  if (OV05C10_WritePageReg(obj, OV05C10_PAGE_ISP, 0x12U, &enable, 1U) != OV05C10_OK)
  {
    return OV05C10_ERROR;
  }
  return OV05C10_Trigger(obj, OV05C10_UPDATE_CONTROLS);
}
