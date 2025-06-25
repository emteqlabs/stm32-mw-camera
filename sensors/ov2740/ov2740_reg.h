#ifndef OV2740_REG_H
#define OV2740_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define OV2740_NAME               	"OV2740"

#define OV2740_REG_ID_BYTE_2         0x300a
#define OV2740_REG_ID_BYTE_1         0x300b
#define OV2740_REG_ID_BYTE_0         0x300c
#define OV2740_CHIP_ID               0x002740

#define OV2740_REG_MODE_SELECT    		0x0100
#define OV2740_MODE_STANDBY         	0x00
#define OV2740_MODE_STREAMING       	0x01

#define OV2740_REG_EXPOSURE         	0x3500	// Exposure, 20-bit, low 4-bits are fraction bits
#define OV2740_REG_ANALOG_GAIN      	0x3508  // Analog gain register (13-bit)
//#define OV2740_REG_DIGITAL_GAIN

#define OV2740_EXPOSURE_MIN_LINES         	4       // min lines
#define OV2740_EXPOSURE_MAX_LINES_MARGIN  	15      // margin for max exposure
#define OV2740_EXPOSURE_MAX_LINES  			2145    // max lines

#define OV2740_EXPOSURE_MIN         		30		// in us, 7.5 us per line (hts / pclk is single line)
#define OV2740_EXPOSURE_MAX					12000	// in us, 7.5 us per line

#define OV2740_ANALOG_GAIN_MAX_LINEAR		15.500f  // analog gain linear max value
#define OV2740_DIGITAL_GAIN_MAX_LINEAR		15.500f  // digital gain linear max value
#define OV2740_ANALOG_GAIN_MAX_DBM			23805  // analog gain dBm max value
#define OV2740_DIGITAL_GAIN_MAX_DBM			23805  // digital gain dBm max value
#define OV2740_ANALOG_GAIN_MIN_REG			0x080
#define OV2740_ANALOG_GAIN_MAX_REG			0x7C0
#define OV2740_DIGITAL_GAIN_MIN_REG			0x010000
#define OV2740_DIGITAL_GAIN_MAX_REG			0x0FFFC0
#define OV2740_GAIN_MIN						0      // dBm
#define OV2740_GAIN_MAX						47610  // dBm

#define OV2740_REG_HTS				0x380C
#define OV2740_REG_HTS_MSB			0x380C
#define OV2740_REG_HTS_LSB			0x380D
#define OV2740_REG_VTS 				0x380E
#define OV2740_REG_VTS_MSB			0x380E
#define OV2740_REG_VTS_LSB			0x380F

#define OV2740_REG_FORMAT				0x3820
#define OV2740_REG_ISP_X_WIN_CONTROL	0x3810
#define OV2740_REG_ISP_Y_WIN_CONTROL	0x3812

#define OV2740_BAYER_PATTERN      	0x01 /* From ISP definition */
#define OV2740_COLOR_DEPTH        	10   /* in bits */

#define OV2740_WIDTH              	1932
#define OV2740_HEIGHT             	1092

typedef int32_t (*OV2740_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV2740_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV2740_Write_Func   WriteReg;
  OV2740_Read_Func    ReadReg;
  void               *handle;
} ov2740_ctx_t;

int32_t ov2740_write_reg(ov2740_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t ov2740_read_reg(ov2740_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* OV2740_REG_H */
