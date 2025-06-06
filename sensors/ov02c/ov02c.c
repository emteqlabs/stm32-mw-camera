#include "ov02c.h"
#include <string.h>
#include <math.h>

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

OV02C_CAMERA_Drv_t OV02C_CAMERA_Driver = {
		.Init = OV02C_Init,
		.DeInit = OV02C_DeInit,
		.ReadID = OV02C_ReadID,
		.GetCapabilities = OV02C_GetCapabilities,
		.SetGain = OV02C_SetGain,
		.SetExposure = OV02C_SetExposure,
		.SetFrequency = OV02C_SetFrequency,
		.MirrorFlipConfig = OV02C_MirrorFlipConfig,
		.GetSensorInfo = OV02C_GetSensorInfo,
};

struct regval {
	uint16_t addr;
	uint8_t val;
};

static const struct regval ov02c10_input_24M_MIPI_2lane_raw10_1928x1082_30fps[] = {
	{0x0301, 0x08},
	{0x0303, 0x06},
	{0x0304, 0x01},
	{0x0305, 0xe0},
	{0x0313, 0x40},
	{0x031c, 0x4f},
	{0x301b, 0xf0},
	{0x3020, 0x97},
	{0x3022, 0x01},
	{0x3026, 0xb4},
	{0x3027, 0xf1},
	{0x303b, 0x00},
	{0x303c, 0x4f},
	{0x303d, 0xe6},
	{0x303e, 0x00},
	{0x303f, 0x03},
	{0x3021, 0x23},
	{0x3501, 0x04},
	{0x3502, 0x6c},
	{0x3504, 0x0c},
	{0x3507, 0x00},
	{0x3508, 0x08},
	{0x3509, 0x00},
	{0x350a, 0x01},
	{0x350b, 0x00},
	{0x350c, 0x41},
	{0x3600, 0x84},
	{0x3603, 0x08},
	{0x3610, 0x57},
	{0x3611, 0x1b},
	{0x3613, 0x78},
	{0x3623, 0x00},
	{0x3632, 0xa0},
	{0x3642, 0xe8},
	{0x364c, 0x70},
	{0x365f, 0x0f},
	{0x3708, 0x30},
	{0x3714, 0x24},
	{0x3725, 0x02},
	{0x3737, 0x08},
	{0x3739, 0x28},
	{0x3749, 0x32},
	{0x374a, 0x32},
	{0x374b, 0x32},
	{0x374c, 0x32},
	{0x374d, 0x81},
	{0x374e, 0x81},
	{0x374f, 0x81},
	{0x3752, 0x36},
	{0x3753, 0x36},
	{0x3754, 0x36},
	{0x3761, 0x00},
	{0x376c, 0x81},
	{0x3774, 0x18},
	{0x3776, 0x08},
	{0x377c, 0x81},
	{0x377d, 0x81},
	{0x377e, 0x81},
	{0x37a0, 0x44},
	{0x37a6, 0x44},
	{0x37aa, 0x0d},
	{0x37ae, 0x00},
	{0x37cb, 0x03},
	{0x37cc, 0x01},
	{0x37d8, 0x02},
	{0x37d9, 0x10},
	{0x37e1, 0x10},
	{0x37e2, 0x18},
	{0x37e3, 0x08},
	{0x37e4, 0x08},
	{0x37e5, 0x02},
	{0x37e6, 0x08},

	// 1928x1092
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
	{0x380b, 0x44},
	{0x380c, 0x04},
	{0x380d, 0x74},
	{0x380e, 0x09},
	{0x380f, 0x18},
	{0x3810, 0x00},
	{0x3811, 0x03},
	{0x3812, 0x00},
	{0x3813, 0x03},
	{0x3814, 0x01},
	{0x3815, 0x01},
	{0x3816, 0x01},
	{0x3817, 0x01},

	{0x3820, 0xa8},
	{0x3821, 0x00},
	{0x3822, 0x80},
	{0x3823, 0x08},
	{0x3824, 0x00},
	{0x3825, 0x20},
	{0x3826, 0x00},
	{0x3827, 0x08},
	{0x382a, 0x00},
	{0x382b, 0x08},
	{0x382d, 0x00},
	{0x382e, 0x00},
	{0x382f, 0x23},
	{0x3834, 0x00},
	{0x3839, 0x00},
	{0x383a, 0xd1},
	{0x383e, 0x03},
	{0x393d, 0x29},
	{0x393f, 0x6e},
	{0x394b, 0x06},
	{0x394c, 0x06},
	{0x394d, 0x08},
	{0x394e, 0x0a},
	{0x394f, 0x01},
	{0x3950, 0x01},
	{0x3951, 0x01},
	{0x3952, 0x01},
	{0x3953, 0x01},
	{0x3954, 0x01},
	{0x3955, 0x01},
	{0x3956, 0x01},
	{0x3957, 0x0e},
	{0x3958, 0x08},
	{0x3959, 0x08},
	{0x395a, 0x08},
	{0x395b, 0x13},
	{0x395c, 0x09},
	{0x395d, 0x05},
	{0x395e, 0x02},
	{0x395f, 0x00},
	{0x395f, 0x00},
	{0x3960, 0x00},
	{0x3961, 0x00},
	{0x3962, 0x00},
	{0x3963, 0x00},
	{0x3964, 0x00},
	{0x3965, 0x00},
	{0x3966, 0x00},
	{0x3967, 0x00},
	{0x3968, 0x01},
	{0x3969, 0x01},
	{0x396a, 0x01},
	{0x396b, 0x01},
	{0x396c, 0x10},
	{0x396d, 0xf0},
	{0x396e, 0x11},
	{0x396f, 0x00},
	{0x3970, 0x37},
	{0x3971, 0x37},
	{0x3972, 0x37},
	{0x3973, 0x37},
	{0x3974, 0x00},
	{0x3975, 0x3c},
	{0x3976, 0x3c},
	{0x3977, 0x3c},
	{0x3978, 0x3c},
	{0x3c00, 0x0f},
	{0x3c20, 0x01},
	{0x3c21, 0x08},
	{0x3f00, 0x8b},
	{0x3f02, 0x0f},
	{0x4000, 0xc3},
	{0x4001, 0xe0},
	{0x4002, 0x00},
	{0x4003, 0x40},
	{0x4008, 0x04},
	{0x4009, 0x23},
	{0x400a, 0x04},
	{0x400b, 0x01},
	{0x4041, 0x20},
	{0x4077, 0x06},
	{0x4078, 0x00},
	{0x4079, 0x1a},
	{0x407a, 0x7f},
	{0x407b, 0x01},
	{0x4080, 0x03},
	{0x4081, 0x84},
	{0x4308, 0x03},
	{0x4309, 0xff},
	{0x430d, 0x00},
	{0x4806, 0x00},
	{0x4813, 0x00},
	{0x4837, 0x10},
	{0x4857, 0x05},
	{0x4884, 0x04},
	{0x4500, 0x07},
	{0x4501, 0x00},
	{0x4503, 0x00},
	{0x450a, 0x04},
	{0x450e, 0x00},
	{0x450f, 0x00},
	{0x4800, 0x64},
	{0x4900, 0x00},
	{0x4901, 0x00},
	{0x4902, 0x01},
	{0x4d00, 0x03},
	{0x4d01, 0xd8},
	{0x4d02, 0xba},
	{0x4d03, 0xa0},
	{0x4d04, 0xb7},
	{0x4d05, 0x34},
	{0x4d0d, 0x00},
	{0x5000, 0xfd},
	{0x5001, 0x50},
	{0x5006, 0x00},
	{0x5080, 0x40},
	{0x5181, 0x2b},
	{0x5202, 0xa3},
	{0x5206, 0x01},
	{0x5207, 0x00},
	{0x520a, 0x01},
	{0x520b, 0x00},
	{0x365d, 0x00},
	{0x4815, 0x40},
	{0x4816, 0x12},
	{0x481f, 0x30},
	{0x4f00, 0x01},
	// plls
	{0x0303, 0x05},
	{0x0305, 0x90},
	{0x0316, 0x90},
	{0x3016, 0x32},
};

