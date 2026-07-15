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
	uint16_t addr;
	uint8_t val;
};

/*
 * OVD sequence is banked through 0xFD (8-bit register map):
 * full register address is (page << 8) | offset.
 */
#define OV05C10_REG_PAGE_SELECT 0x00FD

static const struct regval ov05c10r1a_ae02_init_2888x1808_2lane_raw10_60fps[] = {
	/* reset and base bring-up */
	{0x00FD, 0x00},
	{0x0020, 0x00},
	{0x00FD, 0x00},
	{0x0020, 0x0b},
	{0x00C1, 0x09},
	{0x0021, 0x06},
	{0x0014, 0x78},
	{0x00E7, 0x03},
	{0x00E7, 0x00},
	{0x0021, 0x00},
	{0x0014, 0x96},
	{0x001B, 0xBB},

	/* page 0x01 */
	{0x00FD, 0x01},
	{0x0103, 0x00},
	{0x0104, 0x06},
	{0x0107, 0x08},
	{0x011B, 0x01},
	{0x0124, 0xFF},
	{0x0142, 0x5D},
	{0x0143, 0x08},
	{0x0144, 0x81},
	{0x0146, 0x5F},
	{0x0148, 0x18},
	{0x0149, 0x04},
	{0x015C, 0x18},
	{0x015E, 0x13},
	{0x0170, 0x15},
	{0x0177, 0x35},
	{0x0179, 0x00},
	{0x017B, 0x08},
	{0x017D, 0x08},
	{0x017E, 0x08},
	{0x017F, 0x08},
	{0x0190, 0x37},
	{0x0191, 0x05},
	{0x0192, 0x18},
	{0x0193, 0x27},
	{0x0194, 0x05},
	{0x0195, 0x38},
	{0x019B, 0x00},
	{0x019C, 0x06},
	{0x019D, 0x28},
	{0x019E, 0x06},
	{0x01B2, 0x0D},
	{0x01B3, 0x29},
	{0x01BF, 0x3C},
	{0x01C2, 0x04},
	{0x01C4, 0x00},
	{0x01CA, 0x20},
	{0x01CB, 0x20},
	{0x01CC, 0x28},
	{0x01CD, 0x28},
	{0x01CE, 0x20},
	{0x01CF, 0x20},
	{0x01D0, 0x2A},
	{0x01D1, 0x2A},

	/* page 0x0F */
	{0x00FD, 0x0F},
	{0x0F00, 0x00},
	{0x0F01, 0xA0},
	{0x0F02, 0x48},
	{0x0F07, 0x8E},
	{0x0F08, 0x70},
	{0x0F09, 0x01},
	{0x0F0B, 0x40},
	{0x0F0D, 0x07},
	{0x0F11, 0x33},
	{0x0F12, 0x77},
	{0x0F13, 0x66},
	{0x0F14, 0x65},
	{0x0F15, 0x37},
	{0x0F16, 0xBF},
	{0x0F17, 0xFF},
	{0x0F18, 0xFF},
	{0x0F19, 0x12},
	{0x0F1A, 0x10},
	{0x0F1C, 0x77},
	{0x0F1D, 0x77},
	{0x0F20, 0x0F},
	{0x0F21, 0x0F},
	{0x0F22, 0x0F},
	{0x0F23, 0x0F},
	{0x0F2B, 0x20},
	{0x0F2C, 0x20},
	{0x0F2D, 0x04},

	/* page 0x03 */
	{0x00FD, 0x03},
	{0x039D, 0x0F},
	{0x039F, 0x40},
	{0x00FD, 0x00},
	{0x0020, 0x1B},

	/* page 0x04 */
	{0x00FD, 0x04},
	{0x0419, 0x60},

	/* page 0x02 */
	{0x00FD, 0x02},
	{0x0275, 0x05},
	{0x027F, 0x06},
	{0x029A, 0x03},
	{0x02A2, 0x07},
	{0x02A3, 0x10},
	{0x02A5, 0x02},
	{0x02A6, 0x0B},
	{0x02A7, 0x48},

	/* page 0x07 */
	{0x00FD, 0x07},
	{0x0742, 0x00},
	{0x0743, 0x80},
	{0x0744, 0x00},
	{0x0745, 0x80},
	{0x0746, 0x00},
	{0x0747, 0x80},
	{0x0748, 0x00},
	{0x0749, 0x80},
	{0x0700, 0xF7},

	/* finalize on page 0x00 */
	{0x00FD, 0x00},
	{0x00E7, 0x03},
	{0x00E7, 0x00},
	{0x00FD, 0x00},
	{0x008E, 0x0B},
	{0x008F, 0x48},
	{0x0093, 0x18},
	{0x0094, 0xFF},
	{0x0095, 0xBD},
	{0x0096, 0x1A},
	{0x0098, 0x04},
	{0x0099, 0x08},
	{0x009B, 0x10},
	{0x009C, 0x3F},
	{0x00A1, 0x05},
	{0x00A4, 0x2F},
	{0x00C0, 0x0C},
	{0x00C1, 0x08},
	{0x00C2, 0x00},
	{0x00B6, 0x20},
	{0x00BB, 0x80},
	{0x00FD, 0x00},
	{0x00A0, 0x00},
	{0x00FD, 0x01},
};


