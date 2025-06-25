#include "ov2740_reg.h"

int32_t ov2740_read_reg(ov2740_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->ReadReg(ctx->handle, reg, pdata, length);
}

int32_t ov2740_write_reg(ov2740_ctx_t *ctx, uint16_t reg, uint8_t *data, uint16_t length)
{
  return ctx->WriteReg(ctx->handle, reg, data, length);
}
