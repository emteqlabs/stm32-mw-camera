#include "ov05c10.h"
#include <string.h>
#include <math.h>

#include <stdio.h>

#define SWAP_ENDIAN16(x) ( ((x) >> 8) | ((x) << 8) )

#define SWAP_ENDIAN32(x) 										\
						(										\
								(((x) & 0x000000FF) << 24) | 	\
								(((x) & 0x0000FF00) <<  8) | 	\
								(((x) & 0x00FF0000) >>  8) | 	\
								(((x) & 0xFF000000) >> 24)		\
						)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MDECIBEL_TO_LINEAR(mdB)             (powf(10.0f, (mdB / 1000.0f) / 20.0f))
#define LINEAR_TO_MDECIBEL(linearValue)     (1000 * (20.0f * log10f(linearValue)))

OV05C10_CAMERA_Drv_t OV05C10_CAMERA_Driver = {
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

struct regval {
	uint8_t addr;
	uint8_t val;
};

/*
 * OV05C10 SCCB access is page + 8-bit offset.
 * All accesses use explicit page selection (0xFD) followed by 8-bit offsets.
 */
#define OV05C10_REG_PAGE_SELECT 0x00FD

static const struct regval ov05c10r1a_ae02_init_2888x1808_2lane_raw10_60fps[] = {
	/* reset and base bring-up */
	{0xFD, 0x00},
	{0x20, 0x00},
	{0xFD, 0x00},
	{0x20, 0x0b},
	{0xC1, 0x09},
	{0x21, 0x06},
	{0x14, 0x78},
	{0xE7, 0x03},
	{0xE7, 0x00},
	{0x21, 0x00},
	{0x14, 0x96},
	{0x1B, 0xBB},

	/* page 0x01 */
	{0xFD, 0x01},
	{0x03, 0x00},
	{0x04, 0x06},
	{0x07, 0x08},
	{0x1B, 0x01},
	{0x24, 0xFF},
	{0x42, 0x5D},
	{0x43, 0x08},
	{0x44, 0x81},
	{0x46, 0x5F},
	{0x48, 0x18},
	{0x49, 0x04},
	{0x5C, 0x18},
	{0x5E, 0x13},
	{0x70, 0x15},
	{0x77, 0x35},
	{0x79, 0x00},
	{0x7B, 0x08},
	{0x7D, 0x08},
	{0x7E, 0x08},
	{0x7F, 0x08},
	{0x90, 0x37},
	{0x91, 0x05},
	{0x92, 0x18},
	{0x93, 0x27},
	{0x94, 0x05},
	{0x95, 0x38},
	{0x9B, 0x00},
	{0x9C, 0x06},
	{0x9D, 0x28},
	{0x9E, 0x06},
	{0xB2, 0x0D},
	{0xB3, 0x29},
	{0xBF, 0x3C},
	{0xC2, 0x04},
	{0xC4, 0x00},
	{0xCA, 0x20},
	{0xCB, 0x20},
	{0xCC, 0x28},
	{0xCD, 0x28},
	{0xCE, 0x20},
	{0xCF, 0x20},
	{0xD0, 0x2A},
	{0xD1, 0x2A},

	/* page 0x0F */
	{0xFD, 0x0F},
	{0x00, 0x00},
	{0x01, 0xA0},
	{0x02, 0x48},
	{0x07, 0x8E},
	{0x08, 0x70},
	{0x09, 0x01},
	{0x0B, 0x40},
	{0x0D, 0x07},
	{0x11, 0x33},
	{0x12, 0x77},
	{0x13, 0x66},
	{0x14, 0x65},
	{0x15, 0x37},
	{0x16, 0xBF},
	{0x17, 0xFF},
	{0x18, 0xFF},
	{0x19, 0x12},
	{0x1A, 0x10},
	{0x1C, 0x77},
	{0x1D, 0x77},
	{0x20, 0x0F},
	{0x21, 0x0F},
	{0x22, 0x0F},
	{0x23, 0x0F},
	{0x2B, 0x20},
	{0x2C, 0x20},
	{0x2D, 0x04},

	/* page 0x03 */
	{0xFD, 0x03},
	{0x9D, 0x0F},
	{0x9F, 0x40},
	{0xFD, 0x00},
	{0x20, 0x1B},

	/* page 0x04 */
	{0xFD, 0x04},
	{0x19, 0x60},

	/* page 0x02 */
	{0xFD, 0x02},
	{0x75, 0x05},
	{0x7F, 0x06},
	{0x9A, 0x03},
	{0xA2, 0x07},
	{0xA3, 0x10},
	{0xA5, 0x02},
	{0xA6, 0x0B},
	{0xA7, 0x48},

	/* page 0x07 */
	{0xFD, 0x07},
	{0x42, 0x00},
	{0x43, 0x80},
	{0x44, 0x00},
	{0x45, 0x80},
	{0x46, 0x00},
	{0x47, 0x80},
	{0x48, 0x00},
	{0x49, 0x80},
	{0x00, 0xF7},

	/* finalize on page 0x00 */
	{0xFD, 0x00},
	{0xE7, 0x03},
	{0xE7, 0x00},
	{0xFD, 0x00},
	{0x8E, 0x0B},
	{0x8F, 0x48},
	{0x93, 0x18},
	{0x94, 0xFF},
	{0x95, 0xBD},
	{0x96, 0x1A},
	{0x98, 0x04},
	{0x99, 0x08},
	{0x9B, 0x10},
	{0x9C, 0x3F},
	{0xA1, 0x05},
	{0xA4, 0x2F},
	{0xC0, 0x0C},
	{0xC1, 0x08},
	{0xC2, 0x00},
	{0xB6, 0x20},
	{0xBB, 0x80},
	{0xFD, 0x00},
	{0xA0, 0x00},
	{0xFD, 0x01},
};

static const struct regval ov05c10r1a_ae02_init_2888x1808_2lane_raw10_30fps[] = {
	/* reset and base bring-up */
	{0xFD, 0x00},
	{0x20, 0x00},
	{0xFD, 0x00},
	{0x20, 0x0B},
	{0xC1, 0x09},
	{0x21, 0x06},
	{0x14, 0x78},
	{0xE7, 0x03},
	{0xE7, 0x00},
	{0x21, 0x00},

	/* 19.2 MHz */
	{0x14, 0x96},
	{0x1B, 0xBB},

	/* page 0x01 */
	{0xFD, 0x01},
	{0x03, 0x00},
	{0x04, 0x06},
	{0x05, 0x06}, /* VTS, approximately 30.5 fps */
	{0x06, 0xF4}, /* VTS, approximately 30.5 fps */
	{0x07, 0x08},
	{0x1B, 0x01},
	{0x24, 0xFF},
	{0x42, 0x5D},
	{0x43, 0x08},
	{0x44, 0x81},
	{0x46, 0x5F},
	{0x48, 0x18},
	{0x49, 0x04},
	{0x5C, 0x18},
	{0x5E, 0x13},
	{0x70, 0x15},
	{0x77, 0x35},
	{0x79, 0x00},
	{0x7B, 0x08},
	{0x7D, 0x08},
	{0x7E, 0x08},
	{0x7F, 0x08},
	{0x90, 0x37},
	{0x91, 0x05},
	{0x92, 0x18},
	{0x93, 0x27},
	{0x94, 0x05},
	{0x95, 0x38},
	{0x9B, 0x00},
	{0x9C, 0x06},
	{0x9D, 0x28},
	{0x9E, 0x06},
	{0xB2, 0x0D},
	{0xB3, 0x29},
	{0xBF, 0x3C},
	{0xC2, 0x04},
	{0xC4, 0x00},
	{0xCA, 0x20},
	{0xCB, 0x20},
	{0xCC, 0x28},
	{0xCD, 0x28},
	{0xCE, 0x20},
	{0xCF, 0x20},
	{0xD0, 0x2A},
	{0xD1, 0x2A},

	/* page 0x0F */
	{0xFD, 0x0F},
	{0x00, 0x00},
	{0x01, 0xA0},
	{0x02, 0x48},
	{0x07, 0x8E},
	{0x08, 0x70},
	{0x09, 0x01},
	{0x0B, 0x40},
	{0x0D, 0x07},
	{0x11, 0x33},
	{0x12, 0x77},
	{0x13, 0x66},
	{0x14, 0x65},
	{0x15, 0x37},
	{0x16, 0xBF},
	{0x17, 0xFF},
	{0x18, 0xFF},
	{0x19, 0x12},
	{0x1A, 0x10},
	{0x1C, 0x77},
	{0x1D, 0x77},
	{0x20, 0x0F},
	{0x21, 0x0F},
	{0x22, 0x0F},
	{0x23, 0x0F},
	{0x2B, 0x20},
	{0x2C, 0x20},
	{0x2D, 0x04},

	/* page 0x03 */
	{0xFD, 0x03},
	{0x9D, 0x0F},
	{0x9F, 0x40},

	/* page 0x00 */
	{0xFD, 0x00},
	{0x20, 0x1B},

	/* page 0x04 */
	{0xFD, 0x04},
	{0x19, 0x60},

	/* page 0x02 */
	{0xFD, 0x02},
	{0x75, 0x05},
	{0x7F, 0x06},
	{0x9A, 0x03},
	{0xA1, 0x01}, /* GRBG */
	{0xA2, 0x07},
	{0xA3, 0x10},
	{0xA5, 0x02},
	{0xA6, 0x0B}, /* hsize[11:8] */
	{0xA7, 0x48}, /* hsize[7:0] */

	/* page 0x07 */
	{0xFD, 0x07},
	{0x42, 0x00},
	{0x43, 0x80},
	{0x44, 0x00},
	{0x45, 0x80},
	{0x46, 0x00},
	{0x47, 0x80},
	{0x48, 0x00},
	{0x49, 0x80},
	{0x00, 0xF7},

	/* finalise on page 0x00 */
	{0xFD, 0x00},
	{0xE7, 0x03},
	{0xE7, 0x00},
	{0xFD, 0x00},
	{0x8E, 0x0B}, /* hsize[11:8] */
	{0x8F, 0x48}, /* hsize[7:0] */
	{0x93, 0x18},
	{0x94, 0xFF},
	{0x95, 0xBD},
	{0x96, 0x1A},
	{0x98, 0x04},
	{0x99, 0x08},
	{0x9B, 0x10},
	{0x9C, 0x3F},
	{0xA1, 0x05},
	{0xA4, 0x2F},
	{0xC0, 0x0C},
	{0xC1, 0x08},
	{0xC2, 0x00},
	{0xB6, 0x20},
	{0xBB, 0x80},
	{0xFD, 0x00},
	{0xA0, 0x00}, /* disable MIPI */

	/* page 0x01 */
	{0xFD, 0x01},
	{0x33, 0x03},
	{0x01, 0x02},

	/* final state */
	{0xFD, 0x00},
	{0x20, 0x1F},
	{0xFD, 0x01},
};


static int32_t OV05C10_WriteTable(OV05C10_Object_t *pObj, const struct regval *regs,
		uint32_t size);
static int32_t OV05C10_StreamControl(OV05C10_Object_t *pObj, uint8_t enable);
static int32_t OV05C10_SelectPage(OV05C10_Object_t *pObj, uint8_t page);
static int32_t OV05C10_ReadRegWrap(void *handle, uint8_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV05C10_WriteRegWrap(void *handle, uint8_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV05C10_Delay(OV05C10_Object_t *pObj, uint32_t Delay);

static int32_t OV05C10_SetAnalogGain(OV05C10_Object_t *pObj, float gain_dBm);
static int32_t OV05C10_SetDigitalGain(OV05C10_Object_t *pObj, float gain_dBm);

static int32_t OV05C10_GetPCLK(OV05C10_Object_t *pObj, uint64_t *pclk);
static int32_t OV05C10_GetExposureRange(OV05C10_Object_t *pObj, uint32_t *min_us,
		uint32_t *max_us);

static uint32_t map_range_to_uint32(float value, float in_min, float in_max,
		uint32_t out_min, uint32_t out_max) {
	if (value < in_min)
		value = in_min;
	if (value > in_max)
		value = in_max;

	float normalized = (value - in_min) / (in_max - in_min);

	float mapped = out_min + normalized * (float) (out_max - out_min);

	return (uint32_t) roundf(mapped);
}

static int32_t OV05C10_WriteTable(OV05C10_Object_t *pObj, const struct regval *regs,
		uint32_t size) {
	uint32_t index;
	int32_t ret = OV05C10_OK;

	/* Set registers */
	for (index = 0; index < size; index++) {
		if (ret != OV05C10_ERROR) {
			if (ov05c10_write_reg(&pObj->Ctx, (uint8_t) regs[index].addr,
					(uint8_t*) &(regs[index].val), 1) != OV05C10_OK) {
				ret = OV05C10_ERROR;
			}
		}
	}
	return ret;
}

static int32_t OV05C10_Delay(OV05C10_Object_t *pObj, uint32_t Delay) {
	uint32_t tickstart;
	tickstart = pObj->IO.GetTick();
	while ((pObj->IO.GetTick() - tickstart) < Delay) {
	}
	return OV05C10_OK;
}

static int32_t OV05C10_ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV05C10_Object_t *pObj = (OV05C10_Object_t*) handle;

	return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

static int32_t OV05C10_WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV05C10_Object_t *pObj = (OV05C10_Object_t*) handle;

	return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

int32_t OV05C10_RegisterBusIO(OV05C10_Object_t *pObj, OV05C10_IO_t *pIO) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV05C10_ERROR;
	} else {
		pObj->IO.Init = pIO->Init;
		pObj->IO.DeInit = pIO->DeInit;
		pObj->IO.Address = pIO->Address;
		pObj->IO.WriteReg = pIO->WriteReg;
		pObj->IO.ReadReg = pIO->ReadReg;
		pObj->IO.GetTick = pIO->GetTick;

		pObj->Ctx.ReadReg = OV05C10_ReadRegWrap;
		pObj->Ctx.WriteReg = OV05C10_WriteRegWrap;
		pObj->Ctx.handle = pObj;
		pObj->PageValid = 0U;

		if (pObj->IO.Init != NULL) {
			ret = pObj->IO.Init();
		} else {
			ret = OV05C10_ERROR;
		}
	}

	return ret;
}

int32_t OV05C10_Init(OV05C10_Object_t *pObj, uint32_t Resolution,
		uint32_t PixelFormat) {
	(void)PixelFormat;
	int32_t ret = OV05C10_OK;

	if (pObj->IsInitialized == 0U) {
		pObj->PageValid = 0U;
		switch (Resolution) {
		case OV05C10_RESOLUTION_1928_1082:
			if (OV05C10_WriteTable(pObj,
					ov05c10r1a_ae02_init_2888x1808_2lane_raw10_30fps,
					ARRAY_SIZE(
							ov05c10r1a_ae02_init_2888x1808_2lane_raw10_30fps)) != OV05C10_OK) {
				ret = OV05C10_ERROR;
			}
			break;
			/* Add new resolution here */
		default:
			/* Resolution not supported */
			ret = OV05C10_ERROR;
		}

		if (!ret) {
			pObj->IsInitialized = 1U;
		}
	}

	// get PCLK for exposure control
	ret = OV05C10_GetPCLK(pObj, &pObj->Pclk);

	return ret;
}

int32_t OV05C10_DeInit(OV05C10_Object_t *pObj) {
	if (pObj->IsInitialized == 1U) {
		/* De-initialize camera sensor interface */
		pObj->IsInitialized = 0U;
	}
	pObj->PageValid = 0U;

	return OV05C10_OK;
}

int32_t OV05C10_Start(OV05C10_Object_t *pObj) {
	return OV05C10_StreamControl(pObj, 1U);
}

int32_t OV05C10_Stop(OV05C10_Object_t *pObj) {
	return OV05C10_StreamControl(pObj, 0U);
}

static int32_t OV05C10_StreamControl(OV05C10_Object_t *pObj, uint8_t enable) {
	int32_t ret = OV05C10_OK;
	uint8_t val;

	if (enable) {
        /* Enable the MIPI transmitter, as done by the Linux driver. */
        if (OV05C10_SelectPage(pObj, 0x00) != OV05C10_OK)
            return OV05C10_ERROR;

        val = 0x01;
        if (ov05c10_write_reg(&pObj->Ctx, 0xA0, &val, 1) !=
            OV05C10_OK)
            return OV05C10_ERROR;

        if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK)
            return OV05C10_ERROR;

        val = 0x03;
        if (ov05c10_write_reg(&pObj->Ctx, 0x33, &val, 1) !=
            OV05C10_OK)
            return OV05C10_ERROR;

        val = 0x02;
        if (ov05c10_write_reg(&pObj->Ctx, 0x01, &val, 1) !=
            OV05C10_OK)
            return OV05C10_ERROR;

        if (OV05C10_SelectPage(pObj, 0x00) != OV05C10_OK)
            return OV05C10_ERROR;

        val = 0x1F;
        return ov05c10_write_reg(&pObj->Ctx, 0x20, &val, 1);
	} else {
		if (OV05C10_SelectPage(pObj, 0x00) != OV05C10_OK) {
			ret = OV05C10_ERROR;
		}
		val = 0x5B;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x20, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		if ((ret == OV05C10_OK)
				&& (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x02;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x33, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x02;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x01, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
	}

	OV05C10_Delay(pObj, 20);
	return ret;
}

int32_t OV05C10_ReadID(OV05C10_Object_t *pObj, uint32_t *id)
{
  int32_t ret = OV05C10_OK;
  uint8_t tmp[4];

  if ((pObj == NULL) || (id == NULL) || (pObj->IO.Init == NULL) || (pObj->IO.ReadReg == NULL))
  {
    return OV05C10_ERROR;
  }

  /* Initialize I2C */
  pObj->IO.Init();

  if ((OV05C10_SelectPage(pObj, OV05C10_PAGE_ID) != OV05C10_OK)
      || (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_3, &tmp[3], 1) != OV05C10_OK))
  {
    ret = OV05C10_ERROR;
  }
  else
  {
    if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_2, &tmp[2], 1) != OV05C10_OK)
    {
      ret = OV05C10_ERROR;
    }
    else
    {
      if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_1, &tmp[1], 1) != OV05C10_OK)
      {
        ret = OV05C10_ERROR;
      }
      else
      {
        if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_0, &tmp[0], 1) != OV05C10_OK)
        {
          ret = OV05C10_ERROR;
        }
        else
        {
          ret = OV05C10_OK;
        }
      }
    }
  }

  if (!ret)
  {
    *id = ((uint32_t)tmp[3] << 24) |
          ((uint32_t)tmp[2] << 16) |
          ((uint32_t)tmp[1] << 8)  |
          ((uint32_t)tmp[0] << 0);
  }

  return ret;
}