static int32_t OV05C10_WriteTable(OV05C10_Object_t *pObj, const struct regval *regs,
		uint32_t size);
static int32_t OV05C10_StreamControl(OV05C10_Object_t *pObj, uint8_t enable);
static int32_t OV05C10_SelectPage(OV05C10_Object_t *pObj, uint8_t page);
static int32_t OV05C10_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV05C10_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
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
			if (ov05c10_write_reg(&pObj->Ctx, regs[index].addr,
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

static int32_t OV05C10_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV05C10_Object_t *pObj = (OV05C10_Object_t*) handle;

	return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

static int32_t OV05C10_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
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
		switch (Resolution) {
		case OV05C10_RESOLUTION_1928_1082:
			if (OV05C10_WriteTable(pObj,
					ov05c10r1a_ae02_init_2888x1808_2lane_raw10_60fps,
					ARRAY_SIZE(
							ov05c10r1a_ae02_init_2888x1808_2lane_raw10_60fps)) != OV05C10_OK) {
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

	/* OVD stream control sequence */
	val = 0x01;
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &val, 1) != OV05C10_OK) {
		return OV05C10_ERROR;
	}

	if (enable) {
		val = 0x03;
		if (ov05c10_write_reg(&pObj->Ctx, 0x0133, &val, 1) != OV05C10_OK) {
			ret = OV05C10_ERROR;
		}
		val = 0x02;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x0101, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x00;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &val,
						1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x1F;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x0020, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
	} else {
		val = 0x00;
		if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &val, 1) != OV05C10_OK) {
			ret = OV05C10_ERROR;
		}
		val = 0x5B;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x0020, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x01;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &val,
						1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x02;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x0133, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
		val = 0x02;
		if ((ret == OV05C10_OK)
				&& (ov05c10_write_reg(&pObj->Ctx, 0x0101, &val, 1) != OV05C10_OK)) {
			ret = OV05C10_ERROR;
		}
	}

	OV05C10_Delay(pObj, 20);
	return ret;
}