static int32_t OV02C_WriteTable(OV02C_Object_t *pObj, const struct regval *regs,
		uint32_t size);
static int32_t OV02C_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV02C_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV02C_Delay(OV02C_Object_t *pObj, uint32_t Delay);

static int32_t OV02C_SetAnalogGain(OV02C_Object_t *pObj, float gain_dBm);
static int32_t OV02C_SetDigitalGain(OV02C_Object_t *pObj, float gain_dBm);

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

static int32_t OV02C_WriteTable(OV02C_Object_t *pObj, const struct regval *regs,
		uint32_t size) {
	uint32_t index;
	int32_t ret = OV02C_OK;

	/* Set registers */
	for (index = 0; index < size; index++) {
		if (ret != OV02C_ERROR) {
			if (ov02c_write_reg(&pObj->Ctx, regs[index].addr,
					(uint8_t*) &(regs[index].val), 1) != OV02C_OK) {
				ret = OV02C_ERROR;
			}
		}
	}
	return ret;
}

static int32_t OV02C_Delay(OV02C_Object_t *pObj, uint32_t Delay) {
	uint32_t tickstart;
	tickstart = pObj->IO.GetTick();
	while ((pObj->IO.GetTick() - tickstart) < Delay) {
	}
	return OV02C_OK;
}