int32_t OV05C10_GetCapabilities(OV05C10_Object_t *pObj,
		OV05C10_Capabilities_t *Capabilities) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV05C10_ERROR;
	} else {
		Capabilities->Config_Brightness = 0;
		Capabilities->Config_Contrast = 0;
		Capabilities->Config_HueDegree = 0;
		Capabilities->Config_Gain = 1;
		Capabilities->Config_Exposure = 1;
		Capabilities->Config_ExposureMode = 0;
		Capabilities->Config_LightMode = 0;
		Capabilities->Config_MirrorFlip = 1;
		Capabilities->Config_NightMode = 0;
		Capabilities->Config_Resolution = 0;
		Capabilities->Config_Saturation = 0;
		Capabilities->Config_SpecialEffect = 0;
		Capabilities->Config_Zoom = 0;
		Capabilities->Config_SensorInfo = 1;
		Capabilities->Config_TestPattern = 0;

		ret = OV05C10_OK;
	}

	return ret;
}

int32_t OV05C10_GetSensorInfo(OV05C10_Object_t *pObj, OV05C10_SensorInfo_t *Info) {
	if ((!pObj) || (Info == NULL)) {
		return OV05C10_ERROR;
	}

	if (sizeof(Info->name) >= strlen(OV05C10_NAME) + 1) {
		strcpy(Info->name, OV05C10_NAME);
	} else {
		return OV05C10_ERROR;
	}

	Info->bayer_pattern = OV05C10_BAYER_PATTERN;
	Info->color_depth = OV05C10_COLOR_DEPTH;
	Info->width = OV05C10_WIDTH;
	Info->height = OV05C10_HEIGHT;
	Info->gain_min = OV05C10_GAIN_MIN;
	Info->gain_max = OV05C10_GAIN_MAX;
	if (OV05C10_GetExposureRange(pObj, &Info->exposure_min,
			&Info->exposure_max) != OV05C10_OK) {
		// defaults
		Info->exposure_min = OV05C10_EXPOSURE_MIN;
		Info->exposure_max = OV05C10_EXPOSURE_MAX;
	}
	return OV05C10_OK;
}

