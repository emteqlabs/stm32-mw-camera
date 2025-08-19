#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "cmw_ov2740.h"
#include "cmw_camera.h"
#include "ov2740_reg.h"
#include "ov2740.h"
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
#include "isp_param_conf.h"
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


static int CMW_OV2740_GetResType(uint32_t width, uint32_t height, uint32_t*res)
{
  if (width == OV2740_WIDTH && height == OV2740_HEIGHT)
  {
    *res = OV2740_RESOLUTION_1932_1092;
  }
  else
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  return 0;
}

static int32_t CMW_OV2740_getMirrorFlipConfig(uint32_t Config)
{
	int32_t ret;
	switch (Config)
	{
	case CMW_MIRRORFLIP_NONE:
	  ret = OV2740_MIRROR_FLIP_NONE;
	  break;
	case CMW_MIRRORFLIP_FLIP:
	  ret = OV2740_FLIP;
	  break;
	case CMW_MIRRORFLIP_MIRROR:
	  ret = OV2740_MIRROR;
	  break;
	case CMW_MIRRORFLIP_FLIP_MIRROR:
	default:
	  ret = OV2740_MIRROR_FLIP;
	  break;
	}
	return ret;
}

static int32_t CMW_OV2740_DeInit(void *io_ctx)
{
  int ret = CMW_ERROR_NONE;
  ret = ISP_DeInit(&((CMW_OV2740_t *)io_ctx)->hIsp);
  if (ret)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = OV2740_DeInit(&((CMW_OV2740_t *)io_ctx)->ctx_driver);
  if (ret)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  return ret;
}

static int32_t CMW_OV2740_ReadID(void *io_ctx, uint32_t *Id)
{
  return OV2740_ReadID(&((CMW_OV2740_t *)io_ctx)->ctx_driver, Id);
}

static int32_t CMW_OV2740_SetGain(void *io_ctx, int32_t gain)
{
  return OV2740_SetGain(&((CMW_OV2740_t *)io_ctx)->ctx_driver, gain);
}

static int32_t CMW_OV2740_SetExposure(void *io_ctx, int32_t exposure)
{
  return OV2740_SetExposure(&((CMW_OV2740_t *)io_ctx)->ctx_driver, exposure);
}

/**
  * @brief  Set the sensor white balance mode
  * @param  io_ctx  pointer to component object
  * @param  Automatic automatic mode enable/disable
  * @param  RefColorTemp color temperature if automatic mode is disabled
  * @retval Component status
  */
