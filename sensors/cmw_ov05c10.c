#include "cmw_ov05c10.h"

#include <assert.h>
#include <string.h>

#define OV05C10_PHY_BITRATE_BPS 1800000000

static int32_t CMW_OV05C10_GetSensorInfo(void *io_ctx, ISP_SensorInfoTypeDef *info)
{
  CMW_OV05C10_t *ctx = (CMW_OV05C10_t *)io_ctx;
  uint32_t vts;
  uint64_t denominator;

  if ((ctx == NULL) || (info == NULL))
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  if (sizeof(info->name) < sizeof(OV05C10_NAME))
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  strcpy(info->name, OV05C10_NAME);
  info->bayer_pattern = OV05C10_BAYER_PATTERN;
  info->color_depth = 10U;
  info->width = OV05C10_WIDTH;
  info->height = OV05C10_HEIGHT;
  info->gain_min = OV05C10_GAIN_MIN_MDB;
  info->gain_max = OV05C10_GAIN_MAX_MDB;
  info->again_max = OV05C10_ANALOG_GAIN_MAX_MDB;

  vts = (ctx->ctx_driver.Vts != 0U) ? ctx->ctx_driver.Vts : OV05C10_DEFAULT_VTS;
  denominator = (uint64_t)OV05C10_FPS * vts;
  info->exposure_min = (uint32_t)(((uint64_t)OV05C10_EXPOSURE_MIN_LINES * 1000000ULL +
                                  denominator / 2ULL) / denominator);
  info->exposure_max = (uint32_t)(((uint64_t)(vts - OV05C10_EXPOSURE_MARGIN_LINES) * 1000000ULL +
                                  denominator / 2ULL) / denominator);
  return CMW_ERROR_NONE;
}

static int32_t CMW_OV05C10_GetResolution(uint32_t width, uint32_t height, uint32_t *resolution)
{
  if ((resolution == NULL) || (width != OV05C10_WIDTH) || (height != OV05C10_HEIGHT))
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  *resolution = 0U;
  return CMW_ERROR_NONE;
}

