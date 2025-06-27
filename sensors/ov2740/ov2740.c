#include "ov2740.h"
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

OV2740_CAMERA_Drv_t OV2740_CAMERA_Driver = {
		.Init = OV2740_Init,
		.DeInit = OV2740_DeInit,
		.ReadID = OV2740_ReadID,
		.GetCapabilities = OV2740_GetCapabilities,
		.SetGain = OV2740_SetGain,
		.SetExposure = OV2740_SetExposure,
		.SetFrequency = OV2740_SetFrequency,
		.MirrorFlipConfig = OV2740_MirrorFlipConfig,
		.GetSensorInfo = OV2740_GetSensorInfo,
};

struct regval {
	uint16_t addr;
	uint8_t val;
};

static const struct regval ov2740_MIPI_2lane_raw10_1932x1092_360mhz[] = {
	{0x3000, 0x00},
	{0x3018, 0x32},
	{0x3031, 0x0a},
	{0x3080, 0x08},
	{0x3083, 0xB4},
	{0x3103, 0x00},
	{0x3104, 0x01},
	{0x3106, 0x01},
	{0x3500, 0x00},
	{0x3501, 0x44},
	{0x3502, 0x40},
	{0x3503, 0x88},
	{0x3507, 0x00},
	{0x3508, 0x00},
	{0x3509, 0x80},
	{0x350c, 0x00},
	{0x350d, 0x80},
	{0x3510, 0x00},
	{0x3511, 0x00},
	{0x3512, 0x20},
	{0x3632, 0x00},
	{0x3633, 0x10},
	{0x3634, 0x10},
	{0x3635, 0x10},
	{0x3645, 0x13},
	{0x3646, 0x81},
	{0x3636, 0x10},
	{0x3651, 0x0a},
	{0x3656, 0x02},
	{0x3659, 0x04},
	{0x365a, 0xda},
	{0x365b, 0xa2},
	{0x365c, 0x04},
	{0x365d, 0x1d},
	{0x365e, 0x1a},
	{0x3662, 0xd7},
	{0x3667, 0x78},
	{0x3669, 0x0a},
	{0x366a, 0x92},
	{0x3700, 0x54},
	{0x3702, 0x10},
	{0x3706, 0x42},
	{0x3709, 0x30},
	{0x370b, 0xc2},
	{0x3714, 0x63},
	{0x3715, 0x01},
	{0x3716, 0x00},
	{0x371a, 0x3e},
	{0x3732, 0x0e},
	{0x3733, 0x10},
	{0x375f, 0x0e},
	{0x3768, 0x30},
	{0x3769, 0x44},
	{0x376a, 0x22},
	{0x377b, 0x20},
	{0x377c, 0x00},
	{0x377d, 0x0c},
	{0x3798, 0x00},
	{0x37a1, 0x55},
	{0x37a8, 0x6d},
	{0x37c2, 0x04},
	{0x37c5, 0x00},
	{0x37c8, 0x00},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x00},
	{0x3804, 0x07},
	{0x3805, 0x8f},
	{0x3806, 0x04},
	{0x3807, 0x47},
	{0x3808, 0x07},
	{0x3809, 0x88},
	{0x380a, 0x04},
	{0x380b, 0x40},
	{0x380c, 0x04},
	{0x380d, 0x38},
	{0x380e, 0x06},
	{0x380f, 0xf0},
	{0x3810, 0x00},
	{0x3811, 0x04},
	{0x3812, 0x00},
	{0x3813, 0x04},
	{0x3814, 0x01},
	{0x3815, 0x01},
	{0x3820, 0x80},
	{0x3821, 0x46},
	{0x3822, 0x84},
	{0x3829, 0x00},
	{0x382a, 0x01},
	{0x382b, 0x01},
	{0x3830, 0x04},
	{0x3836, 0x01},
	{0x3837, 0x08},
	{0x3839, 0x01},
	{0x383a, 0x00},
	{0x383b, 0x08},
	{0x383c, 0x00},
	{0x3f0b, 0x00},
	{0x4001, 0x20},
	{0x4009, 0x07},
	{0x4003, 0x10},
	{0x4010, 0xe0},
	{0x4016, 0x00},
	{0x4017, 0x10},
	{0x4044, 0x02},
	{0x4304, 0x08},
	{0x4307, 0x30},
	{0x4320, 0x80},
	{0x4322, 0x00},
	{0x4323, 0x00},
	{0x4324, 0x00},
	{0x4325, 0x00},
	{0x4326, 0x00},
	{0x4327, 0x00},
	{0x4328, 0x00},
	{0x4329, 0x00},
	{0x432c, 0x03},
	{0x432d, 0x81},
	{0x4501, 0x84},
	{0x4502, 0x40},
	{0x4503, 0x18},
	{0x4504, 0x04},
	{0x4508, 0x02},
	{0x4601, 0x10},
	{0x4800, 0x00},
	{0x4816, 0x52},
	{0x4837, 0x16},
	{0x5000, 0x7f},
	{0x5001, 0x00},
	{0x5005, 0x38},
	{0x501e, 0x0d},
	{0x5040, 0x00},
	{0x5901, 0x00},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x00},
	{0x3804, 0x07},
	{0x3805, 0x8f},
	{0x3806, 0x04},
	{0x3807, 0x47},
	{0x3808, 0x07},
	{0x3809, 0x8c},
	{0x380a, 0x04},
	{0x380b, 0x44},
	{0x3810, 0x00},
	{0x3811, 0x00},
	{0x3812, 0x00},
	{0x3813, 0x01},
};

