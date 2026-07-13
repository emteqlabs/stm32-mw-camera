#ifndef OV05C10_REG_H
#define OV05C10_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define OV05C10_NAME                          "OV05C1010"

#define OV05C10_REG_ID_BYTE_2                 0x300a
#define OV05C10_REG_ID_BYTE_1                 0x300b
#define OV05C10_REG_ID_BYTE_0                 0x300c
#define OV05C10_CHIP_ID                       0x5602  // last byte, is not 0x03 as per datasheet, but 0x43, ignoring it

#define OV05C10_REG_MODE_SELECT               0x0100
#define OV05C10_MODE_STANDBY                  0x00
#define OV05C10_MODE_STREAMING                0x01

#define OV05C10_REG_EXPOSURE                  0x3501  // Coarse exposure (MSB), 16-bit
#define OV05C10_REG_ANALOG_GAIN               0x3508  // Analog gain register (16-bit, upper bits used)
#define OV05C10_REG_DIGITAL_GAIN              0x350A  // Digital gain register (24-bit)

#define OV05C10_EXPOSURE_MIN_LINES            (uint16_t)4       // min exposure in lines
#define OV05C10_EXPOSURE_MIN_VTS              (uint16_t)0x048C  // min vts
#define OV05C10_EXPOSURE_MAX_LINES_MARGIN     (uint16_t)15      // margin for max exposure
#define OV05C10_EXPOSURE_MAX_VTS              (uint16_t)0x048C  // max vts

#define OV05C10_EXPOSURE_MIN                  77       // in us, 4 lines, 19.28 us per line (hts / pclk is single line)
#define OV05C10_EXPOSURE_MAX                  22442    // in us, 1164 lines (default VTS), 19.28 us per line

#define OV05C10_ANALOG_GAIN_MAX_LINEAR        15.500f  // analog gain linear max value
#define OV05C10_DIGITAL_GAIN_MAX_LINEAR       15.500f  // digital gain linear max value
#define OV05C10_ANALOG_GAIN_MAX_DBM           23805  // analog gain dBm max value
#define OV05C10_DIGITAL_GAIN_MAX_DBM          23805  // digital gain dBm max value
#define OV05C10_ANALOG_GAIN_MIN_REG           0x0100
#define OV05C10_ANALOG_GAIN_MAX_REG           0x0F80
#define OV05C10_DIGITAL_GAIN_MIN_REG          0x010000
#define OV05C10_DIGITAL_GAIN_MAX_REG          0x0FFFC0
// analog gain is used up to 23805 dBm, rest is digital
#define OV05C10_GAIN_MIN                      0      // dBm
#define OV05C10_GAIN_MAX                      30000  // dBm

#define OV05C10_REG_HTS                       0x380C
#define OV05C10_REG_HTS_MSB                   0x380C
#define OV05C10_REG_HTS_LSB                   0x380D
#define OV05C10_REG_VTS                       0x380E
#define OV05C10_REG_VTS_MSB                   0x380E
#define OV05C10_REG_VTS_LSB                   0x380F

#define OV05C10_REG_FORMAT                    0x3820
#define OV05C1010_REG_ISP_X_WIN_CONTROL       0x3810
#define OV05C1010_REG_ISP_Y_WIN_CONTROL       0x3812

#define OV05C10_BAYER_PATTERN                 0x01 /* From ISP definition */
#define OV05C10_COLOR_DEPTH                   10 /* in bits */

#define OV05C10_WIDTH                         1928
#define OV05C10_HEIGHT                        1082

typedef int32_t (*OV05C10_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*OV05C10_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  OV05C10_Write_Func   WriteReg;
  OV05C10_Read_Func    ReadReg;
  void               *handle;
} ov05c10_ctx_t;

int32_t ov05c10_write_reg(ov05c10_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t ov05c10_read_reg(ov05c10_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* OV05C10_REG_H */