static int32_t OV05C10_SetAnalogGain(OV05C10_Object_t *pObj, float gain_dBm) {
	int ret = OV05C10_OK;
	if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK) {
		return OV05C10_ERROR;
	}
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV05C10_ANALOG_GAIN_MAX_DBM) {
		gain_dBm = OV05C10_ANALOG_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV05C10_ANALOG_GAIN_MAX_LINEAR) {
		linear_gain = OV05C10_ANALOG_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint16_t reg_value = (uint16_t) map_range_to_uint32(linear_gain, 1.0f,
			OV05C10_ANALOG_GAIN_MAX_LINEAR, OV05C10_ANALOG_GAIN_MIN_REG,
			OV05C10_ANALOG_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN16(reg_value);
	if (ov05c10_write_reg(&pObj->Ctx, 0x24,
			(uint8_t*) &reg_value, 1) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_gain;
	}
	exit_gain: return ret;
}

static int32_t OV05C10_SetDigitalGain(OV05C10_Object_t *pObj, float gain_dBm) {
	int ret = OV05C10_OK;
	(void)pObj;
	(void)gain_dBm;
	return OV05C10_OK;

	/* Unused for current OVD mode, kept for API compatibility. */
	/*
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV05C10_DIGITAL_GAIN_MAX_DBM) {
		gain_dBm = OV05C10_DIGITAL_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV05C10_DIGITAL_GAIN_MAX_LINEAR) {
		linear_gain = OV05C10_DIGITAL_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint32_t reg_value = map_range_to_uint32(linear_gain, 1.0f,
			OV05C10_DIGITAL_GAIN_MAX_LINEAR, OV05C10_DIGITAL_GAIN_MIN_REG,
			OV05C10_DIGITAL_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN32(reg_value);
		if (ov05c10_write_reg(&pObj->Ctx, (uint8_t) OV05C10_REG_DIGITAL_GAIN,
				((uint8_t*) &reg_value) + 1, 3) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_gain;
	}
	exit_gain: return ret;
	*/
}

int32_t OV05C10_SetGain(OV05C10_Object_t *pObj, int32_t gain_dBm) {
	int32_t ret = 0;

	if (gain_dBm < OV05C10_GAIN_MIN) {
		gain_dBm = OV05C10_GAIN_MIN;
	} else if (gain_dBm > OV05C10_GAIN_MAX) {
		gain_dBm = OV05C10_GAIN_MAX;
	}

	float analog_gain_dBm =
			(gain_dBm > OV05C10_ANALOG_GAIN_MAX_DBM) ?
					OV05C10_ANALOG_GAIN_MAX_DBM : gain_dBm;
	float digital_gain_dBm = gain_dBm - analog_gain_dBm;
	if (OV05C10_SetAnalogGain(pObj, analog_gain_dBm) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_gain;
	}
	if (OV05C10_SetDigitalGain(pObj, digital_gain_dBm) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_gain;
	}

	exit_gain: return ret;
}

static int32_t OV05C10_GetPCLK(OV05C10_Object_t *pObj, uint64_t *pclk) {
	int32_t ret;
	const double iclk = 19.2;   // MHz
	const double osc_clk = 144.0;

	uint8_t r10, r11, r12, r13, r14, r15, r19, r1a, r1b, r1c, r1d, r1e;
	double mpll_mc = 1.0, dpll_mc = 1.0, rowclk = 1.0;

	ret = OV05C10_SelectPage(pObj, 0x00);
	if (ret != OV05C10_OK)
		goto exit_pclk;

	ret = ov05c10_read_reg(&pObj->Ctx, 0x10, &r10, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x11, &r11, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x12, &r12, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x13, &r13, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x14, &r14, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x15, &r15, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x19, &r19, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x1A, &r1a, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x1B, &r1b, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x1C, &r1c, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x1D, &r1d, 1);
	if (ret != OV05C10_OK) goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x1E, &r1e, 1);
	if (ret != OV05C10_OK) goto exit_pclk;

	/* Follow manufacturer script equations. */
	uint8_t mpll_byp_sel = r19 & 0x01;
	double mpllloopdiv_by2_sel = ((r15 & 0x04) ? 1.0 : 0.0) + 1.0;
	double mpll_nc_high = (double) (r1a & 0x30) / 256.0;
	double mpll_nc_low = (double) r1b;
	uint8_t mpll_mc_sel = r1a & 0x07;
	static const double mc_lut[8] = { 1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 6.0, 8.0 };
	mpll_mc = mc_lut[mpll_mc_sel];
	double mpll_predivp_sel = ((r1c & 0x80) ? 1.0 : 0.0) + 1.0;

	double mpll_clk_sel;
	if (mpll_byp_sel != 0U) {
		mpll_clk_sel = osc_clk;
	} else {
		mpll_clk_sel = iclk * (mpllloopdiv_by2_sel * (mpll_nc_high * 256.0 + mpll_nc_low))
				/ mpll_predivp_sel / mpll_mc;
	}

	uint8_t dpll_byp_sel = r10 & 0x01;
	double dpll_nc_high = (double) (r13 & 0x30) / 256.0;
	double dpll_nc_low = (double) r14;
	uint8_t dpll_mc_sel = r13 & 0x07;
	dpll_mc = mc_lut[dpll_mc_sel];
	double dpll_vcoby2_sel = ((r15 & 0x02) ? 1.0 : 0.0) + 1.0;
	double dpll_out = iclk
			* (((1.0 - (double) dpll_byp_sel)
					* (dpll_nc_high * 256.0 + dpll_nc_low) * dpll_vcoby2_sel)
					+ (double) dpll_byp_sel)
			/ dpll_mc / dpll_vcoby2_sel;

	uint8_t rowclk_ctrl = r1e & 0x03;
	rowclk = (double) (1U << rowclk_ctrl);

	uint8_t dpll_row_clkpost_sel = (r11 & 0x60) >> 5;
	double dpll_cnt_clk_sel = (double) ((r10 & 0x18) >> 3) + 1.0;
	double dpll_dacclk_400m = dpll_out / dpll_cnt_clk_sel;
	double dpll_divrow_sel = (double) (r12 & 0x03) + 5.0;
	double dpll_rowclk_60m;
	if (dpll_row_clkpost_sel < 2U) {
		dpll_rowclk_60m = iclk / dpll_cnt_clk_sel;
	} else {
		dpll_rowclk_60m = dpll_dacclk_400m / dpll_divrow_sel
				/ ((double) (dpll_row_clkpost_sel - 2U) * 2.0 + 2.0) / dpll_cnt_clk_sel;
	}

	uint8_t rowclk_sel = (r1d & 0x02) >> 1;
	double mpll_pclk_sel_pre = (double) (r1c & 0x03) + 1.0;
	double mpll_pclk_sel = (double) ((r1c & 0x0C) >> 2) + 3.0;
	double dpll_pclkdiv_sel = (double) (r11 & 0x03) + 3.0;

	double mpll_pclk_190m = mpll_clk_sel / mpll_pclk_sel_pre / mpll_pclk_sel / 2.0;
	double dpll_clk_220m = dpll_out / dpll_pclkdiv_sel;

	uint8_t pllclk_sel = r1d & 0x01;
	double pll_clk = (pllclk_sel == 0U) ? mpll_pclk_190m : dpll_clk_220m;
	double pll_row_clk = (rowclk_sel != 0U) ? dpll_rowclk_60m : pll_clk;
	double row_clk = pll_row_clk / rowclk;

	*pclk = (uint64_t) (row_clk * 1000000.0);
	ret = OV05C10_OK;

exit_pclk:
	return ret;
}

