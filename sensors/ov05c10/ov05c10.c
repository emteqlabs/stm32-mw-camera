#include "ov05c10.h"

#include <math.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define OV05C10_FRAME_RATE_HZ              60ULL
#define OV05C10_USEC_PER_SECOND            1000000ULL
#define OV05C10_CONTROL_UPDATE              0x01U
#define OV05C10_ORIENTATION_UPDATE          0x02U

typedef enum
{
  OV05C10_TABLE_WRITE,
  OV05C10_TABLE_DELAY
} OV05C10_TableOp_t;

typedef struct
{
  OV05C10_TableOp_t op;
  uint8_t addr;
  uint8_t val;
} OV05C10_TableEntry_t;

#define OV05C10_REG(addr_, val_) {OV05C10_TABLE_WRITE, (addr_), (val_)}
#define OV05C10_DELAY(ms_)       {OV05C10_TABLE_DELAY, 0U, (ms_)}

OV05C10_CAMERA_Drv_t OV05C10_CAMERA_Driver =
{
  .Init = OV05C10_Init,
  .DeInit = OV05C10_DeInit,
  .ReadID = OV05C10_ReadID,
  .GetCapabilities = OV05C10_GetCapabilities,
  .SetGain = OV05C10_SetGain,
  .SetExposure = OV05C10_SetExposure,
  .SetFrequency = OV05C10_SetFrequency,
  .MirrorFlipConfig = OV05C10_MirrorFlipConfig,
  .GetSensorInfo = OV05C10_GetSensorInfo,
};

/*
 * Official OV05C10R1A AE02 linear mode:
 * 24 MHz ECLK, 2888x1808, RAW10, two lanes, 60 fps.
 * MIPI enable and stream-on are intentionally kept in OV05C10_Start().
 */