static int32_t OV02C_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV02C_Object_t *pObj = (OV02C_Object_t*) handle;

	return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

static int32_t OV02C_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData,
		uint16_t Length) {
	OV02C_Object_t *pObj = (OV02C_Object_t*) handle;

	return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

int32_t OV02C_RegisterBusIO(OV02C_Object_t *pObj, OV02C_IO_t *pIO) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV02C_ERROR;
	} else {
		pObj->IO.Init = pIO->Init;
		pObj->IO.DeInit = pIO->DeInit;
		pObj->IO.Address = pIO->Address;
		pObj->IO.WriteReg = pIO->WriteReg;
		pObj->IO.ReadReg = pIO->ReadReg;
		pObj->IO.GetTick = pIO->GetTick;

		pObj->Ctx.ReadReg = OV02C_ReadRegWrap;
		pObj->Ctx.WriteReg = OV02C_WriteRegWrap;
		pObj->Ctx.handle = pObj;

		if (pObj->IO.Init != NULL) {
			ret = pObj->IO.Init();
		} else {
			ret = OV02C_ERROR;
		}
	}

	return ret;
}

int32_t OV02C_Init(OV02C_Object_t *pObj, uint32_t Resolution,
		uint32_t PixelFormat) {
	int32_t ret = OV02C_OK;

	if (pObj->IsInitialized == 0U) {
		switch (Resolution) {
		case OV02C_RESOLUTION_1928_1082:
			if (OV02C_WriteTable(pObj,
					ov02c10_input_24M_MIPI_2lane_raw10_1928x1082_30fps,
					ARRAY_SIZE(
							ov02c10_input_24M_MIPI_2lane_raw10_1928x1082_30fps)) != OV02C_OK) {
				ret = OV02C_ERROR;
			}
			break;
			/* Add new resolution here */
		default:
			/* Resolution not supported */
			ret = OV02C_ERROR;
		}

		if (!ret) {
			pObj->IsInitialized = 1U;
		}
	}

	return ret;
}

int32_t OV02C_DeInit(OV02C_Object_t *pObj) {
	if (pObj->IsInitialized == 1U) {
		/* De-initialize camera sensor interface */
		pObj->IsInitialized = 0U;
	}

	return OV02C_OK;
}

int32_t OV02C_Start(OV02C_Object_t *pObj) {
	int ret;
	uint8_t tmp;
	/* Start streaming */
	tmp = OV02C_MODE_STREAMING;
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1) != OV02C_OK) {
		ret = OV02C_ERROR;
	} else {
		OV02C_Delay(pObj, 20);
	}
	return ret;
}

