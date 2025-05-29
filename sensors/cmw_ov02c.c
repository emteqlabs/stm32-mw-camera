#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "cmw_ov02c.h"
#include "cmw_camera.h"
#include "ov02c_reg.h"
#include "ov02c.h"
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
#include "isp_param_conf.h"
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


static int CMW_OV02C_GetResType(uint32_t width, uint32_t height, uint32_t*res)
{
  if (width == 1920 && height == 1080)
  {
    *res = OV02C_RESOLUTION_1920_1080;
  }
  else
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  return 0;
}

static int32_t CMW_OV02C_getMirrorFlipConfig(uint32_t Config)
{
  // NOT IMPLEMENTED YET
  int32_t ret = 0;
  return ret;
}

static int32_t CMW_OV02C_DeInit(void *io_ctx)
{
  int ret = CMW_ERROR_NONE;
  ret = ISP_DeInit(&((CMW_OV02C_t *)io_ctx)->hIsp);
  if (ret)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = OV02C_DeInit(&((CMW_OV02C_t *)io_ctx)->ctx_driver);
  if (ret)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  return ret;
}

static int32_t CMW_OV02C_ReadID(void *io_ctx, uint32_t *Id)
{
  return OV02C_ReadID(&((CMW_OV02C_t *)io_ctx)->ctx_driver, Id);
}

static int32_t CMW_OV02C_SetGain(void *io_ctx, int32_t gain)
{
  return OV02C_SetGain(&((CMW_OV02C_t *)io_ctx)->ctx_driver, gain);
}

static int32_t CMW_OV02C_SetExposure(void *io_ctx, int32_t exposure)
{
  return IMX335_SetExposure(&((CMW_OV02C_t *)io_ctx)->ctx_driver, exposure);
}

/**
  * @brief  Set the sensor white balance mode
  * @param  io_ctx  pointer to component object
  * @param  Automatic automatic mode enable/disable
  * @param  RefColorTemp color temperature if automatic mode is disabled
  * @retval Component status
  */
int32_t CMW_OV02C_SetWBRefMode(void *io_ctx, uint8_t Automatic, uint32_t RefColorTemp)
{
  int ret = CMW_ERROR_NONE;

  ret = ISP_SetWBRefMode(&((CMW_OV02C_t *)io_ctx)->hIsp, Automatic, RefColorTemp);
  if (ret)
  {
    return CMW_ERROR_PERIPH_FAILURE;
  }

  return CMW_ERROR_NONE;
}

/**
  * @brief  List the sensor white balance modes
  * @param  io_ctx  pointer to component object
  * @param  RefColorTemp color temperature list
  * @retval Component status
  */