static int32_t OV2740_WriteTable(OV2740_Object_t *pObj, const struct regval *regs,
		uint32_t size);
static int32_t OV2740_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV2740_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV2740_Delay(OV2740_Object_t *pObj, uint32_t Delay);

static int32_t OV2740_SetAnalogGain(OV2740_Object_t *pObj, float gain_dBm);
static int32_t OV2740_SetDigitalGain(OV2740_Object_t *pObj, float gain_dBm);

static int32_t OV2740_GetPCLK(OV2740_Object_t *pObj, uint64_t *pclk);
static int32_t OV2740_GetExposureRange(OV2740_Object_t *pObj, uint32_t *min_us,
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

static int32_t OV2740_WriteTable(OV2740_Object_t *pObj, const struct regval *regs,
		uint32_t size) {
	uint32_t index;
	int32_t ret = OV2740_OK;

	/* Set registers */
	for (index = 0; index < size; index++) {
		if (ret != OV2740_ERROR) {
			if (ov2740_write_reg(&pObj->Ctx, regs[index].addr,
					(uint8_t*) &(regs[index].val), 1) != OV2740_OK) {
				ret = OV2740_ERROR;
			}
		}
	}
	return ret;
}

static int32_t OV2740_Delay(OV2740_Object_t *pObj, uint32_t Delay) {
	uint32_t tickstart;
	tickstart = pObj->IO.GetTick();
	while ((pObj->IO.GetTick() - tickstart) < Delay) {
	}
	return OV2740_OK;
}

static int32_t OV2740_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV2740_Object_t *pObj = (OV2740_Object_t*) handle;

	return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

static int32_t OV2740_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV2740_Object_t *pObj = (OV2740_Object_t*) handle;

	return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

int32_t OV2740_RegisterBusIO(OV2740_Object_t *pObj, OV2740_IO_t *pIO) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV2740_ERROR;
	} else {
		pObj->IO.Init = pIO->Init;
		pObj->IO.DeInit = pIO->DeInit;
		pObj->IO.Address = pIO->Address;
		pObj->IO.WriteReg = pIO->WriteReg;
		pObj->IO.ReadReg = pIO->ReadReg;
		pObj->IO.GetTick = pIO->GetTick;

		pObj->Ctx.ReadReg = OV2740_ReadRegWrap;
		pObj->Ctx.WriteReg = OV2740_WriteRegWrap;
		pObj->Ctx.handle = pObj;

		if (pObj->IO.Init != NULL) {
			ret = pObj->IO.Init();
		} else {
			ret = OV2740_ERROR;
		}
	}

	return ret;
}

int32_t OV2740_Init(OV2740_Object_t *pObj, uint32_t Resolution,
		uint32_t PixelFormat) {
	int32_t ret = OV2740_OK;

	if (pObj->IsInitialized == 0U) {
		switch (Resolution) {
		case OV2740_RESOLUTION_1932_1092:
			if (OV2740_WriteTable(pObj,
					ov2740_MIPI_2lane_raw10_1932x1092_360mhz,
					ARRAY_SIZE(
							ov2740_MIPI_2lane_raw10_1932x1092_360mhz)) != OV2740_OK) {
				ret = OV2740_ERROR;
			}
			break;
			/* Add new resolution here */
		default:
			/* Resolution not supported */
			ret = OV2740_ERROR;
		}

		if (!ret) {
			pObj->IsInitialized = 1U;
		}
	}

	// get PCLK for exposure control
	ret = OV2740_GetPCLK(pObj, &pObj->Pclk);

	return ret;
}

int32_t OV2740_DeInit(OV2740_Object_t *pObj) {
	if (pObj->IsInitialized == 1U) {
		/* De-initialize camera sensor interface */
		pObj->IsInitialized = 0U;
	}

	return OV2740_OK;
}