int32_t OV02C_ReadID(OV02C_Object_t *pObj, uint32_t *Id) {
	int32_t ret = OV02C_OK;
	uint8_t tmp[3];

	/* Initialize I2C */
	pObj->IO.Init();

	if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_2, &tmp[2], 1) != OV02C_OK) {
		ret = OV02C_ERROR;
	} else {
		if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_1, &tmp[1],
				1) != OV02C_OK) {
			ret = OV02C_ERROR;
		} else {
			if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_0, &tmp[0],
					1) != OV02C_OK) {
				ret = OV02C_ERROR;
			} else {
				ret = OV02C_OK;
			}
		}

	}
	// for some reason, LSB of chip ID is not 0x03, but 0x43... ignoring it for now
	if (!ret) {
		*Id = (tmp[2] << 8) | (tmp[1] << 0);
	}

	return ret;
}

int32_t OV02C_GetCapabilities(OV02C_Object_t *pObj,
		OV02C_Capabilities_t *Capabilities) {
	int32_t ret;

	if (pObj == NULL) {
		ret = OV02C_ERROR;
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

		ret = OV02C_OK;
	}

	return ret;
}

int32_t OV02C_GetSensorInfo(OV02C_Object_t *pObj, OV02C_SensorInfo_t *Info) {
	if ((!pObj) || (Info == NULL)) {
		return OV02C_ERROR;
	}

	if (sizeof(Info->name) >= strlen(OV02C_NAME) + 1) {
		strcpy(Info->name, OV02C_NAME);
	} else {
		return OV02C_ERROR;
	}

	Info->bayer_pattern = OV02C_BAYER_PATTERN;
	Info->color_depth = OV02C_COLOR_DEPTH;
	Info->width = OV02C_WIDTH;
	Info->height = OV02C_HEIGHT;
	Info->gain_min = OV02C_GAIN_MIN;
	Info->gain_max = OV02C_GAIN_MAX;
	Info->exposure_min = OV02C_EXPOSURE_MIN;
	Info->exposure_max = OV02C_EXPOSURE_MAX;

	return OV02C_OK;
}

static int32_t OV02C_SetAnalogGain(OV02C_Object_t *pObj, float gain_dBm) {
	int ret = OV02C_OK;
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV02C_ANALOG_GAIN_MAX_DBM) {
		gain_dBm = OV02C_ANALOG_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV02C_ANALOG_GAIN_MAX_LINEAR) {
		linear_gain = OV02C_ANALOG_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint16_t reg_value = (uint16_t) map_range_to_uint32(linear_gain, 1.0f,
			OV02C_ANALOG_GAIN_MAX_LINEAR, OV02C_ANALOG_GAIN_MIN_REG,
			OV02C_ANALOG_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN16(reg_value);
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_ANALOG_GAIN,
			(uint8_t*) &reg_value, 1) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_gain;
	}
	exit_gain: return ret;
}

static int32_t OV02C_SetDigitalGain(OV02C_Object_t *pObj, float gain_dBm) {
	int ret = OV02C_OK;
	if (gain_dBm < 0) {
		gain_dBm = 0;
	} else if (gain_dBm > OV02C_DIGITAL_GAIN_MAX_DBM) {
		gain_dBm = OV02C_DIGITAL_GAIN_MAX_DBM;
	}
	// convert dBm to linear gain
	float linear_gain = MDECIBEL_TO_LINEAR(gain_dBm);
	if (linear_gain > OV02C_DIGITAL_GAIN_MAX_LINEAR) {
		linear_gain = OV02C_DIGITAL_GAIN_MAX_LINEAR;
	}
	// convert linear gain to register value
	uint32_t reg_value = map_range_to_uint32(linear_gain, 1.0f,
			OV02C_DIGITAL_GAIN_MAX_LINEAR, OV02C_DIGITAL_GAIN_MIN_REG,
			OV02C_DIGITAL_GAIN_MAX_REG);
	reg_value = SWAP_ENDIAN32(reg_value);
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_DIGITAL_GAIN,
			((uint8_t*) &reg_value) + 1, 3) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_gain;
	}
	exit_gain: return ret;
}

