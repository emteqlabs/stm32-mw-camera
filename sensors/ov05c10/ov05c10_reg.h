#ifndef OV05C10_REG_H
#define OV05C10_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define OV05C10_NAME                         "OV05C10"
#define OV05C10_CHIP_ID                      0x43055610U

#define OV05C10_PAGE_SYSTEM                  0x00U
#define OV05C10_PAGE_CIS                     0x01U
#define OV05C10_PAGE_ISP                     0x04U
#define OV05C10_REG_PAGE_SELECT              0xFDU

#define OV05C10_REG_CHIP_ID                  0x00U
#define OV05C10_REG_UPDATE                   0x01U
#define OV05C10_REG_EXPOSURE                 0x02U
#define OV05C10_REG_VTS                      0x34U
#define OV05C10_REG_DIGITAL_GAIN_H           0x21U
#define OV05C10_REG_DIGITAL_GAIN_L           0x22U
#define OV05C10_REG_ANALOG_GAIN              0x24U
#define OV05C10_REG_MIRROR_FLIP              0x32U

typedef int32_t (*OV05C10_Write_Func)(void *, uint8_t, uint8_t *, uint16_t);
typedef int32_t (*OV05C10_Read_Func)(void *, uint8_t, uint8_t *, uint16_t);

typedef struct
{
  OV05C10_Write_Func WriteReg;
  OV05C10_Read_Func ReadReg;
  void *handle;
} ov05c10_ctx_t;

int32_t ov05c10_read_reg(ov05c10_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t length);
int32_t ov05c10_write_reg(ov05c10_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* OV05C10_REG_H */