static const OV05C10_TableEntry_t OV05C10_Linear60Init[] =
{
  OV05C10_REG(0xFD, 0x00),
  OV05C10_REG(0x20, 0x00),
  OV05C10_REG(0xFD, 0x00),
  OV05C10_REG(0x20, 0x0B),
  OV05C10_REG(0xC1, 0x09),
  OV05C10_REG(0x21, 0x06),
  OV05C10_REG(0x14, 0x78),
  OV05C10_REG(0xE7, 0x03),
  OV05C10_REG(0xE7, 0x00),
  OV05C10_REG(0x21, 0x00),
  OV05C10_DELAY(1),

  OV05C10_REG(0xFD, 0x01),
  OV05C10_REG(0x03, 0x00),
  OV05C10_REG(0x04, 0x06),
  OV05C10_REG(0x07, 0x08),
  OV05C10_REG(0x1B, 0x01),
  OV05C10_REG(0x24, 0xFF),
  OV05C10_REG(0x42, 0x5D),
  OV05C10_REG(0x43, 0x08),
  OV05C10_REG(0x44, 0x81),
  OV05C10_REG(0x46, 0x5F),
  OV05C10_REG(0x48, 0x18),
  OV05C10_REG(0x49, 0x04),
  OV05C10_REG(0x5C, 0x18),
  OV05C10_REG(0x5E, 0x13),
  OV05C10_REG(0x70, 0x15),
  OV05C10_REG(0x77, 0x35),
  OV05C10_REG(0x79, 0x00),
  OV05C10_REG(0x7B, 0x08),
  OV05C10_REG(0x7D, 0x08),
  OV05C10_REG(0x7E, 0x08),
  OV05C10_REG(0x7F, 0x08),
  OV05C10_REG(0x90, 0x37),
  OV05C10_REG(0x91, 0x05),
  OV05C10_REG(0x92, 0x18),
  OV05C10_REG(0x93, 0x27),
  OV05C10_REG(0x94, 0x05),
  OV05C10_REG(0x95, 0x38),
  OV05C10_REG(0x9B, 0x00),
  OV05C10_REG(0x9C, 0x06),
  OV05C10_REG(0x9D, 0x28),
  OV05C10_REG(0x9E, 0x06),
  OV05C10_REG(0xB2, 0x0F),
  OV05C10_REG(0xB3, 0x29),
  OV05C10_REG(0xBF, 0x3C),
  OV05C10_REG(0xC2, 0x04),
  OV05C10_REG(0xC4, 0x00),
  OV05C10_REG(0xCA, 0x20),
  OV05C10_REG(0xCB, 0x20),
  OV05C10_REG(0xCC, 0x28),
  OV05C10_REG(0xCD, 0x28),
  OV05C10_REG(0xCE, 0x20),
  OV05C10_REG(0xCF, 0x20),
  OV05C10_REG(0xD0, 0x2A),
  OV05C10_REG(0xD1, 0x2A),

  OV05C10_REG(0xFD, 0x0F),
  OV05C10_REG(0x00, 0x00),
  OV05C10_REG(0x01, 0xA0),
  OV05C10_REG(0x02, 0x48),
  OV05C10_REG(0x07, 0x8E),
  OV05C10_REG(0x08, 0x70),
  OV05C10_REG(0x09, 0x01),
  OV05C10_REG(0x0B, 0x40),
  OV05C10_REG(0x0D, 0x07),
  OV05C10_REG(0x11, 0x33),
  OV05C10_REG(0x12, 0x77),
  OV05C10_REG(0x13, 0x66),
  OV05C10_REG(0x14, 0x65),
  OV05C10_REG(0x15, 0x37),
  OV05C10_REG(0x16, 0xBF),
  OV05C10_REG(0x17, 0xFF),
  OV05C10_REG(0x18, 0xFF),
  OV05C10_REG(0x19, 0x12),
  OV05C10_REG(0x1A, 0x10),
  OV05C10_REG(0x1C, 0x77),
  OV05C10_REG(0x1D, 0x77),
  OV05C10_REG(0x20, 0x0F),
  OV05C10_REG(0x21, 0x0F),
  OV05C10_REG(0x22, 0x0F),
  OV05C10_REG(0x23, 0x0F),
  OV05C10_REG(0x2B, 0x20),
  OV05C10_REG(0x2C, 0x20),
  OV05C10_REG(0x2D, 0x04),

  OV05C10_REG(0xFD, 0x03),
  OV05C10_REG(0x9D, 0x0F),
  OV05C10_REG(0x9F, 0x40),
  OV05C10_REG(0xFD, 0x00),
  OV05C10_REG(0x20, 0x1B),

  OV05C10_REG(0xFD, 0x04),
  OV05C10_REG(0x19, 0x60),

  OV05C10_REG(0xFD, 0x02),
  OV05C10_REG(0x75, 0x05),
  OV05C10_REG(0x7F, 0x06),
  OV05C10_REG(0x9A, 0x03),
  OV05C10_REG(0xA2, 0x07),
  OV05C10_REG(0xA3, 0x10),
  OV05C10_REG(0xA5, 0x02),
  OV05C10_REG(0xA6, 0x0B),
  OV05C10_REG(0xA7, 0x48),

  OV05C10_REG(0xFD, 0x07),
  OV05C10_REG(0x42, 0x00),
  OV05C10_REG(0x43, 0x80),
  OV05C10_REG(0x44, 0x00),
  OV05C10_REG(0x45, 0x80),
  OV05C10_REG(0x46, 0x00),
  OV05C10_REG(0x47, 0x80),
  OV05C10_REG(0x48, 0x00),
  OV05C10_REG(0x49, 0x80),
  OV05C10_REG(0x00, 0xF7),

  OV05C10_REG(0xFD, 0x00),
  OV05C10_REG(0xE7, 0x03),
  OV05C10_REG(0xE7, 0x00),
  OV05C10_REG(0xFD, 0x00),
  OV05C10_REG(0x93, 0x18),
  OV05C10_REG(0x94, 0xFF),
  OV05C10_REG(0x95, 0xBD),
  OV05C10_REG(0x96, 0x1A),
  OV05C10_REG(0x98, 0x04),
  OV05C10_REG(0x99, 0x08),
  OV05C10_REG(0x9B, 0x10),
  OV05C10_REG(0x9C, 0x3F),
  OV05C10_REG(0xA1, 0x05),
  OV05C10_REG(0xA4, 0x2F),
  OV05C10_REG(0xC0, 0x0C),
  OV05C10_REG(0xC1, 0x08),
  OV05C10_REG(0xC2, 0x00),
  OV05C10_REG(0xB6, 0x20),
  OV05C10_REG(0xBB, 0x80),
};