int32_t CMW_OV02C_ListWBRefModes(void *io_ctx, uint32_t RefColorTemp[])
{
  int ret = CMW_ERROR_NONE;

  ret = ISP_ListWBRefModes(&((CMW_OV02C_t *)io_ctx)->hIsp, RefColorTemp);
  if (ret)
  {
    return CMW_ERROR_PERIPH_FAILURE;
  }

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV02C_SetFrequency(void *io_ctx, int32_t frequency)
{
  return CMW_ERROR_FEATURE_NOT_SUPPORTED;
}

static int32_t CMW_OV02C_SetFramerate(void *io_ctx, int32_t framerate)
{
  return CMW_ERROR_FEATURE_NOT_SUPPORTED;
}

static int32_t CMW_OV02C_SetMirrorFlip(void *io_ctx, uint32_t config)
{
  int32_t mirrorFlip = CMW_OV02C_getMirrorFlipConfig(config);
  return IMX335_MirrorFlipConfig(&((CMW_OV02C_t *)io_ctx)->ctx_driver, mirrorFlip);
}

static int32_t CMW_OV02C_GetSensorInfo(void *io_ctx, ISP_SensorInfoTypeDef *info)
{
  if ((io_ctx ==  NULL) || (info == NULL))
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  if (sizeof(info->name) >= strlen(OV02C_NAME) + 1)
  {
    strcpy(info->name, OV02C_NAME);
  }
  else
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  info->bayer_pattern = OV02C_BAYER_PATTERN;
  info->color_depth = OV02C_COLOR_DEPTH;
  info->width = OV02C_WIDTH;
  info->height = OV02C_HEIGHT;

  // TODO GAIN
  info->gain_min = OV02C_ANALOG_GAIN_MIN;
  info->gain_max = OV02C_ANALOG_GAIN_MAX;

  info->exposure_min = OV02C_EXPOSURE_MIN;
  info->exposure_max = OV02C_EXPOSURE_MAX;

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV02C_SetTestPattern(void *io_ctx, int32_t mode)
{
  return OV02C_SetTestPattern(&((CMW_OV02C_t *)io_ctx)->ctx_driver, mode);
}

static int32_t CMW_OV02C_Init(void *io_ctx, CMW_Sensor_Init_t *initSensor)
{
  int ret = CMW_ERROR_NONE;
  uint32_t resolution;

  ret = CMW_OV02C_GetResType(initSensor->width, initSensor->height, &resolution);
  if (ret)
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  ret = CMW_OV02C_SetMirrorFlip(io_ctx, initSensor->mirrorFlip);
  if (ret)
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  ret = OV02C_Init(&((CMW_OV02C_t *)io_ctx)->ctx_driver, resolution, initSensor->pixel_format);
  if (ret != OV02C_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV02C_Start(void *io_ctx)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  int ret;
  /* Statistic area is provided with null value so that it force the ISP Library to get the statistic
   * area information from the tuning file.
   */
  (void) ISP_IQParamCacheInit; /* unused */
  ret = ISP_Init(&((CMW_OV02C_t *)io_ctx)->hIsp, ((CMW_OV02C_t *)io_ctx)->hdcmipp, 0, &((CMW_OV02C_t *)io_ctx)->appliHelpers, &ISP_IQParamCacheInit_OV02C);
  if (ret != ISP_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = ISP_Start(&((CMW_OV02C_t *)io_ctx)->hIsp);
  if (ret != ISP_OK)
  {
      return CMW_ERROR_PERIPH_FAILURE;
  }
#endif
  return OV02C_Start(&((CMW_OV02C_t *)io_ctx)->ctx_driver);
}

static int32_t CMW_OV02C_Run(void *io_ctx)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  int ret;
  ret = ISP_BackgroundProcess(&((CMW_OV02C_t *)io_ctx)->hIsp);
  if (ret != ISP_OK)
  {
      return CMW_ERROR_PERIPH_FAILURE;
  }
#endif
  return CMW_ERROR_NONE;
}

static void CMW_OV02C_PowerOn(CMW_OV02C_t *io_ctx)
{
  io_ctx->ShutdownPin(0);   // xshutdown should be 0 before power on
  io_ctx->Delay(100);
  io_ctx->EnablePin(0);     // power off
  io_ctx->Delay(100);

  io_ctx->EnablePin(1);     // power on
  io_ctx->Delay(100);
  io_ctx->ShutdownPin(1);   // xshutdown should be 1 after power on
  io_ctx->Delay(100);
}

static void CMW_OV02C_VsyncEventCallback(void *io_ctx, uint32_t pipe)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  /* Update the ISP frame counter and call its statistics handler */
  switch (pipe)
  {
    case DCMIPP_PIPE0 :
      ISP_IncDumpFrameId(&((CMW_OV02C_t *)io_ctx)->hIsp);
      break;
    case DCMIPP_PIPE1 :
      ISP_IncMainFrameId(&((CMW_OV02C_t *)io_ctx)->hIsp);
      ISP_GatherStatistics(&((CMW_OV02C_t *)io_ctx)->hIsp);
      break;
    case DCMIPP_PIPE2 :
      ISP_IncAncillaryFrameId(&((CMW_OV02C_t *)io_ctx)->hIsp);
      break;
  }
#endif
}

static void CMW_OV02C_FrameEventCallback(void *io_ctx, uint32_t pipe)
{
}

int CMW_OV02C_Probe(CMW_OV02C_t *io_ctx, CMW_Sensor_if_t *ov02c_if)
{
  int ret = CMW_ERROR_NONE;
  uint32_t id;
  io_ctx->ctx_driver.IO.Address = io_ctx->Address;
  io_ctx->ctx_driver.IO.Init = io_ctx->Init;
  io_ctx->ctx_driver.IO.DeInit = io_ctx->DeInit;
  io_ctx->ctx_driver.IO.GetTick = io_ctx->GetTick;
  io_ctx->ctx_driver.IO.ReadReg = io_ctx->ReadReg;
  io_ctx->ctx_driver.IO.WriteReg = io_ctx->WriteReg;

  CMW_OV02C_PowerOn(io_ctx);

  ret = OV02C_RegisterBusIO(&io_ctx->ctx_driver, &io_ctx->ctx_driver.IO);
  if (ret != OV02C_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = OV02C_ReadID(&io_ctx->ctx_driver, &id);
  if (ret != OV02C_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  if (id != OV02C_CHIP_ID)
  {
      ret = CMW_ERROR_UNKNOWN_COMPONENT;
  }

  memset(ov02c_if, 0, sizeof(*ov02c_if));
  ov02c_if->Init = CMW_OV02C_Init;
  ov02c_if->Start = CMW_OV02C_Start;
  ov02c_if->DeInit = CMW_OV02C_DeInit;
  ov02c_if->Run = CMW_OV02C_Run;
  ov02c_if->VsyncEventCallback = CMW_OV02C_VsyncEventCallback;
  ov02c_if->FrameEventCallback = CMW_OV02C_FrameEventCallback;
  ov02c_if->ReadID = CMW_OV02C_ReadID;
  ov02c_if->SetGain = CMW_OV02C_SetGain;
  ov02c_if->SetExposure = CMW_OV02C_SetExposure;
  ov02c_if->SetWBRefMode = CMW_OV02C_SetWBRefMode;
  ov02c_if->ListWBRefModes = CMW_OV02C_ListWBRefModes;
  ov02c_if->SetFrequency = CMW_OV02C_SetFrequency;
  ov02c_if->SetFramerate = CMW_OV02C_SetFramerate;
  ov02c_if->SetMirrorFlip = CMW_OV02C_SetMirrorFlip;
  ov02c_if->GetSensorInfo = CMW_OV02C_GetSensorInfo;
  ov02c_if->SetTestPattern = CMW_OV02C_SetTestPattern;
  return ret;
}

