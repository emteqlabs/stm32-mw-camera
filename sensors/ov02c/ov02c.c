#include "ov02c.h"
#include <string.h>
#include <math.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MDECIBEL_TO_LINEAR(mdB)             (powf(10.0f, (mdB / 1000.0f) / 20.0f))
#define LINEAR_TO_MDECIBEL(linearValue)     (1000 * (20.0f * log10f(linearValue)))

OV02C_CAMERA_Drv_t OV02C_CAMERA_Driver = { .Init = OV02C_Init, .DeInit =
		OV02C_DeInit, .ReadID = OV02C_ReadID, .GetCapabilities =
		OV02C_GetCapabilities, .SetGain = OV02C_SetGain, .SetExposure =
		OV02C_SetExposure, .SetFrequency = OV02C_SetFrequency,
		.MirrorFlipConfig = OV02C_MirrorFlipConfig, .GetSensorInfo =
				OV02C_GetSensorInfo, .SetTestPattern = OV02C_SetTestPattern };

struct regval {
	uint16_t addr;
	uint8_t val;
};

static const struct regval ov02c10_input_24M_MIPI_2lane_raw10_1920x1080_30fps[] = {
	{0X0103, 0X01},
	{0X0301, 0X08},
	{0X0303, 0X06},
	{0X0304, 0X01},
	{0X0305, 0X90},
	{0X0313, 0X40},
	{0X031c, 0X4f},
	{0X3016, 0X32},
	{0X301b, 0Xf0},
	{0X3020, 0X97},
	{0X3021, 0X23},
	{0X3022, 0X01},
	{0X3026, 0Xb4},
	{0X3027, 0Xf1},
	{0X303b, 0X00},
	{0X303c, 0X4f},
	{0X303d, 0Xe6},
	{0X303e, 0X00},
	{0X303f, 0X03},
	{0X3501, 0X04},
	{0X3502, 0X6c},
	{0X3504, 0X0c},
	{0X3507, 0X00},
	{0X3508, 0X08},
	{0X3509, 0X00},
	{0X350a, 0X01},
	{0X350b, 0X00},
	{0X350c, 0X41},
	{0X3600, 0X84},
	{0X3603, 0X08},
	{0X3610, 0X57},
	{0X3611, 0X1b},
	{0X3613, 0X78},
	{0X3623, 0X00},
	{0X3632, 0Xa0},
	{0X3642, 0Xe8},
	{0X364c, 0X70},
	{0X365d, 0X00},
	{0X365f, 0X0f},
	{0X3708, 0X30},
	{0X3714, 0X24},
	{0X3725, 0X02},
	{0X3737, 0X08},
	{0X3739, 0X28},
	{0X3749, 0X32},
	{0X374a, 0X32},
	{0X374b, 0X32},
	{0X374c, 0X32},
	{0X374d, 0X81},
	{0X374e, 0X81},
	{0X374f, 0X81},
	{0X3752, 0X36},
	{0X3753, 0X36},
	{0X3754, 0X36},
	{0X3761, 0X00},
	{0X376c, 0X81},
	{0X3774, 0X18},
	{0X3776, 0X08},
	{0X377c, 0X81},
	{0X377d, 0X81},
	{0X377e, 0X81},
	{0X37a0, 0X44},
	{0X37a6, 0X44},
	{0X37aa, 0X0d},
	{0X37ae, 0X00},
	{0X37cb, 0X03},
	{0X37cc, 0X01},
	{0X37d8, 0X02},
	{0X37d9, 0X10},
	{0X37e1, 0X10},
	{0X37e2, 0X18},
	{0X37e3, 0X08},
	{0X37e4, 0X08},
	{0X37e5, 0X02},
	{0X37e6, 0X08},
	{0X3800, 0X00},
	{0X3801, 0X00},
	{0X3802, 0X00},
	{0X3803, 0X04},
	{0X3804, 0X07},
	{0X3805, 0X8f},
	{0X3806, 0X04},
	{0X3807, 0X43},
	{0X3808, 0X07},
	{0X3809, 0X80},
	{0X380a, 0X04},
	{0X380b, 0X38},
	{0X380c, 0X04},  // HTS MSB
	{0X380d, 0X74},  // HTS LSB
	{0X380e, 0X09},  // VTS MSB
	{0X380f, 0X18},  // VTS LSB
	{0X3810, 0X00},
	{0X3811, 0X07},
	{0X3812, 0X00},
	{0X3813, 0X04},
	{0X3814, 0X01},
	{0X3815, 0X01},
	{0X3816, 0X01},
	{0X3817, 0X01},
	{0X3820, 0Xa8},
	{0X3821, 0X00},
	{0X3822, 0X80},
	{0X3823, 0X08},
	{0X3824, 0X00},
	{0X3825, 0X20},
	{0X3826, 0X00},
	{0X3827, 0X08},
	{0X382a, 0X00},
	{0X382b, 0X08},
	{0X382d, 0X00},
	{0X382e, 0X00},
	{0X382f, 0X23},
	{0X3834, 0X00},
	{0X3839, 0X00},
	{0X383a, 0Xd1},
	{0X383e, 0X03},
	{0X393d, 0X29},
	{0X393f, 0X6e},
	{0X394b, 0X06},
	{0X394c, 0X06},
	{0X394d, 0X08},
	{0X394e, 0X0a},
	{0X394f, 0X01},
	{0X3950, 0X01},
	{0X3951, 0X01},
	{0X3952, 0X01},
	{0X3953, 0X01},
	{0X3954, 0X01},
	{0X3955, 0X01},
	{0X3956, 0X01},
	{0X3957, 0X0e},
	{0X3958, 0X08},
	{0X3959, 0X08},
	{0X395a, 0X08},
	{0X395b, 0X13},
	{0X395c, 0X09},
	{0X395d, 0X05},
	{0X395e, 0X02},
	{0X395f, 0X00},
	{0X395f, 0X00},
	{0X3960, 0X00},
	{0X3961, 0X00},
	{0X3962, 0X00},
	{0X3963, 0X00},
	{0X3964, 0X00},
	{0X3965, 0X00},
	{0X3966, 0X00},
	{0X3967, 0X00},
	{0X3968, 0X01},
	{0X3969, 0X01},
	{0X396a, 0X01},
	{0X396b, 0X01},
	{0X396c, 0X10},
	{0X396d, 0Xf0},
	{0X396e, 0X11},
	{0X396f, 0X00},
	{0X3970, 0X37},
	{0X3971, 0X37},
	{0X3972, 0X37},
	{0X3973, 0X37},
	{0X3974, 0X00},
	{0X3975, 0X3c},
	{0X3976, 0X3c},
	{0X3977, 0X3c},
	{0X3978, 0X3c},
	{0X3c00, 0X0f},
	{0X3c20, 0X01},
	{0X3c21, 0X08},
	{0X3f00, 0X8b},
	{0X3f02, 0X0f},
	{0X4000, 0Xc3},
	{0X4001, 0Xe0},
	{0X4002, 0X00},
	{0X4003, 0X40},
	{0X4008, 0X04},
	{0X4009, 0X23},
	{0X400a, 0X04},
	{0X400b, 0X01},
	{0X4041, 0X20},
	{0X4077, 0X06},
	{0X4078, 0X00},
	{0X4079, 0X1a},
	{0X407a, 0X7f},
	{0X407b, 0X01},
	{0X4080, 0X03},
	{0X4081, 0X84},
	{0X4308, 0X03},
	{0X4309, 0Xff},
	{0X430d, 0X00},
	{0X4500, 0X07},
	{0X4501, 0X00},
	{0X4503, 0X00},
	{0X450a, 0X04},
	{0X450e, 0X00},
	{0X450f, 0X00},
	{0X4800, 0X64},
	{0X4806, 0X00},
	{0X4813, 0X00},
	{0X4815, 0X40},
	{0X4816, 0X12},
	{0X481f, 0X30},
	{0X4837, 0X14},
	{0X4857, 0X05},
	{0X4884, 0X04},
	{0X4900, 0X00},
	{0X4901, 0X00},
	{0X4902, 0X01},
	{0X4d00, 0X03},
	{0X4d01, 0Xd8},
	{0X4d02, 0Xba},
	{0X4d03, 0Xa0},
	{0X4d04, 0Xb7},
	{0X4d05, 0X34},
	{0X4d0d, 0X00},
	{0X5000, 0Xfd},
	{0X5001, 0X50},
	{0X5006, 0X00},
	{0X5080, 0X40},
	{0X5181, 0X2b},
	{0X5202, 0Xa3},
	{0X5206, 0X01},
	{0X5207, 0X00},
	{0X520a, 0X01},
	{0X520b, 0X00},
	{0X4f00, 0X01},
};