int32_t OV05C10_ReadID(OV05C10_Object_t *pObj, uint32_t *Id) {
	int32_t ret = OV05C10_OK;
	uint8_t tmp[4] = {0};
	uint8_t page;

	/* Initialize I2C */
	if ((pObj == NULL) || (Id == NULL) || (pObj->IO.Init == NULL)) {
		return OV05C10_ERROR;
	}
	pObj->IO.Init();

	page = 0x00;
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &page, 1) != OV05C10_OK) {
		return OV05C10_ERROR;
	}

	if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_2, &tmp[2], 1) != OV05C10_OK) {
		ret = OV05C10_ERROR;
	} else {
		if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_1, &tmp[1],
				1) != OV05C10_OK) {
			ret = OV05C10_ERROR;
		} else {
			if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_0, &tmp[0],
					1) != OV05C10_OK) {
				ret = OV05C10_ERROR;
			} else {
				ret = OV05C10_OK;
			}
		}

	}
	if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_ID_BYTE_3, &tmp[3], 1) != OV05C10_OK) {
		ret = OV05C10_ERROR;
	}

	if (!ret) {
		*Id = ((uint32_t)tmp[3] << 24) |
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
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_ANALOG_GAIN,
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
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_DIGITAL_GAIN,
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
	float PLL2_PreDiv0, PLL2_PreDiv = 1.0f;
	uint16_t PLL2_DivLoop;
	float PLL2_DivSys = 1.0f;
	uint8_t PLL2_DivSysPre, PLL2_Sa1Div, PLL2_DivDac, PLL2_SRAMDiv;
	float PLL2_VCO, PLL2_SCLK, PLL2_DACCLK, PLL2_SRAMCLK, PLL2_SA1CLK;
	static const uint8_t mclk = 24;  // MHz

	// Required registers
	uint8_t R314, R315, R316, R317, R318, R31a, R31c, R31d, R321;

	// Read registers
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0314, &R314, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0315, &R315, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0316, &R316, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0317, &R317, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0318, &R318, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x031a, &R31a, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x031c, &R31c, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x031d, &R31d, 1);
	if (ret != 0)
		goto exit_pclk;
	ret = ov05c10_read_reg(&pObj->Ctx, 0x0321, &R321, 1);
	if (ret != 0)
		goto exit_pclk;

	PLL2_PreDiv0 = (float) ((R31a & 0x1) + 1);
	switch (R314 & 0x7) {
	case 0:
		PLL2_PreDiv = 1.0f;
		break;
	case 1:
		PLL2_PreDiv = 1.5f;
		break;
	case 2:
		PLL2_PreDiv = 2.0f;
		break;
	case 3:
		PLL2_PreDiv = 2.5f;
		break;
	case 4:
		PLL2_PreDiv = 3.0f;
		break;
	case 5:
		PLL2_PreDiv = 4.0f;
		break;
	case 6:
		PLL2_PreDiv = 6.0f;
		break;
	case 7:
		PLL2_PreDiv = 8.0f;
		break;
	}

	PLL2_DivLoop = ((R315 & 0x3) << 8) | R316;

	switch (R317 & 0xf) {
	case 0:
		PLL2_DivSys = 1.0f;
		break;
	case 1:
		PLL2_DivSys = 1.5f;
		break;
	case 2:
		PLL2_DivSys = 2.0f;
		break;
	case 3:
		PLL2_DivSys = 2.5f;
		break;
	case 4:
		PLL2_DivSys = 3.0f;
		break;
	case 5:
		PLL2_DivSys = 3.5f;
		break;
	case 6:
		PLL2_DivSys = 4.0f;
		break;
	case 7:
		PLL2_DivSys = 5.0f;
		break;
	}

	PLL2_DivSysPre = (R318 & 0xf) + 1;
	PLL2_Sa1Div = (R31c & 0xf) + 1;
	PLL2_DivDac = (R31d & 0x1f) + 1;
	PLL2_SRAMDiv = (R321 & 0x7) + 1;

	PLL2_VCO = mclk / PLL2_PreDiv0 / PLL2_PreDiv * PLL2_DivLoop;
	PLL2_SCLK = PLL2_VCO / PLL2_DivSys / PLL2_DivSysPre;
	PLL2_DACCLK = PLL2_VCO / PLL2_DivDac;
	PLL2_SRAMCLK = PLL2_VCO / PLL2_SRAMDiv;
	PLL2_SA1CLK = PLL2_VCO / PLL2_Sa1Div;

	(void)PLL2_SRAMCLK;
	(void)PLL2_DACCLK;
	(void)PLL2_SCLK;

	// Output SA1 clock as PCLK, in Hz
	*pclk = ((uint64_t) PLL2_SA1CLK * 1000000);
	ret = OV05C10_OK;

	exit_pclk: return ret;
}

static int32_t OV05C10_GetExposureRange(OV05C10_Object_t *pObj, uint32_t *min_us,
		uint32_t *max_us) {
	int32_t ret = OV05C10_OK;

	uint64_t pclk = pObj->Pclk;
	uint16_t hts = 0;
	if (OV05C10_SelectPage(pObj, 0x0F) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	// read HTS, to determine line time
	if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_HTS, (uint8_t*) &hts, 2) != 0) {
		ret = OV05C10_ERROR;
		goto exit_exp_range;
	}
	hts = SWAP_ENDIAN16(hts);
	uint32_t line_us = (hts * 1000000) / pclk;
	*min_us = OV05C10_EXPOSURE_MIN_LINES * line_us;
	*max_us = (OV05C10_EXPOSURE_MAX_VTS - OV05C10_EXPOSURE_MAX_LINES_MARGIN) * line_us;
	exit_exp_range: return ret;
}