int32_t OV02C_SetGain(OV02C_Object_t *pObj, int32_t gain_dBm) {
	int32_t ret = 0;

	if (gain_dBm < OV02C_GAIN_MIN) {
		gain_dBm = OV02C_GAIN_MIN;
	} else if (gain_dBm > OV02C_GAIN_MAX) {
		gain_dBm = OV02C_GAIN_MAX;
	}

	float analog_gain_dBm =
			(gain_dBm > OV02C_ANALOG_GAIN_MAX_DBM) ?
					OV02C_ANALOG_GAIN_MAX_DBM : gain_dBm;
	float digital_gain_dBm = gain_dBm - analog_gain_dBm;
	if (OV02C_SetAnalogGain(pObj, analog_gain_dBm) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_gain;
	}
	if (OV02C_SetDigitalGain(pObj, digital_gain_dBm) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_gain;
	}

	exit_gain: return ret;
}

int32_t OV02C_SetExposure(OV02C_Object_t *pObj, int32_t exposure_us) {
	int32_t ret = 0;

	// Exposure calculations
	// line time (t_line) = Horizontal Total Size (HTS) / Pixel Clock (PCLK)
	// Vertical Total Size (VTS) = Exposure (t_exposure) / t_line = Exposure / (HTS / PCLK)
	// PCLK = FPS * HTS * VTS
	uint64_t pixel_clock = 80000000ULL;

	uint16_t hts = 0, vts = 0;
	// read HTS and VTS
	if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_HTS, (uint8_t*) &hts,
			2) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_exp;
	}
	hts = SWAP_ENDIAN16(hts);
	if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_VTS, (uint8_t*) &vts,
			2) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_exp;
	}
	vts = SWAP_ENDIAN16(vts);
	/* Calculate number of lines (rounded) */
	uint32_t line_us = (hts * 1000000) / pixel_clock;
	uint32_t lines = exposure_us / line_us;

	/* Clamp to sensor limits */
	if (lines < OV02C_EXPOSURE_MIN_LINES)
		lines = OV02C_EXPOSURE_MIN_LINES;
	if (lines > vts - OV02C_EXPOSURE_MAX_LINES_MARGIN)
		lines = vts - OV02C_EXPOSURE_MAX_LINES_MARGIN; /* 16-bit register limit */

	// TODO maybe add hold functionality

	/* Write exposure lines (16-bit: 0x3501 = MSB, 0x3502 = LSB) */
	uint16_t exp_val = SWAP_ENDIAN16((uint16_t )lines);
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_EXPOSURE, (uint8_t*) &exp_val, 2)
			!= 0) {
		ret = OV02C_ERROR;
		goto exit_exp;
	}
	exit_exp:
	// TODO: maybe add hold functionality if possible
	return ret;
}

int32_t OV02C_MirrorFlipConfig(OV02C_Object_t *pObj, OV02C_MirrorFlip_t Config) {
	int32_t ret = OV02C_OK;
	uint8_t tmp;

	uint16_t shift_x = 0x0003, shift_y = 0x0003;

	switch (Config) {
	case OV02C_FLIP:
		tmp = 0xa0;
		shift_x -= 1;  // to keep same Bayer pattern
		break;
	case OV02C_MIRROR:
		tmp = 0xb8;
		shift_y -= 1;
		break;
	case OV02C_MIRROR_FLIP:
		tmp = 0xb0;
		shift_x -= 1;
		shift_y -= 1;
		break;
	case OV02C_MIRROR_FLIP_NONE:
	default:
		tmp = 0xa8;
		break;
	}
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_FORMAT, &tmp, 1) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_mirrorflip;
	}
	shift_x = SWAP_ENDIAN16(shift_x);
	if (ov02c_write_reg(&pObj->Ctx, OV02C10_REG_ISP_X_WIN_CONTROL,
			(uint8_t*) &shift_x, 2) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_mirrorflip;
	}
	shift_y = SWAP_ENDIAN16(shift_y);
	if (ov02c_write_reg(&pObj->Ctx, OV02C10_REG_ISP_Y_WIN_CONTROL,
			(uint8_t*) &shift_y, 2) != OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_mirrorflip;
	}
	exit_mirrorflip: return ret;
}