static int32_t OV05C10_GetExposureRange(OV05C10_Object_t *pObj, uint32_t *min_us,
		uint32_t *max_us) {
	int32_t ret = OV05C10_OK;

	uint64_t pclk = pObj->Pclk;
	uint8_t hts_raw[2] = { 0U };
	uint8_t vts_raw[3] = { 0U };
	uint32_t vts = 0U;
	uint16_t hts = 0U;
	if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	// read HTS, to determine line time
	if (ov05c10_read_reg(&pObj->Ctx, 0x37, hts_raw, 2) != 0) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	if (ov05c10_read_reg(&pObj->Ctx, 0x34, vts_raw, 3) != 0) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	hts = ((uint16_t) hts_raw[0] << 8) | hts_raw[1];
	vts = ((uint32_t) vts_raw[0] << 16) | ((uint32_t) vts_raw[1] << 8) | vts_raw[2];
	if (vts == 0U) {
		vts = OV05C10_EXPOSURE_MAX_VTS;
	}
	if (vts <= OV05C10_EXPOSURE_MAX_LINES_MARGIN) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	uint32_t line_us = (hts * 1000000) / pclk;
	*min_us = OV05C10_EXPOSURE_MIN_LINES * line_us;
	*max_us = (vts - OV05C10_EXPOSURE_MAX_LINES_MARGIN) * line_us;
	exit_exp_range: return ret;
}