static int32_t OV05C10_ReadRegWrap(void *handle, uint8_t reg, uint8_t *data,
                                   uint16_t length)
{
  OV05C10_Object_t *pObj = (OV05C10_Object_t *)handle;
  return pObj->IO.ReadReg(pObj->IO.Address, reg, data, length);
}

static int32_t OV05C10_WriteRegWrap(void *handle, uint8_t reg, uint8_t *data,
                                    uint16_t length)
{
  OV05C10_Object_t *pObj = (OV05C10_Object_t *)handle;
  return pObj->IO.WriteReg(pObj->IO.Address, reg, data, length);
}

static int32_t OV05C10_Delay(OV05C10_Object_t *pObj, uint32_t delay)
{
  uint32_t start = (uint32_t)pObj->IO.GetTick();
  while (((uint32_t)pObj->IO.GetTick() - start) < delay)
  {
  }
  return OV05C10_OK;
}

static int32_t OV05C10_SelectPage(OV05C10_Object_t *pObj, uint8_t page)
{
  if ((pObj->PageValid != 0U) && (pObj->CurrentPage == page))
  {
    return OV05C10_OK;
  }

  if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &page, 1) != OV05C10_OK)
  {
    pObj->PageValid = 0U;
    return OV05C10_ERROR;
  }

  pObj->CurrentPage = page;
  pObj->PageValid = 1U;
  return OV05C10_OK;
}

static int32_t OV05C10_WriteByte(OV05C10_Object_t *pObj, uint8_t reg, uint8_t value)
{
  return ov05c10_write_reg(&pObj->Ctx, reg, &value, 1);
}

static int32_t OV05C10_ReadByte(OV05C10_Object_t *pObj, uint8_t reg, uint8_t *value)
{
  return ov05c10_read_reg(&pObj->Ctx, reg, value, 1);
}

static int32_t OV05C10_WriteTable(OV05C10_Object_t *pObj,
                                  const OV05C10_TableEntry_t *table,
                                  uint32_t count)
{
  uint32_t i;

  for (i = 0U; i < count; i++)
  {
    if (table[i].op == OV05C10_TABLE_DELAY)
    {
      if (OV05C10_Delay(pObj, table[i].val) != OV05C10_OK)
      {
        return OV05C10_ERROR;
      }
      continue;
    }

    if (OV05C10_WriteByte(pObj, table[i].addr, table[i].val) != OV05C10_OK)
    {
      pObj->PageValid = 0U;
      return OV05C10_ERROR;
    }

    if (table[i].addr == OV05C10_REG_PAGE_SELECT)
    {
      pObj->CurrentPage = table[i].val;
      pObj->PageValid = 1U;
    }
  }

  return OV05C10_OK;
}

static int32_t OV05C10_ReadVTS(OV05C10_Object_t *pObj, uint32_t *vts)
{
  uint8_t msb;
  uint8_t mid;
  uint8_t lsb;

  if ((vts == NULL) || (OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_VTS_MSB, &msb) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_VTS_MID, &mid) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_VTS_LSB, &lsb) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  *vts = ((uint32_t)(msb & 0x3FU) << 16) |
         ((uint32_t)mid << 8) |
         (uint32_t)lsb;

  return (*vts >= (OV05C10_EXPOSURE_MIN_LINES +
                   OV05C10_EXPOSURE_MAX_LINES_MARGIN)) ?
         OV05C10_OK : OV05C10_ERROR;
}

static uint32_t OV05C10_LinesToExposure(uint32_t lines, uint32_t vts)
{
  uint64_t denominator = OV05C10_FRAME_RATE_HZ * (uint64_t)vts;
  return (uint32_t)(((uint64_t)lines * OV05C10_USEC_PER_SECOND + denominator / 2ULL) /
                    denominator);
}

