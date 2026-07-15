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
#define OV05C10_CHIP_ID                       0x43055610

#define OV05C10_REG_MODE_SELECT               0x0020
#define OV05C10_MODE_STANDBY                  0x5B
#define OV05C10_MODE_STREAMING                0x1F

#define OV05C10_REG_EXPOSURE                  0x0103  // Coarse exposure (MSB), 16-bit on page 0x01
#define OV05C10_REG_ANALOG_GAIN               0x0124  // Analog gain register on page 0x01
#define OV05C10_REG_DIGITAL_GAIN              0x0000  // Not used in current OVD sequence

#define OV05C10_EXPOSURE_MIN_LINES            (uint16_t)4       // min exposure in lines
#define OV05C10_EXPOSURE_MIN_VTS              (uint16_t)0x071E  // min vts from OVD (1822)
#define OV05C10_EXPOSURE_MAX_LINES_MARGIN     (uint16_t)15      // margin for max exposure
#define OV05C10_EXPOSURE_MAX_VTS              (uint16_t)0x071E  // max vts from OVD (1822)

#define OV05C10_EXPOSURE_MIN                  31       // us, approximate for 4 lines at OVD timing
#define OV05C10_EXPOSURE_MAX                  14309    // us, approximate for (VTS-margin) at OVD timing

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

#define OV05C10_REG_HTS                       0x0F00
#define OV05C10_REG_HTS_MSB                   0x0F00
#define OV05C10_REG_HTS_LSB                   0x0F01
#define OV05C10_REG_VTS                       0x0F02
#define OV05C10_REG_VTS_MSB                   0x0F02
#define OV05C10_REG_VTS_LSB                   0x0F03

#define OV05C10_REG_FORMAT                    0x0101
#define OV05C1010_REG_ISP_X_WIN_CONTROL       0x0700
#define OV05C1010_REG_ISP_Y_WIN_CONTROL       0x0701

#define OV05C10_BAYER_PATTERN                 0x01 /* From ISP definition */
#define OV05C10_COLOR_DEPTH                   10 /* in bits */

#define OV05C10_WIDTH                         2888
#define OV05C10_HEIGHT                        1808

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