int32_t OV05C10_SetExposure(OV05C10_Object_t *pObj,
                            int32_t exposure_us)
{
    uint8_t hts_raw[2];
    uint8_t vts_raw[3];
    uint8_t exposure_raw[3];
    uint8_t trigger = 0x01;

    uint16_t hts;
    uint32_t vts;
    uint32_t min_lines;
    uint32_t max_lines;
    uint32_t exposure_lines;

    uint64_t numerator;
    uint64_t denominator;

    if ((pObj == NULL) || (pObj->Pclk == 0U)) {
        return OV05C10_ERROR;
    }

    if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK) {
        return OV05C10_ERROR;
    }

    /*
     * P1:0x37[4:0], P1:0x38[7:0]
     * HTS is expressed in the clock domain represented by pObj->Pclk.
     */
    if (ov05c10_read_reg(&pObj->Ctx, 0x37,
                         hts_raw, sizeof(hts_raw)) != OV05C10_OK) {
        return OV05C10_ERROR;
    }

    /*
     * P1:0x34[5:0], P1:0x35[7:0], P1:0x36[7:0]
     */
    if (ov05c10_read_reg(&pObj->Ctx, 0x34,
                         vts_raw, sizeof(vts_raw)) != OV05C10_OK) {
        return OV05C10_ERROR;
    }

    hts = ((uint16_t)(hts_raw[0] & 0x1FU) << 8) |
          (uint16_t)hts_raw[1];

    vts = ((uint32_t)(vts_raw[0] & 0x3FU) << 16) |
          ((uint32_t)vts_raw[1] << 8) |
          (uint32_t)vts_raw[2];

    if ((hts == 0U) ||
        (vts <= OV05C10_EXPOSURE_MAX_LINES_MARGIN)) {
        return OV05C10_ERROR;
    }

    min_lines = OV05C10_EXPOSURE_MIN_LINES;
    max_lines = vts - OV05C10_EXPOSURE_MAX_LINES_MARGIN;

    if (max_lines < min_lines) {
        return OV05C10_ERROR;
    }

    if (exposure_us <= 0) {
        exposure_lines = min_lines;
    } else {
        /*
         * exposure_lines =
         *     exposure_us * timing_clock_hz /
         *     (HTS * 1,000,000)
         *
         * Add denominator / 2 for rounding to the nearest line.
         */
        numerator =
            (uint64_t)(uint32_t)exposure_us *
            (uint64_t)pObj->Pclk;

        denominator =
            (uint64_t)hts * 1000000ULL;

        exposure_lines =
            (uint32_t)((numerator + denominator / 2ULL) /
                       denominator);
    }

    if (exposure_lines < min_lines) {
        exposure_lines = min_lines;
    }

    if (exposure_lines > max_lines) {
        exposure_lines = max_lines;
    }

    /*
     * Exposure is a 23-bit value:
     *
     * P1:0x02 = exposure[22:16]
     * P1:0x03 = exposure[15:8]
     * P1:0x04 = exposure[7:0]
     */
    exposure_raw[0] =
        (uint8_t)((exposure_lines >> 16) & 0x7FU);
    exposure_raw[1] =
        (uint8_t)((exposure_lines >> 8) & 0xFFU);
    exposure_raw[2] =
        (uint8_t)(exposure_lines & 0xFFU);

    if (ov05c10_write_reg(&pObj->Ctx, 0x02,
                          exposure_raw,
                          sizeof(exposure_raw)) != OV05C10_OK) {
        return OV05C10_ERROR;
    }

    /*
     * Apply the buffered exposure setting.
     */
    if (ov05c10_write_reg(&pObj->Ctx, 0x01,
                          &trigger, 1) != OV05C10_OK) {
        return OV05C10_ERROR;
    }

    return OV05C10_OK;
}