static int32_t CMW_OV05C10_Init(void *io_ctx, CMW_Sensor_Init_t *init_sensor)
{
  CMW_OV05C10_t *ctx = (CMW_OV05C10_t *)io_ctx;
  CMW_OV05C10_config_t *config;
  OV05C10_MirrorFlip_t mirror_flip;
  uint32_t resolution;

  if ((ctx == NULL) || (init_sensor == NULL) || (init_sensor->sensor_config == NULL))
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  if ((init_sensor->fps != (int)OV05C10_FPS) ||
      (CMW_OV05C10_GetResolution(init_sensor->width, init_sensor->height, &resolution) != CMW_ERROR_NONE))
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  config = (CMW_OV05C10_config_t *)init_sensor->sensor_config;
  if ((config->pixel_format != CMW_PIXEL_FORMAT_DEFAULT) &&
      (config->pixel_format != CMW_PIXEL_FORMAT_RAW10))
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  if (OV05C10_Init(&ctx->ctx_driver, resolution, OV05C10_RAW10) != OV05C10_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  switch (init_sensor->mirrorFlip)
  {
    case CMW_MIRRORFLIP_NONE:
      mirror_flip = OV05C10_MIRROR_FLIP_NONE;
      break;
    case CMW_MIRRORFLIP_FLIP:
      mirror_flip = OV05C10_FLIP;
      break;
    case CMW_MIRRORFLIP_MIRROR:
      mirror_flip = OV05C10_MIRROR;
      break;
    case CMW_MIRRORFLIP_FLIP_MIRROR:
      mirror_flip = OV05C10_MIRROR_FLIP;
      break;
    default:
      return CMW_ERROR_WRONG_PARAM;
  }

  return (OV05C10_MirrorFlipConfig(&ctx->ctx_driver, mirror_flip) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_DeInit(void *io_ctx)
{
  return (OV05C10_DeInit(&((CMW_OV05C10_t *)io_ctx)->ctx_driver) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_Start(void *io_ctx)
{
  return (OV05C10_Start(&((CMW_OV05C10_t *)io_ctx)->ctx_driver) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_Stop(void *io_ctx)
{
  return (OV05C10_Stop(&((CMW_OV05C10_t *)io_ctx)->ctx_driver) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_ReadID(void *io_ctx, uint32_t *id)
{
  return (OV05C10_ReadID(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, id) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_SetGain(void *io_ctx, int32_t gain)
{
  return (OV05C10_SetGain(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, gain) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_SetExposure(void *io_ctx, int32_t exposure)
{
  return (OV05C10_SetExposure(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, exposure) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_GetAppliedGain(void *io_ctx, int32_t *gain)
{
  return (OV05C10_GetAppliedGain(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, gain) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_GetAppliedExposure(void *io_ctx, int32_t *exposure)
{
  return (OV05C10_GetAppliedExposure(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, exposure) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_SetMirrorFlip(void *io_ctx, uint32_t config)
{
  OV05C10_MirrorFlip_t mirror_flip;

  switch (config)
  {
    case CMW_MIRRORFLIP_NONE:
      mirror_flip = OV05C10_MIRROR_FLIP_NONE;
      break;
    case CMW_MIRRORFLIP_FLIP:
      mirror_flip = OV05C10_FLIP;
      break;
    case CMW_MIRRORFLIP_MIRROR:
      mirror_flip = OV05C10_MIRROR;
      break;
    case CMW_MIRRORFLIP_FLIP_MIRROR:
      mirror_flip = OV05C10_MIRROR_FLIP;
      break;
    default:
      return CMW_ERROR_WRONG_PARAM;
  }
  return (OV05C10_MirrorFlipConfig(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, mirror_flip) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_SetTestPattern(void *io_ctx, int32_t mode)
{
  return (OV05C10_SetTestPattern(&((CMW_OV05C10_t *)io_ctx)->ctx_driver, mode) == OV05C10_OK) ?
         CMW_ERROR_NONE : CMW_ERROR_COMPONENT_FAILURE;
}

static int32_t CMW_OV05C10_GetDefaultPHYBitrate(void *io_ctx, int32_t *bitrate)
{
  (void)io_ctx;
  if (bitrate == NULL)
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  *bitrate = OV05C10_PHY_BITRATE_BPS;
  return CMW_ERROR_NONE;
}

static void CMW_OV05C10_PowerOn(CMW_OV05C10_t *io_ctx)
{
  io_ctx->ShutdownPin(0);
  io_ctx->Delay(5U);
  io_ctx->ShutdownPin(1);
  io_ctx->Delay(8U);
}

void CMW_OV05C10_SetDefaultSensorValues(CMW_OV05C10_config_t *config)
{
  assert(config != NULL);
  config->pixel_format = CMW_PIXEL_FORMAT_RAW10;
}

int CMW_OV05C10_Probe(CMW_OV05C10_t *io_ctx, CMW_Sensor_if_t *sensor_if)
{
  uint32_t id;

  if ((io_ctx == NULL) || (sensor_if == NULL) || (io_ctx->ShutdownPin == NULL) ||
      (io_ctx->Delay == NULL))
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  io_ctx->ctx_driver.IO.Address = io_ctx->Address;
  io_ctx->ctx_driver.IO.Init = io_ctx->Init;
  io_ctx->ctx_driver.IO.DeInit = io_ctx->DeInit;
  io_ctx->ctx_driver.IO.GetTick = io_ctx->GetTick;
  io_ctx->ctx_driver.IO.ReadReg = io_ctx->ReadReg;
  io_ctx->ctx_driver.IO.WriteReg = io_ctx->WriteReg;

  CMW_OV05C10_PowerOn(io_ctx);
  if (OV05C10_RegisterBusIO(&io_ctx->ctx_driver, &io_ctx->ctx_driver.IO) != OV05C10_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  if (OV05C10_ReadID(&io_ctx->ctx_driver, &id) != OV05C10_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  if (id != OV05C10_CHIP_ID)
  {
    return CMW_ERROR_UNKNOWN_COMPONENT;
  }

  memset(sensor_if, 0, sizeof(*sensor_if));
  sensor_if->Init = CMW_OV05C10_Init;
  sensor_if->DeInit = CMW_OV05C10_DeInit;
  sensor_if->Start = CMW_OV05C10_Start;
  sensor_if->Stop = CMW_OV05C10_Stop;
  sensor_if->ReadID = CMW_OV05C10_ReadID;
  sensor_if->SetGain = CMW_OV05C10_SetGain;
  sensor_if->SetExposure = CMW_OV05C10_SetExposure;
  sensor_if->GetAppliedGain = CMW_OV05C10_GetAppliedGain;
  sensor_if->GetAppliedExposure = CMW_OV05C10_GetAppliedExposure;
  sensor_if->SetMirrorFlip = CMW_OV05C10_SetMirrorFlip;
  sensor_if->GetSensorInfo = CMW_OV05C10_GetSensorInfo;
  sensor_if->SetTestPattern = CMW_OV05C10_SetTestPattern;
  sensor_if->GetDefaultPHYBitrate = CMW_OV05C10_GetDefaultPHYBitrate;
  return CMW_ERROR_NONE;
}