//static const struct regval test_pattern_enable_regs[] = { };
//
//static const struct regval test_pattern_disable_regs[] = { };

static int32_t OV02C_WriteTable(OV02C_Object_t *pObj, const struct regval *regs,
		uint32_t size);
static int32_t OV02C_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV02C_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data,
		uint16_t Length);
static int32_t OV02C_Delay(OV02C_Object_t *pObj, uint32_t Delay);

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
		case OV02C_RESOLUTION_1920_1080:
			if (OV02C_WriteTable(pObj,
					ov02c10_input_24M_MIPI_2lane_raw10_1920x1080_30fps,
					ARRAY_SIZE(
							ov02c10_input_24M_MIPI_2lane_raw10_1920x1080_30fps))
					!= OV02C_OK) {
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
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1)
			!= OV02C_OK) {
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

	if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_2, &tmp[2], 1)
			!= OV02C_OK) {
		ret = OV02C_ERROR;
	} else {
		if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_1, &tmp[1], 1)
				!= OV02C_OK) {
			ret = OV02C_ERROR;
		} else {
			if (ov02c_read_reg(&pObj->Ctx, OV02C_REG_ID_BYTE_0, &tmp[0], 1)
					!= OV02C_OK) {
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
		Capabilities->Config_TestPattern = 1;

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
	Info->gain_min = OV02C_ANALOG_GAIN_MIN;
	Info->gain_max = OV02C_ANALOG_GAIN_MAX;
	Info->exposure_min = OV02C_EXPOSURE_MIN;
	Info->exposure_max = OV02C_EXPOSURE_MAX;

	return OV02C_OK;
}

int32_t OV02C_SetGain(OV02C_Object_t *pObj, int32_t gain) {
	int32_t ret = 0;
//	uint8_t tmp = 0;
//	uint32_t analog_ctrl, digital_ctrl;
//	float gain_ratio;
//
//	/* Compute desired gain ratio from mdB (20*log10 scale): */
//	gain_ratio = powf(10.0f, (float)gain / 2000.0f);
//	if (gain_ratio < 1.0f) {
//		gain_ratio = 1.0f;  /* minimum gain = 0 dB */
//	}
//	/* Determine analog gain (prefers analog up to its max) */
//	float analog_max = (float)OV02C_ANALOG_GAIN_MAX / OV02C_ANALOG_GAIN_MIN; /* ≈15.5x */
//	float analog_gain = (gain_ratio <= analog_max) ? gain_ratio : analog_max;
//	float digital_gain = (gain_ratio <= analog_max) ? 1.0f : (gain_ratio / analog_gain);
//	/* Clamp to digital max */
//	if (digital_gain > (float)OV02C_DIGITAL_GAIN_MAX / OV02C_DIGITAL_GAIN_MIN) {
//		digital_gain = (float)OV02C_DIGITAL_GAIN_MAX / OV02C_DIGITAL_GAIN_MIN; /* ≈16x */
//	}
//
//	/* Convert to register integer values (round to nearest) */
//	analog_ctrl = (uint32_t)roundf(analog_gain * OV02C_ANALOG_GAIN_MIN);
//	digital_ctrl = (uint32_t)roundf(digital_gain * OV02C_DIGITAL_GAIN_MIN);
//
//	/* Clamp control values to sensor limits */
//	if (analog_ctrl < OV02C_ANALOG_GAIN_MIN) analog_ctrl = OV02C_ANALOG_GAIN_MIN;
//	if (analog_ctrl > OV02C_ANALOG_GAIN_MAX) analog_ctrl = OV02C_ANALOG_GAIN_MAX;
//	if (digital_ctrl < OV02C_DIGITAL_GAIN_MIN) digital_ctrl = OV02C_DIGITAL_GAIN_MIN;
//	if (digital_ctrl > OV02C_DIGITAL_GAIN_MAX) digital_ctrl = OV02C_DIGITAL_GAIN_MAX;
//
//	/* Disable streaming to synchronize update (TODO: maybe it is better to enable HOLD function) */
//	tmp = OV02C_MODE_STANDBY;
//	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1)
//			!= OV02C_OK) {
//		ret = OV02C_ERROR;
//		goto exit_gain;
//	} else {
//		OV02C_Delay(pObj, 20);
//	}
//
//	/* Write analog gain (16-bit: 0x3508 = MSB, 0x3509 = LSB)*/
//	{
//		uint16_t analog_val = (uint16_t)(analog_ctrl << 4); /* shift per datasheet*/
//		tmp = (analog_val >> 8) & 0xFF;
//		if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_ANALOG_GAIN, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_gain; }
//		tmp = analog_val & 0xFF;
//		if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_ANALOG_GAIN + 1, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_gain; }
//	}
//
//	/* Write digital gain (24-bit: 0x350A = MSB, 0x350C = LSB)*/
//	{
//		uint32_t digital_val = (uint32_t)(digital_ctrl << 6); /* shift per datasheet*/
//		tmp = (digital_val >> 16) & 0xFF;
//		if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_DIGITAL_GAIN,     &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_gain; }
//		tmp = (digital_val >> 8)  & 0xFF;
//		if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_DIGITAL_GAIN + 1, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_gain; }
//		tmp = digital_val        & 0xFF;
//		if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_DIGITAL_GAIN + 2, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_gain; }
//	}
//
//exit_gain:
//	/* Re-enable streaming to apply updates (TODO: maybe it is better to enable HOLD function) */
//	tmp = OV02C_MODE_STREAMING;
//	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1)
//			!= OV02C_OK) {
//		ret = OV02C_ERROR;
//	} else {
//		OV02C_Delay(pObj, 20);
//	}
	return ret;
}

int32_t OV02C_SetExposure(OV02C_Object_t *pObj, int32_t exposure_us)
{
    int32_t ret = 0;
    uint8_t tmp = 0;

    // Exposure calculations
    // line time (t_line) = Horizontal Total Size (HTS) / Pixel Clock (PCLK)
    // Vertical Total Size (VTS) = Exposure (t_exposure) / t_line = Exposure / (HTS / PCLK)
    // PCLK = FPS * HTS * VTS
    uint64_t pixel_clock = 80000000ULL;
    uint32_t hts = 1140;  // from config registers (TODO read the regs)
    uint32_t vts = 2328;  // from config registers (TODO read the regs
    /* Calculate number of lines (rounded) */
    uint32_t line_us = (hts * 1000000) / pixel_clock;
    uint32_t lines = exposure_us / line_us;
//    uint32_t lines = (uint32_t)((exposure_us * pixel_clock + 500000ULL) / (hts * 1000000ULL));

    /* Clamp to sensor limits */
    if (lines < OV02C_EXPOSURE_MIN_LINES) lines = OV02C_EXPOSURE_MIN_LINES;
    if (lines > vts - OV02C_EXPOSURE_MAX_LINES_MARGIN) lines = vts - OV02C_EXPOSURE_MAX_LINES_MARGIN; /* 16-bit register limit */

    /* Disable streaming (TODO: maybe it is better to enable HOLD function) */
    tmp = OV02C_MODE_STANDBY;
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1)
			!= OV02C_OK) {
		ret = OV02C_ERROR;
		goto exit_exp;
	} else {
		OV02C_Delay(pObj, 20);
	}

    /* Write exposure lines (16-bit: 0x3501 = MSB, 0x3502 = LSB) */
    {
        uint16_t exp_val = (uint16_t)lines;
        tmp = (exp_val >> 8) & 0xFF;
        if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_EXPOSURE, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_exp; }
        tmp = exp_val & 0xFF;
        if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_EXPOSURE + 1, &tmp, 1) != 0) { ret = OV02C_ERROR; goto exit_exp; }
    }

exit_exp:
    /* Re-enable streaming (TODO: maybe it is better to enable HOLD function) */
	tmp = OV02C_MODE_STREAMING;
	if (ov02c_write_reg(&pObj->Ctx, OV02C_REG_MODE_SELECT, &tmp, 1)
			!= OV02C_OK) {
		ret = OV02C_ERROR;
	} else {
		OV02C_Delay(pObj, 20);
	}
    return ret;
}

int32_t OV02C_MirrorFlipConfig(OV02C_Object_t *pObj, uint32_t Config) {
	int32_t ret = OV02C_ERROR;
	return ret;
}

int32_t OV02C_SetTestPattern(OV02C_Object_t *pObj, int32_t mode) {
	int32_t ret = OV02C_ERROR;
	return ret;
}