int32_t OV05C10_RegisterBusIO(OV05C10_Object_t *pObj, OV05C10_IO_t *pIO)
{
  if ((pObj == NULL) || (pIO == NULL))
  {
    return OV05C10_ERROR;
  }

  pObj->IO = *pIO;
  pObj->Ctx.ReadReg = OV05C10_ReadRegWrap;
  pObj->Ctx.WriteReg = OV05C10_WriteRegWrap;
  pObj->Ctx.handle = pObj;
  pObj->PageValid = 0U;

  return (pObj->IO.Init != NULL) ? pObj->IO.Init() : OV05C10_ERROR;
}

int32_t OV05C10_Init(OV05C10_Object_t *pObj, uint32_t resolution,
                     uint32_t pixelFormat)
{
  (void)pixelFormat;

  if ((pObj == NULL) || (resolution != OV05C10_RESOLUTION_2888_1808))
  {
    return OV05C10_ERROR;
  }

  if (pObj->IsInitialized == 0U)
  {
    pObj->PageValid = 0U;
    if (OV05C10_WriteTable(pObj, OV05C10_Linear60Init,
                           ARRAY_SIZE(OV05C10_Linear60Init)) != OV05C10_OK)
    {
      return OV05C10_ERROR;
    }

    pObj->AppliedExposure = 0;
    pObj->AppliedGain = 0;
    pObj->IsInitialized = 1U;
  }

  return OV05C10_OK;
}

int32_t OV05C10_DeInit(OV05C10_Object_t *pObj)
{
  if (pObj == NULL)
  {
    return OV05C10_ERROR;
  }

  pObj->IsInitialized = 0U;
  pObj->PageValid = 0U;
  return OV05C10_OK;
}

int32_t OV05C10_Start(OV05C10_Object_t *pObj)
{
  if (pObj == NULL)
  {
    return OV05C10_ERROR;
  }

  /* Preserve the vendor stream-on page sequence, including its first P0 write. */
  pObj->PageValid = 0U;
  if ((OV05C10_SelectPage(pObj, OV05C10_PAGE_MODE) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0xA0, 0x01) != OV05C10_OK) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x33, 0x03) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x01, OV05C10_ORIENTATION_UPDATE) != OV05C10_OK) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_MODE) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_MODE_SELECT,
                         OV05C10_MODE_STREAMING) != OV05C10_OK) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  return OV05C10_OK;
}

int32_t OV05C10_Stop(OV05C10_Object_t *pObj)
{
  if (pObj == NULL)
  {
    return OV05C10_ERROR;
  }

  pObj->PageValid = 0U;
  if ((OV05C10_SelectPage(pObj, OV05C10_PAGE_MODE) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_MODE_SELECT,
                         OV05C10_MODE_STANDBY) != OV05C10_OK) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x33, 0x02) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x01, OV05C10_ORIENTATION_UPDATE) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  OV05C10_Delay(pObj, 20U);
  return OV05C10_OK;
}

int32_t OV05C10_ReadID(OV05C10_Object_t *pObj, uint32_t *id)
{
  uint8_t bytes[4];

  if ((pObj == NULL) || (id == NULL) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_ID) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_ID_BYTE_3, &bytes[0]) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_ID_BYTE_2, &bytes[1]) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_ID_BYTE_1, &bytes[2]) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_ID_BYTE_0, &bytes[3]) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  *id = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
        ((uint32_t)bytes[2] << 8) | (uint32_t)bytes[3];
  return OV05C10_OK;
}

int32_t OV05C10_GetCapabilities(OV05C10_Object_t *pObj,
                                OV05C10_Capabilities_t *capabilities)
{
  if ((pObj == NULL) || (capabilities == NULL))
  {
    return OV05C10_ERROR;
  }

  memset(capabilities, 0, sizeof(*capabilities));
  capabilities->Config_Gain = 1U;
  capabilities->Config_Exposure = 1U;
  capabilities->Config_MirrorFlip = 1U;
  capabilities->Config_SensorInfo = 1U;
  return OV05C10_OK;
}

