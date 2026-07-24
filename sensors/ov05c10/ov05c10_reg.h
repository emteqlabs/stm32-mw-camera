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
#define OV05C10_REG_EXPOSURE_MSB              0x02
#define OV05C10_REG_EXPOSURE_MID              0x03
#define OV05C10_REG_EXPOSURE_LSB              0x04
#define OV05C10_REG_ANALOG_GAIN               0x24
#define OV05C10_REG_DIGITAL_GAIN_MSB          0x21
#define OV05C10_REG_DIGITAL_GAIN_LSB          0x22
#define OV05C10_REG_ORIENTATION               0x32

#define OV05C10_ORIENTATION_MIRROR            0x01U
#define OV05C10_ORIENTATION_FLIP              0x02U

#define OV05C10_EXPOSURE_MIN_LINES            6U
#define OV05C10_EXPOSURE_MAX_LINES_MARGIN     33U

#define OV05C10_ANALOG_GAIN_MAX_LINEAR        15.5f
#define OV05C10_ANALOG_GAIN_MAX_DBM           23807
#define OV05C10_ANALOG_GAIN_MIN_REG           0x10U
#define OV05C10_ANALOG_GAIN_MAX_REG           0xF8U
#define OV05C10_DIGITAL_GAIN_MIN_REG          0x0040U
#define OV05C10_DIGITAL_GAIN_MAX_REG          0x0100U
#define OV05C10_GAIN_MIN                      0
#define OV05C10_GAIN_MAX                      35848

#define OV05C10_REG_VTS_MSB                   0x34  /* page 0x01, read-only */
#define OV05C10_REG_VTS_MID                   0x35  /* page 0x01, read-only */
#define OV05C10_REG_VTS_LSB                   0x36  /* page 0x01, read-only */
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
