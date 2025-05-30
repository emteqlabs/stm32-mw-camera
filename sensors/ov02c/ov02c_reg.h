#ifndef OV02C_REG_H
#define OV02C_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define OV02C_NAME               	"OV02C10"

#define OV02C_REG_ID_BYTE_2         0x300a
#define OV02C_REG_ID_BYTE_1         0x300b
#define OV02C_REG_ID_BYTE_0         0x300c
#define OV02C_CHIP_ID            	0x5602  // last byte, is not 0x03 as per datasheet, but 0x43, ignoring it

#define OV02C_REG_MODE_SELECT    	0x0100
#define OV02C_MODE_STANDBY         	0x00
#define OV02C_MODE_STREAMING       	0x01

#define OV02C_REG_EXPOSURE         	0x3501  // Coarse exposure (MSB), 16-bit
#define OV02C_REG_ANALOG_GAIN      	0x3508  // Analog gain register (16-bit, upper bits used)
#define OV02C_REG_DIGITAL_GAIN     	0x350A  // Digital gain register (24-bit)

#define OV02C_EXPOSURE_MIN_LINES         	4       // min lines
#define OV02C_EXPOSURE_MAX_LINES_MARGIN  	15      // margin for max exposure
#define OV02C_EXPOSURE_MAX_LINES  			2310    // max lines
#define OV02C_EXPOSURE_MIN         			57		// in us, 4 lines, 14.25 us per line (hts / pclk is single line)
#define OV02C_EXPOSURE_MAX					32917	// in us, 2310 lines, 14.25 us per line
#define OV02C_ANALOG_GAIN_MIN		       	0x10    // analog gain register min
#define OV02C_ANALOG_GAIN_MAX		       	0xF8    // analog gain register max
#define OV02C_DIGITAL_GAIN_MIN		      	0x0400  // digital gain register min
#define OV02C_DIGITAL_GAIN_MAX		      	0x3FFF
#define OV02C_GAIN_MIN						0			// TODO
#define OV02C_GAIN_MAX						(48 * 1000)	// TODO

#define OV02C_REG_HTS_MSB			0x380C
#define OV02C_REG_HTS_LSB			0x380D

#define OV02C_REG_VTS_MSB			0x380E
#define OV02C_REG_VTS_LSB			0x380F

#define OV02C_BAYER_PATTERN      	0x01 /* From ISP definition BGGR / TODO comnon enumeration in camera */
#define OV02C_COLOR_DEPTH        	10 /* in bits */

#define OV02C_WIDTH              	1928
#define OV02C_HEIGHT             	1082

typedef int32_t (*OV02C_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV02C_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV02C_Write_Func   WriteReg;
  OV02C_Read_Func    ReadReg;
  void               *handle;
} ov02c_ctx_t;

int32_t ov02c_write_reg(ov02c_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t ov02c_read_reg(ov02c_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);

int32_t ov02c_register_set(ov02c_ctx_t *ctx, uint16_t reg, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* OV02C_REG_H */