int32_t OV05C10_GetSensorInfo(OV05C10_Object_t *pObj,
                              OV05C10_SensorInfo_t *info)
{
  uint32_t vts;

  if ((pObj == NULL) || (info == NULL) ||
      (sizeof(info->name) < (strlen(OV05C10_NAME) + 1U)) ||
      (OV05C10_ReadVTS(pObj, &vts) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  strcpy(info->name, OV05C10_NAME);
  info->bayer_pattern = OV05C10_BAYER_PATTERN;
  info->color_depth = OV05C10_COLOR_DEPTH;
  info->width = OV05C10_WIDTH;
  info->height = OV05C10_HEIGHT;
  info->gain_min = OV05C10_GAIN_MIN;
  info->gain_max = OV05C10_GAIN_MAX;
  info->exposure_min = OV05C10_LinesToExposure(OV05C10_EXPOSURE_MIN_LINES, vts);
  info->exposure_max = OV05C10_LinesToExposure(
      vts - OV05C10_EXPOSURE_MAX_LINES_MARGIN, vts);
  return OV05C10_OK;
}

int32_t OV05C10_SetExposure(OV05C10_Object_t *pObj, int32_t exposureUs)
{
  uint32_t vts;
  uint32_t lines;
  uint32_t maxLines;
  uint64_t scaled;

  if ((pObj == NULL) || (OV05C10_ReadVTS(pObj, &vts) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  maxLines = vts - OV05C10_EXPOSURE_MAX_LINES_MARGIN;
  if (exposureUs <= 0)
  {
    lines = OV05C10_EXPOSURE_MIN_LINES;
  }
  else
  {
    scaled = (uint64_t)(uint32_t)exposureUs * OV05C10_FRAME_RATE_HZ * (uint64_t)vts;
    lines = (uint32_t)((scaled + OV05C10_USEC_PER_SECOND / 2ULL) /
                       OV05C10_USEC_PER_SECOND);
    if (lines < OV05C10_EXPOSURE_MIN_LINES)
    {
      lines = OV05C10_EXPOSURE_MIN_LINES;
    }
    else if (lines > maxLines)
    {
      lines = maxLines;
    }
  }

  if ((OV05C10_WriteByte(pObj, OV05C10_REG_EXPOSURE_MSB,
                         (uint8_t)((lines >> 16) & 0x7FU)) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_EXPOSURE_MID,
                         (uint8_t)((lines >> 8) & 0xFFU)) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_EXPOSURE_LSB,
                         (uint8_t)(lines & 0xFFU)) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x01, OV05C10_CONTROL_UPDATE) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  pObj->AppliedExposure = (int32_t)OV05C10_LinesToExposure(lines, vts);
  return OV05C10_OK;
}

int32_t OV05C10_GetAppliedExposure(OV05C10_Object_t *pObj, int32_t *exposureUs)
{
  if ((pObj == NULL) || (exposureUs == NULL))
  {
    return OV05C10_ERROR;
  }
  *exposureUs = pObj->AppliedExposure;
  return OV05C10_OK;
}

int32_t OV05C10_SetGain(OV05C10_Object_t *pObj, int32_t gainMdB)
{
  float requested;
  float analogTarget;
  float analogApplied;
  float digitalTarget;
  float applied;
  uint32_t analogCode;
  uint32_t digitalCode;
  int32_t appliedMdB;

  if (pObj == NULL)
  {
    return OV05C10_ERROR;
  }

  if (gainMdB < OV05C10_GAIN_MIN)
  {
    gainMdB = OV05C10_GAIN_MIN;
  }
  else if (gainMdB > OV05C10_GAIN_MAX)
  {
    gainMdB = OV05C10_GAIN_MAX;
  }

  requested = powf(10.0f, (float)gainMdB / 20000.0f);
  analogTarget = (requested < OV05C10_ANALOG_GAIN_MAX_LINEAR) ?
                 requested : OV05C10_ANALOG_GAIN_MAX_LINEAR;
  analogCode = (uint32_t)lroundf(analogTarget * 16.0f);
  if (analogCode < OV05C10_ANALOG_GAIN_MIN_REG)
  {
    analogCode = OV05C10_ANALOG_GAIN_MIN_REG;
  }
  else if (analogCode > OV05C10_ANALOG_GAIN_MAX_REG)
  {
    analogCode = OV05C10_ANALOG_GAIN_MAX_REG;
  }

  analogApplied = (float)analogCode / 16.0f;
  digitalTarget = requested / analogApplied;
  digitalCode = (uint32_t)lroundf(digitalTarget * 64.0f);
  if (digitalCode < OV05C10_DIGITAL_GAIN_MIN_REG)
  {
    digitalCode = OV05C10_DIGITAL_GAIN_MIN_REG;
  }
  else if (digitalCode > OV05C10_DIGITAL_GAIN_MAX_REG)
  {
    digitalCode = OV05C10_DIGITAL_GAIN_MAX_REG;
  }

  if ((OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_ANALOG_GAIN,
                         (uint8_t)analogCode) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_DIGITAL_GAIN_MSB,
                         (uint8_t)((digitalCode >> 8) & 0xFFU)) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, OV05C10_REG_DIGITAL_GAIN_LSB,
                         (uint8_t)(digitalCode & 0xFFU)) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x01, OV05C10_CONTROL_UPDATE) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  applied = analogApplied * ((float)digitalCode / 64.0f);
  appliedMdB = (int32_t)lroundf(20000.0f * log10f(applied));
  if (appliedMdB < OV05C10_GAIN_MIN)
  {
    appliedMdB = OV05C10_GAIN_MIN;
  }
  else if (appliedMdB > OV05C10_GAIN_MAX)
  {
    appliedMdB = OV05C10_GAIN_MAX;
  }
  pObj->AppliedGain = appliedMdB;
  return OV05C10_OK;
}

int32_t OV05C10_GetAppliedGain(OV05C10_Object_t *pObj, int32_t *gainMdB)
{
  if ((pObj == NULL) || (gainMdB == NULL))
  {
    return OV05C10_ERROR;
  }
  *gainMdB = pObj->AppliedGain;
  return OV05C10_OK;
}

int32_t OV05C10_MirrorFlipConfig(OV05C10_Object_t *pObj,
                                 OV05C10_MirrorFlip_t config)
{
  uint8_t orientation;
  uint8_t bits;

  switch (config)
  {
    case OV05C10_FLIP:
      bits = OV05C10_ORIENTATION_FLIP;
      break;
    case OV05C10_MIRROR:
      bits = OV05C10_ORIENTATION_MIRROR;
      break;
    case OV05C10_MIRROR_FLIP:
      bits = OV05C10_ORIENTATION_MIRROR | OV05C10_ORIENTATION_FLIP;
      break;
    case OV05C10_MIRROR_FLIP_NONE:
      bits = 0U;
      break;
    default:
      return OV05C10_ERROR;
  }

  if ((pObj == NULL) ||
      (OV05C10_SelectPage(pObj, OV05C10_PAGE_ANALOG) != OV05C10_OK) ||
      (OV05C10_ReadByte(pObj, OV05C10_REG_ORIENTATION, &orientation) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  orientation = (uint8_t)((orientation &
                           (uint8_t)~(OV05C10_ORIENTATION_MIRROR |
                                     OV05C10_ORIENTATION_FLIP)) | bits);
  if ((OV05C10_WriteByte(pObj, OV05C10_REG_ORIENTATION, orientation) != OV05C10_OK) ||
      (OV05C10_WriteByte(pObj, 0x01, OV05C10_ORIENTATION_UPDATE) != OV05C10_OK))
  {
    return OV05C10_ERROR;
  }

  return OV05C10_OK;
}

int32_t OV05C10_SetFrequency(OV05C10_Object_t *pObj, int32_t frequency)
{
  (void)pObj;
  (void)frequency;
  return OV05C10_ERROR;
}

int32_t OV05C10_SetFramerate(OV05C10_Object_t *pObj, int32_t framerate)
{
  (void)pObj;
  (void)framerate;
  return OV05C10_ERROR;
}