static int32_t OV05C10_SelectPage(OV05C10_Object_t *pObj, uint8_t page) {
	if ((pObj->PageValid != 0U) && (pObj->CurrentPage == page)) {
		return OV05C10_OK;
	}

	if (ov05c10_write_reg(&pObj->Ctx, (uint8_t) OV05C10_REG_PAGE_SELECT, &page, 1)
			!= OV05C10_OK) {
		return OV05C10_ERROR;
	}

	pObj->CurrentPage = page;
	pObj->PageValid = 1U;
	return OV05C10_OK;
}

int32_t OV05C10_MirrorFlipConfig(OV05C10_Object_t *pObj, OV05C10_MirrorFlip_t Config) {
	int32_t ret = OV05C10_OK;
	uint8_t tmp;

	uint16_t shift_x = 0x0003, shift_y = 0x0003;

	switch (Config) {
	case OV05C10_FLIP:
		tmp = 0xa0;
		shift_x -= 1;  // to keep same Bayer pattern
		break;
	case OV05C10_MIRROR:
		tmp = 0xb8;
		shift_y -= 1;
		break;
	case OV05C10_MIRROR_FLIP:
		tmp = 0xb0;
		shift_x -= 1;
		shift_y -= 1;
		break;
	case OV05C10_MIRROR_FLIP_NONE:
	default:
		tmp = 0xa8;
		break;
	}
	if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	if (ov05c10_write_reg(&pObj->Ctx, 0x01, &tmp, 1) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	if (OV05C10_SelectPage(pObj, 0x07) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	shift_x = SWAP_ENDIAN16(shift_x);
	if (ov05c10_write_reg(&pObj->Ctx, 0x00,
			(uint8_t*) &shift_x, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	shift_y = SWAP_ENDIAN16(shift_y);
	if (ov05c10_write_reg(&pObj->Ctx, 0x01,
			(uint8_t*) &shift_y, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	exit_mirrorflip: return ret;
}