int32_t OV05C10_SetExposure(OV05C10_Object_t *pObj, int32_t exposure_us) {
	int32_t ret = OV05C10_OK;

	// Exposure calculations
	// line time (t_line) = Horizontal Total Size (HTS) / Pixel Clock (PCLK)
	// Vertical Total Size (VTS) = Exposure (t_exposure) / t_line = Exposure / (HTS / PCLK)
	// PCLK = FPS * HTS * VTS
	uint16_t hts = 0, vts = 0;
	if (OV05C10_SelectPage(pObj, 0x0F) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp;
	}

	// read HTS
	if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_HTS, (uint8_t*) &hts, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp;
	}
	hts = SWAP_ENDIAN16(hts);

	if (hts == 0 || pObj->Pclk == 0) {
        ret = OV05C10_ERROR; // Avoid division by zero
        goto exit_exp;
    }

	/* Calculate number of lines (rounded) */
	uint32_t line_us = (hts * 1000000) / pObj->Pclk;
	if (line_us == 0) {
        ret = OV05C10_ERROR; // Avoid division by zero
        goto exit_exp;
    }
	uint32_t lines = exposure_us / line_us;

	/* Clamp to sensor limits */
	if (lines < OV05C10_EXPOSURE_MIN_LINES) {
		lines = OV05C10_EXPOSURE_MIN_LINES;
	}

	/* Calculate new VTS value */
	uint32_t vts_new = lines + OV05C10_EXPOSURE_MAX_LINES_MARGIN;
	if(vts_new < OV05C10_EXPOSURE_MIN_VTS) {
		vts_new = OV05C10_EXPOSURE_MIN_VTS;
	}
	if(vts_new > OV05C10_EXPOSURE_MAX_VTS) {
		vts_new = OV05C10_EXPOSURE_MAX_VTS;
	}

	/* read current VTS and update with new if different */
	if (ov05c10_read_reg(&pObj->Ctx, OV05C10_REG_VTS, (uint8_t*) &vts, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp;
	}
	vts = SWAP_ENDIAN16(vts);

	if(vts_new != vts) {
		uint16_t vts_reg = SWAP_ENDIAN16((uint16_t )vts_new);
		if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_VTS, (uint8_t*) &vts_reg, 2) != OV05C10_OK) {
			ret = OV05C10_ERROR;
			goto exit_exp;
		}
	}

	/* reclamp lines */
	if(lines + OV05C10_EXPOSURE_MAX_LINES_MARGIN > vts_new) {
		lines = vts_new - OV05C10_EXPOSURE_MAX_LINES_MARGIN;
	}

	if (OV05C10_SelectPage(pObj, 0x01) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_exp;
	}

	/* Write exposure lines (16-bit: 0x3501 = MSB, 0x3502 = LSB) */
	uint16_t exp_val = SWAP_ENDIAN16((uint16_t )lines);
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_EXPOSURE, (uint8_t*) &exp_val, 2)
			!= 0) {
		ret = OV05C10_ERROR;
		goto exit_exp;
	}
exit_exp:
	return ret;
}

static int32_t OV05C10_SelectPage(OV05C10_Object_t *pObj, uint8_t page) {
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_PAGE_SELECT, &page, 1)
			!= OV05C10_OK) {
		return OV05C10_ERROR;
	}
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
	if (ov05c10_write_reg(&pObj->Ctx, OV05C10_REG_FORMAT, &tmp, 1) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	shift_x = SWAP_ENDIAN16(shift_x);
	if (ov05c10_write_reg(&pObj->Ctx, OV05C1010_REG_ISP_X_WIN_CONTROL,
			(uint8_t*) &shift_x, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	shift_y = SWAP_ENDIAN16(shift_y);
	if (ov05c10_write_reg(&pObj->Ctx, OV05C1010_REG_ISP_Y_WIN_CONTROL,
			(uint8_t*) &shift_y, 2) != OV05C10_OK) {
		ret = OV05C10_ERROR;
		goto exit_mirrorflip;
	}
	exit_mirrorflip: return ret;
}