int32_t OV2740_Start(OV2740_Object_t *pObj) {
	int ret;
	uint8_t tmp;
	/* Start streaming */
	tmp = OV2740_MODE_STREAMING;
	if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_MODE_SELECT, &tmp, 1) != OV2740_OK) {
		ret = OV2740_ERROR;
	} else {
		OV2740_Delay(pObj, 20);
	}
	return ret;
}

int32_t OV2740_ReadID(OV2740_Object_t *pObj, uint32_t *Id) {
	int32_t ret = OV2740_OK;
	uint8_t tmp[3];

	/* Initialize I2C */
	pObj->IO.Init();

	if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_ID_BYTE_2, &tmp[2], 1) != OV2740_OK) {
		ret = OV2740_ERROR;
	} else {
		if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_ID_BYTE_1, &tmp[1],
				1) != OV2740_OK) {
			ret = OV2740_ERROR;
		} else {
			if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_ID_BYTE_0, &tmp[0],
					1) != OV2740_OK) {
				ret = OV2740_ERROR;
			} else {
				ret = OV2740_OK;
			}
		}

	}

	if (!ret) {
		*Id = (tmp[2] << 16) | (tmp[1] << 8) | (tmp[0] << 0);
	}

	return ret;
}

int32_t OV2740_GetCapabilities(OV2740_Object_t *pObj,
		OV2740_Capabilities_t *Capabilities) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV2740_ERROR;
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

		ret = OV2740_OK;
	}

	return ret;
}

int32_t OV2740_GetSensorInfo(OV2740_Object_t *pObj, OV2740_SensorInfo_t *Info) {
	if ((!pObj) || (Info == NULL)) {
		return OV2740_ERROR;
	}

	if (sizeof(Info->name) >= strlen(OV2740_NAME) + 1) {
		strcpy(Info->name, OV2740_NAME);
	} else {
		return OV2740_ERROR;
	}

	Info->bayer_pattern = OV2740_BAYER_PATTERN;
	Info->color_depth = OV2740_COLOR_DEPTH;
	Info->width = OV2740_WIDTH;
	Info->height = OV2740_HEIGHT;
	Info->gain_min = OV2740_GAIN_MIN;
	Info->gain_max = OV2740_GAIN_MAX;
	if (OV2740_GetExposureRange(pObj, &Info->exposure_min,
			&Info->exposure_max) != OV2740_OK) {
		// defaults
		Info->exposure_min = OV2740_EXPOSURE_MIN;
		Info->exposure_max = OV2740_EXPOSURE_MAX;
	}
	return OV2740_OK;
}

static int32_t OV2740_SetAnalogGain(OV2740_Object_t *pObj, float gain_dBm) {
	int ret = OV2740_OK;
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV2740_ANALOG_GAIN_MAX_DBM) {
		gain_dBm = OV2740_ANALOG_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV2740_ANALOG_GAIN_MAX_LINEAR) {
		linear_gain = OV2740_ANALOG_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint16_t reg_value = (uint16_t) map_range_to_uint32(linear_gain, 1.0f,
			OV2740_ANALOG_GAIN_MAX_LINEAR, OV2740_ANALOG_GAIN_MIN_REG,
			OV2740_ANALOG_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN16(reg_value);
	if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_ANALOG_GAIN,
			(uint8_t*) &reg_value, 2) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_gain;
	}
	exit_gain: return ret;
}

