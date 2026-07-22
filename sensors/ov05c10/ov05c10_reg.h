#ifndef OV05C10_REG_H
#define OV05C10_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define OV05C10_NAME                          "OV05C10"

#define OV05C10_PAGE_ID                       0x00
#define OV05C10_REG_ID_BYTE_3                 0x00
#define OV05C10_REG_ID_BYTE_2                 0x01
#define OV05C10_REG_ID_BYTE_1                 0x02
#define OV05C10_REG_ID_BYTE_0                 0x03
#define OV05C10_CHIP_ID                       0x43055610

#define OV05C10_PAGE_MODE                     0x00
#define OV05C10_REG_MODE_SELECT               0x20
#define OV05C10_MODE_STANDBY                  0x5B
#define OV05C10_MODE_STREAMING                0x1F

#define OV05C10_PAGE_ANALOG                   0x01
#define OV05C10_REG_EXPOSURE                  0x03  // Coarse exposure (MSB) on page 0x01
#define OV05C10_REG_ANALOG_GAIN               0x24  // Analog gain register on page 0x01
#define OV05C10_REG_DIGITAL_GAIN              0x00  // Not used in current OVD sequence

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

#define OV05C10_REG_HTS                       0x37  /* page 0x01, read-only */
#define OV05C10_REG_HTS_MSB                   0x37  /* page 0x01, read-only */
#define OV05C10_REG_HTS_LSB                   0x38  /* page 0x01, read-only */
#define OV05C10_REG_VTS_MSB                   0x34  /* page 0x01, read-only */
#define OV05C10_REG_VTS_MID                   0x35  /* page 0x01, read-only */
#define OV05C10_REG_VTS_LSB                   0x36  /* page 0x01, read-only */
#define OV05C10_REG_VBLANK                    0x05  /* page 0x01, write */
#define OV05C10_REG_VBLANK_MSB                0x05  /* page 0x01, write */
#define OV05C10_REG_VBLANK_LSB                0x06  /* page 0x01, write */
#define OV05C10_REG_VBLANK_TRIGGER            0x01  /* page 0x01, write 0x01 */

#define OV05C10_REG_FORMAT                    0x01  /* page 0x01 */
#define OV05C10_PAGE_ISP_WINDOW               0x07
#define OV05C1010_REG_ISP_X_WIN_CONTROL       0x00  /* page 0x07 */
#define OV05C1010_REG_ISP_Y_WIN_CONTROL       0x01  /* page 0x07 */

#define OV05C10_BAYER_PATTERN                 0x01 /* From ISP definition */
#define OV05C10_COLOR_DEPTH                   10 /* in bits */

#define OV05C10_WIDTH                         2888
#define OV05C10_HEIGHT                        1808

typedef int32_t (*OV05C10_Write_Func)(void *, uint8_t, uint8_t*, uint16_t);
typedef int32_t (*OV05C10_Read_Func) (void *, uint8_t, uint8_t*, uint16_t);

typedef struct
{
  OV05C10_Write_Func   WriteReg;
  OV05C10_Read_Func    ReadReg;
  void               *handle;
} ov05c10_ctx_t;

/*
 * Bus access uses SCCB 8-bit register offsets.
 * Logical 16-bit register addresses must be translated to page (0xFD) + offset
 * before calling these raw accessors.
 */
int32_t ov05c10_write_reg(ov05c10_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint16_t length);
int32_t ov05c10_read_reg(ov05c10_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* OV05C10_REG_H */