int32_t CMW_OV2740_SetWBRefMode(void *io_ctx, uint8_t Automatic, uint32_t RefColorTemp)
{
  int ret = CMW_ERROR_NONE;

  ret = ISP_SetWBRefMode(&((CMW_OV2740_t *)io_ctx)->hIsp, Automatic, RefColorTemp);
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
int32_t CMW_OV2740_ListWBRefModes(void *io_ctx, uint32_t RefColorTemp[])
{
  int ret = CMW_ERROR_NONE;

  ret = ISP_ListWBRefModes(&((CMW_OV2740_t *)io_ctx)->hIsp, RefColorTemp);
  if (ret)
  {
    return CMW_ERROR_PERIPH_FAILURE;
  }

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV2740_SetFrequency(void *io_ctx, int32_t frequency)
{
  UNUSED(io_ctx);
  UNUSED(frequency);
  return CMW_ERROR_FEATURE_NOT_SUPPORTED;
}

static int32_t CMW_OV2740_SetFramerate(void *io_ctx, int32_t framerate)
{
  UNUSED(io_ctx);
  UNUSED(framerate);
  return CMW_ERROR_FEATURE_NOT_SUPPORTED;
}

static int32_t CMW_OV2740_SetMirrorFlip(void *io_ctx, uint32_t config)
{
	int32_t mirrorFlip = CMW_OV2740_getMirrorFlipConfig(config);
	return OV2740_MirrorFlipConfig(&((CMW_OV2740_t *)io_ctx)->ctx_driver, mirrorFlip);
}

static int32_t CMW_OV2740_GetSensorInfo(void *io_ctx, ISP_SensorInfoTypeDef *info)
{
  if ((io_ctx ==  NULL) || (info == NULL))
  {
    return CMW_ERROR_WRONG_PARAM;
  }
  OV2740_SensorInfo_t sensor_info;
  OV2740_GetSensorInfo(&((CMW_OV2740_t *)io_ctx)->ctx_driver, &sensor_info);

  if (sizeof(info->name) >= strlen(OV2740_NAME) + 1)
  {
    strcpy(info->name, OV2740_NAME);
  }
  else
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  info->bayer_pattern = sensor_info.bayer_pattern;
  info->color_depth = sensor_info.color_depth;
  info->width = sensor_info.width;
  info->height = sensor_info.height;

  info->gain_min = sensor_info.gain_min;
  info->gain_max = sensor_info.gain_max;

  info->exposure_min = sensor_info.exposure_min;
  info->exposure_max = sensor_info.exposure_max;

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV2740_Init(void *io_ctx, CMW_Sensor_Init_t *initSensor)
{
  int ret = CMW_ERROR_NONE;
  uint32_t resolution;

  ret = CMW_OV2740_GetResType(initSensor->width, initSensor->height, &resolution);
  if (ret)
  {
    return CMW_ERROR_WRONG_PARAM;
  }

  ret = OV2740_Init(&((CMW_OV2740_t *)io_ctx)->ctx_driver, resolution, initSensor->pixel_format);
  if (ret != OV2740_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  return CMW_ERROR_NONE;
}

static int32_t CMW_OV2740_Start(void *io_ctx)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  int ret;
  /* Statistic area is provided with null value so that it force the ISP Library to get the statistic
   * area information from the tuning file.
   */
  (void) ISP_IQParamCacheInit; /* unused */
  ret = ISP_Init(&((CMW_OV2740_t *)io_ctx)->hIsp, ((CMW_OV2740_t *)io_ctx)->hdcmipp, 0, &((CMW_OV2740_t *)io_ctx)->appliHelpers, &ISP_IQParamCacheInit_OV2740);
  if (ret != ISP_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = ISP_Start(&((CMW_OV2740_t *)io_ctx)->hIsp);
  if (ret != ISP_OK)
  {
      return CMW_ERROR_PERIPH_FAILURE;
  }
#endif
  return OV2740_Start(&((CMW_OV2740_t *)io_ctx)->ctx_driver);
}

static int32_t CMW_OV2740_Run(void *io_ctx)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  int ret;
  ret = ISP_BackgroundProcess(&((CMW_OV2740_t *)io_ctx)->hIsp);
  if (ret != ISP_OK)
  {
      return CMW_ERROR_PERIPH_FAILURE;
  }
#endif
  return CMW_ERROR_NONE;
}

static void CMW_OV2740_PowerOn(CMW_OV2740_t *io_ctx)
{
  io_ctx->ShutdownPin(0);   // xshutdown2 should be 0 before power on
  io_ctx->XShutdownPin(0);   // xshutdown should be 0 before power on
  io_ctx->Delay(10);
  io_ctx->EnablePin(0);     // power off
  io_ctx->Delay(10);

  io_ctx->EnablePin(1);     // power on
  io_ctx->Delay(20);
  io_ctx->ShutdownPin(1);   // xshutdown2 should be 1 after power on
  io_ctx->XShutdownPin(1);   // xshutdown should be 1 before power on
  io_ctx->Delay(20);
}

static void CMW_OV2740_VsyncEventCallback(void *io_ctx, uint32_t pipe)
{
#ifndef ISP_MW_TUNING_TOOL_SUPPORT
  /* Update the ISP frame counter and call its statistics handler */
  switch (pipe)
  {
    case DCMIPP_PIPE0 :
      ISP_IncDumpFrameId(&((CMW_OV2740_t *)io_ctx)->hIsp);
      break;
    case DCMIPP_PIPE1 :
      ISP_IncMainFrameId(&((CMW_OV2740_t *)io_ctx)->hIsp);
      ISP_GatherStatistics(&((CMW_OV2740_t *)io_ctx)->hIsp);
      break;
    case DCMIPP_PIPE2 :
      ISP_IncAncillaryFrameId(&((CMW_OV2740_t *)io_ctx)->hIsp);
      break;
  }
#endif
}

static void CMW_OV2740_FrameEventCallback(void *io_ctx, uint32_t pipe)
{
  UNUSED(io_ctx);
  UNUSED(pipe);
}

int CMW_OV2740_Probe(CMW_OV2740_t *io_ctx, CMW_Sensor_if_t *ov2740_if)
{
  int ret = CMW_ERROR_NONE;
  uint32_t id;
  io_ctx->ctx_driver.IO.Address = io_ctx->Address;
  io_ctx->ctx_driver.IO.Init = io_ctx->Init;
  io_ctx->ctx_driver.IO.DeInit = io_ctx->DeInit;
  io_ctx->ctx_driver.IO.GetTick = io_ctx->GetTick;
  io_ctx->ctx_driver.IO.ReadReg = io_ctx->ReadReg;
  io_ctx->ctx_driver.IO.WriteReg = io_ctx->WriteReg;

  CMW_OV2740_PowerOn(io_ctx);

  ret = OV2740_RegisterBusIO(&io_ctx->ctx_driver, &io_ctx->ctx_driver.IO);
  if (ret != OV2740_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }

  ret = OV2740_ReadID(&io_ctx->ctx_driver, &id);
  if (ret != OV2740_OK)
  {
    return CMW_ERROR_COMPONENT_FAILURE;
  }
  if (id != OV2740_CHIP_ID)
  {
      ret = CMW_ERROR_UNKNOWN_COMPONENT;
  }

  memset(ov2740_if, 0, sizeof(*ov2740_if));
  ov2740_if->Init = CMW_OV2740_Init;
  ov2740_if->Start = CMW_OV2740_Start;
  ov2740_if->DeInit = CMW_OV2740_DeInit;
  ov2740_if->Run = CMW_OV2740_Run;
  ov2740_if->VsyncEventCallback = CMW_OV2740_VsyncEventCallback;
  ov2740_if->FrameEventCallback = CMW_OV2740_FrameEventCallback;
  ov2740_if->ReadID = CMW_OV2740_ReadID;
  ov2740_if->SetGain = CMW_OV2740_SetGain;
  ov2740_if->SetExposure = CMW_OV2740_SetExposure;
  ov2740_if->SetWBRefMode = CMW_OV2740_SetWBRefMode;
  ov2740_if->ListWBRefModes = CMW_OV2740_ListWBRefModes;
  ov2740_if->SetFrequency = CMW_OV2740_SetFrequency;
  ov2740_if->SetFramerate = CMW_OV2740_SetFramerate;
  ov2740_if->SetMirrorFlip = CMW_OV2740_SetMirrorFlip;
  ov2740_if->GetSensorInfo = CMW_OV2740_GetSensorInfo;
  return ret;
}