static int32_t OV2740_SetDigitalGain(OV2740_Object_t *pObj, float gain_dBm) {
	int ret = OV2740_OK;
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV2740_DIGITAL_GAIN_MAX_DBM) {
		gain_dBm = OV2740_DIGITAL_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV2740_DIGITAL_GAIN_MAX_LINEAR) {
		linear_gain = OV2740_DIGITAL_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint32_t reg_value = map_range_to_uint32(linear_gain, 1.0f,
			OV2740_DIGITAL_GAIN_MAX_LINEAR, OV2740_DIGITAL_GAIN_MIN_REG,
			OV2740_DIGITAL_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN32(reg_value);
//	if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_DIGITAL_GAIN,
//			((uint8_t*) &reg_value) + 1, 3) != OV2740_OK) {
//		ret = OV2740_ERROR;
//		goto exit_gain;
//	}
	exit_gain: return ret;
}

int32_t OV2740_SetGain(OV2740_Object_t *pObj, int32_t gain_dBm) {
	int32_t ret = 0;

	if (gain_dBm < OV2740_GAIN_MIN) {
		gain_dBm = OV2740_GAIN_MIN;
	} else if (gain_dBm > OV2740_GAIN_MAX) {
		gain_dBm = OV2740_GAIN_MAX;
	}

	float analog_gain_dBm =
			(gain_dBm > OV2740_ANALOG_GAIN_MAX_DBM) ?
					OV2740_ANALOG_GAIN_MAX_DBM : gain_dBm;
	float digital_gain_dBm = gain_dBm - analog_gain_dBm;
	if (OV2740_SetAnalogGain(pObj, analog_gain_dBm) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_gain;
	}
	if (OV2740_SetDigitalGain(pObj, digital_gain_dBm) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_gain;
	}

	exit_gain: return ret;
}

static int32_t OV2740_GetPCLK(OV2740_Object_t *pObj, uint64_t *pclk)
{
    *pclk = 720000000U * 2 * 2 / 10;  // link freq * 2 (DDR) * 2 (lanes) / 10 (RAW10)
    return 0;
}

static int32_t OV2740_GetExposureRange(OV2740_Object_t *pObj, uint32_t *min_us,
		uint32_t *max_us) {
	int32_t ret = OV2740_OK;

	uint64_t pclk = pObj->Pclk;
	uint16_t hts = 0, vts = 0;
	// read HTS and VTS
	if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_HTS, (uint8_t*) &hts, 2) != 0) {
		ret = OV2740_ERROR;
		goto exit_exp_range;
	}
	hts = SWAP_ENDIAN16(hts);
	if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_VTS, (uint8_t*) &vts, 2) != 0) {
		ret = OV2740_ERROR;
		goto exit_exp_range;
	}
	vts = SWAP_ENDIAN16(vts);
	uint32_t line_us = (hts * 1000000) / pclk;
	*min_us = OV2740_EXPOSURE_MIN_LINES * line_us;
	*max_us = (vts - 15) * line_us;
	exit_exp_range: return ret;
}

int32_t OV2740_SetExposure(OV2740_Object_t *pObj, int32_t exposure_us) {
	int32_t ret = OV2740_OK;

	// Exposure calculations
	// line time (t_line) = Horizontal Total Size (HTS) / Pixel Clock (PCLK)
	// Vertical Total Size (VTS) = Exposure (t_exposure) / t_line = Exposure / (HTS / PCLK)
	// PCLK = FPS * HTS * VTS
	uint16_t hts = 0, vts = 0;

	// read HTS and VTS
	if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_HTS, (uint8_t*) &hts,
			2) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_exp;
	}
	hts = SWAP_ENDIAN16(hts);
	if (ov2740_read_reg(&pObj->Ctx, OV2740_REG_VTS, (uint8_t*) &vts,
			2) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_exp;
	}
	vts = SWAP_ENDIAN16(vts);
	/* Calculate number of lines (rounded) */
	uint32_t line_us = (hts * 1000000) / pObj->Pclk;
	uint32_t lines = exposure_us / line_us;

	/* Clamp to sensor limits */
	if (lines < OV2740_EXPOSURE_MIN_LINES)
		lines = OV2740_EXPOSURE_MIN_LINES;
	if (lines > vts - OV2740_EXPOSURE_MAX_LINES_MARGIN)
		lines = vts - OV2740_EXPOSURE_MAX_LINES_MARGIN; /* 16-bit register limit */

	// TODO maybe add hold functionality

	/* Write exposure lines */
	lines = lines << 4;  // last 4 bits are fraction bits
	uint8_t exp_val[3] = {
		(lines >> 16) & 0xFF,
		(lines >> 8) & 0xFF,
		lines & 0xFF
	};
	for(int i = 0; i < 3; i++) {
		if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_EXPOSURE + i, &exp_val[i], 1)
				!= 0) {
			ret = OV2740_ERROR;
			goto exit_exp;
		}
	}
	exit_exp:
	// TODO: maybe add hold functionality if possible
	return ret;
}

int32_t OV2740_MirrorFlipConfig(OV2740_Object_t *pObj, OV2740_MirrorFlip_t Config) {
	int32_t ret = OV2740_OK;
	uint8_t fmt1 = 0, fmt2 = 0;

	switch (Config) {
	case OV2740_FLIP:
		fmt1 = 0x06;
		fmt2 = 0x46;
		break;
	case OV2740_MIRROR:
		fmt1 = 0x00;
		fmt2 = 0x00;
		break;
	case OV2740_MIRROR_FLIP:
		fmt1 = 0x06;
		fmt2 = 0x00;
		break;
	case OV2740_MIRROR_FLIP_NONE:
	default:
		fmt1 = 0x00;
		fmt2 = 0x46;
		break;
	}
	if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_FORMAT1, &fmt1, 1) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_mirrorflip;
	}
	if (ov2740_write_reg(&pObj->Ctx, OV2740_REG_FORMAT2, &fmt2, 1) != OV2740_OK) {
		ret = OV2740_ERROR;
		goto exit_mirrorflip;
	}
	exit_mirrorflip: return ret;
}
