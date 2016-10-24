#include <interface/vcos/vcos.h>
#include <mmal.h>
#include <mmal_logging.h>
#include <mmal_buffer.h>
#include <util/mmal_util.h>
#include <util/mmal_util_params.h>
#include <util/mmal_default_components.h>
#include <util/mmal_connection.h>
#include <mmal_parameters_camera.h>
#include "camcontrol.h"

#define CONTROL_SUCCESS 1
#define CONTROL_FAILED 0

int mmal_status_to_int(MMAL_STATUS_T status) {
	if (status == MMAL_SUCCESS)
		return CONTROL_SUCCESS;
	else {
		switch (status) {
			case MMAL_ENOMEM: 	 vcos_log_error("Out of memory"); break;
			case MMAL_ENOSPC: 	 vcos_log_error("Out of resources (other than memory)"); break;
			case MMAL_EINVAL: 	 vcos_log_error("Argument is invalid"); break;
			case MMAL_ENOSYS: 	 vcos_log_error("Function not implemented"); break;
			case MMAL_ENOENT: 	 vcos_log_error("No such file or directory"); break;
			case MMAL_ENXIO:  	 vcos_log_error("No such device or address"); break;
			case MMAL_EIO:       vcos_log_error("I/O error"); break;
			case MMAL_ESPIPE:    vcos_log_error("Illegal seek"); break;
			case MMAL_ECORRUPT:  vcos_log_error("Data is corrupt \attention FIXME: not POSIX"); break;
			case MMAL_ENOTREADY: vcos_log_error("Component is not ready \attention FIXME: not POSIX"); break;
			case MMAL_ECONFIG:	 vcos_log_error("Component is not configured \attention FIXME: not POSIX"); break;
			case MMAL_EISCONN:   vcos_log_error("Port is already connected "); break;
			case MMAL_ENOTCONN:  vcos_log_error("Port is disconnected"); break;
			case MMAL_EAGAIN:    vcos_log_error("Resource temporarily unavailable. Try again later"); break;
			case MMAL_EFAULT:    vcos_log_error("Bad address"); break;
			default:             vcos_log_error("Unknown status error"); break;
		}
		return CONTROL_FAILED;
	}
}

int raspicam_get_saturation(MMAL_COMPONENT_T * camera, int * saturation) {
	MMAL_RATIONAL_T value;
	MMAL_STATUS_T status = mmal_port_parameter_get_rational(camera->control, MMAL_PARAMETER_SATURATION, &value);
	if( status == MMAL_SUCCESS)
		*saturation = value.num;
	return mmal_status_to_int(status);
}

int raspicam_set_saturation(MMAL_COMPONENT_T * camera, int saturation) {
	if(camera == NULL)
		return CONTROL_FAILED;
	if( saturation < SATURATION_MIN || saturation > SATURATION_MAX) {
		vcos_log_error("Invalid saturation value %d. Allowed values %d - %d", saturation, SATURATION_MIN, SATURATION_MAX);
		return CONTROL_FAILED;
	}
	MMAL_RATIONAL_T value = {saturation, SATURATION_MAX};
	return mmal_status_to_int(mmal_port_parameter_set_rational(camera->control, MMAL_PARAMETER_SATURATION, value));
}

int raspicam_get_sharpness(MMAL_COMPONENT_T * camera, int * sharpness) {
	MMAL_RATIONAL_T value;
	MMAL_STATUS_T status = mmal_port_parameter_get_rational(camera->control, MMAL_PARAMETER_SATURATION, &value);
	if(status == MMAL_SUCCESS)
		*sharpness = value.num;
	return mmal_status_to_int(status);
}

int raspicam_set_sharpness(MMAL_COMPONENT_T * camera, int sharpness) {
	if(camera == NULL) return CONTROL_FAILED;
	if(sharpness < SHARPNESS_MIN || sharpness > SHARPNESS_MAX) {
		vcos_log_error("Invalid sharpness value %d. Allowed values %d - %d", sharpness, SHARPNESS_MIN, SHARPNESS_MAX);
		return CONTROL_FAILED;
	}
	MMAL_RATIONAL_T value = {sharpness, SHARPNESS_MAX};
	return mmal_status_to_int(mmal_port_parameter_set_rational(camera->control, MMAL_PARAMETER_SHARPNESS, value));
}

int raspicam_get_contrast(MMAL_COMPONENT_T * camera, int * contrast) {
	MMAL_RATIONAL_T value;
	MMAL_STATUS_T status = mmal_port_parameter_get_rational(camera->control, MMAL_PARAMETER_CONTRAST, &value);
	if(status == MMAL_SUCCESS)
		*contrast = value.num;
	return mmal_status_to_int(status);
}

int raspicam_set_contrast(MMAL_COMPONENT_T * camera, int contrast) {
	if(camera == NULL) return CONTROL_FAILED;
	if(contrast < CONTRAST_MIN || contrast > CONTRAST_MAX) {
		vcos_log_error("Invalid contrast value %d. Allowed values %d - %d", contrast, CONTRAST_MIN, CONTRAST_MAX);
		return CONTROL_FAILED;
	}
	MMAL_RATIONAL_T value = {contrast, CONTRAST_MAX};
	return mmal_status_to_int(mmal_port_parameter_set_rational(camera->control, MMAL_PARAMETER_CONTRAST, value));
}

int raspicam_set_brightness(MMAL_COMPONENT_T * camera, int brightness) {
	if(camera == NULL) return CONTROL_FAILED;
	if(brightness < BRIGHTNESS_MIN || brightness > BRIGHTNESS_MAX) {
		vcos_log_error("Invalid brightness value %d. Allowed values %d - %d", brightness, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
		return CONTROL_FAILED;
	}
	MMAL_RATIONAL_T value = {brightness, BRIGHTNESS_MAX};
	return mmal_status_to_int(mmal_port_parameter_set_rational(camera->control, MMAL_PARAMETER_BRIGHTNESS, value));
}

int raspicam_get_brightness(MMAL_COMPONENT_T * camera, int * brightness) {
	MMAL_RATIONAL_T value;
	MMAL_STATUS_T status = mmal_port_parameter_get_rational (camera->control, MMAL_PARAMETER_BRIGHTNESS, &value);
	if(status == MMAL_SUCCESS)
		*brightness = value.num;
	return mmal_status_to_int(status);
}

int raspicam_set_iso(MMAL_COMPONENT_T * camera, int iso) {
	if(camera == NULL) return CONTROL_FAILED;
	return mmal_status_to_int(mmal_port_parameter_set_uint32(camera->control, MMAL_PARAMETER_ISO, iso));
}

int raspicam_get_metering_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_EXPOSUREMETERINGMODE_T * mode) {
	MMAL_PARAMETER_EXPOSUREMETERINGMODE_T meter_mode = {{MMAL_PARAMETER_EXP_METERING_MODE,sizeof(meter_mode)}, MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE};
	int ret = mmal_status_to_int(mmal_port_parameter_set(camera->control, &meter_mode.hdr));
	if(ret) {
		*mode = meter_mode.value;
	}
	return ret;
}

int raspicam_set_metering_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_EXPOSUREMETERINGMODE_T mode) {
	if (camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_EXPOSUREMETERINGMODE_T meter_mode = {{MMAL_PARAMETER_EXP_METERING_MODE,sizeof(meter_mode)}, mode};
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &meter_mode.hdr));
}

int raspicam_get_exposure_compensation(MMAL_COMPONENT_T * camera, int * compensation) {
	return mmal_status_to_int(mmal_port_parameter_get_int32(camera->control, MMAL_PARAMETER_EXPOSURE_COMP , compensation));
}

int raspicam_set_exposure_compensation(MMAL_COMPONENT_T *camera, int exp_comp) {
	if(camera == NULL) return CONTROL_FAILED;
	if(exp_comp < EXPOSURE_COMPENSATION_MIN || exp_comp > EXPOSURE_COMPENSATION_MAX) {
		vcos_log_error("Invalid exposure compensation value %d. Allowed values %d - %d",
			exp_comp,
			EXPOSURE_COMPENSATION_MIN,
			EXPOSURE_COMPENSATION_MAX
		);
		return CONTROL_FAILED;
	}
	return mmal_status_to_int(mmal_port_parameter_set_int32(camera->control, MMAL_PARAMETER_EXPOSURE_COMP , exp_comp));
}

int raspicam_get_exposure_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_EXPOSUREMODE_T * mode) {
	MMAL_PARAMETER_EXPOSUREMODE_T exp_mode = {{MMAL_PARAMETER_EXPOSURE_MODE,sizeof(exp_mode)}, MMAL_PARAM_EXPOSUREMODE_OFF};
	int ret = mmal_status_to_int(mmal_port_parameter_get(camera->control, &exp_mode.hdr));
	if(ret) {
		*mode = exp_mode.value;
	}
	return ret;
}

int raspicam_set_exposure_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_EXPOSUREMODE_T mode) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_EXPOSUREMODE_T exp_mode = {{MMAL_PARAMETER_EXPOSURE_MODE,sizeof(exp_mode)}, mode};
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &exp_mode.hdr));
}

int raspicam_get_awb_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_AWBMODE_T * awb_mode) {
	MMAL_PARAMETER_AWBMODE_T param = {{MMAL_PARAMETER_AWB_MODE,sizeof(param)}, *awb_mode};
	int ret = mmal_status_to_int(mmal_port_parameter_get(camera->control, &param.hdr));
	if( ret)
		*awb_mode = param.value;
	return ret;
}

int raspicam_set_awb_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_AWBMODE_T awb_mode) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_AWBMODE_T param = {{MMAL_PARAMETER_AWB_MODE,sizeof(param)}, awb_mode};
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &param.hdr));
}

int raspicam_set_awb_gains(MMAL_COMPONENT_T *camera, float r_gain, float b_gain) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS, sizeof(param)}, {0,0}, {0,0}};
	if(!r_gain || !b_gain)
		return CONTROL_FAILED;
	param.r_gain.num = (unsigned int)(r_gain * 65536);
	param.b_gain.num = (unsigned int)(b_gain * 65536);
	param.r_gain.den = param.b_gain.den = 65536;
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &param.hdr));
}

int raspicam_set_imageFX(MMAL_COMPONENT_T *camera, MMAL_PARAM_IMAGEFX_T imageFX) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_IMAGEFX_T imgFX = {{MMAL_PARAMETER_IMAGE_EFFECT, sizeof(imgFX)}, imageFX};
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &imgFX.hdr));
}

int raspicam_get_imageFX(MMAL_COMPONENT_T * camera, MMAL_PARAM_IMAGEFX_T * effect) {
	MMAL_PARAMETER_IMAGEFX_T imgFX = {{MMAL_PARAMETER_IMAGE_EFFECT, sizeof(imgFX)}, *effect};
	int ret = mmal_status_to_int(mmal_port_parameter_set(camera->control, &imgFX.hdr));
	*effect = imgFX.value;
	return ret;
}

int raspicam_set_colourFX(MMAL_COMPONENT_T *camera, const MMAL_PARAM_COLOURFX_T *colourFX) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT,sizeof(colfx)}, 0, 0, 0};
	colfx.enable = colourFX->enable;
	colfx.u = colourFX->u;
	colfx.v = colourFX->v;
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &colfx.hdr));
}

int raspicam_get_rotation(MMAL_COMPONENT_T * camera, int * rotation) {
	return mmal_status_to_int(mmal_port_parameter_get_int32(camera->output[0], MMAL_PARAMETER_ROTATION, rotation));
}

int raspicam_set_rotation(MMAL_COMPONENT_T *camera, int rotation) {
	if(camera == NULL) return CONTROL_FAILED;
	int ret;
	int my_rotation = ((rotation % 360 ) / 90) * 90;
	ret = mmal_status_to_int(mmal_port_parameter_set_int32(camera->output[0], MMAL_PARAMETER_ROTATION, my_rotation));
	ret &= mmal_status_to_int(mmal_port_parameter_set_int32(camera->output[1], MMAL_PARAMETER_ROTATION, my_rotation));
	ret &= mmal_status_to_int(mmal_port_parameter_set_int32(camera->output[2], MMAL_PARAMETER_ROTATION, my_rotation));
	return ret;
}

int raspicam_get_flips(MMAL_COMPONENT_T * camera, int * hflip, int *vflip) {
	MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, MMAL_PARAM_MIRROR_NONE};
	int ret = mmal_status_to_int(mmal_port_parameter_get(camera->output[0], &mirror.hdr));
	if(!ret) {
		return ret;
	}
	switch(mirror.value) {
		case MMAL_PARAM_MIRROR_BOTH:
			*hflip = 1;
			*vflip = 1;
			break;
		case MMAL_PARAM_MIRROR_HORIZONTAL:
			*hflip = 1;
			*vflip = 0;
			break;
		case MMAL_PARAM_MIRROR_VERTICAL:
			*hflip = 0;
			*vflip = 1;
			break;
		default:
			*hflip = 0;
			*vflip = 0;
			break;
	}
	return ret;
}

int raspicam_set_flips(MMAL_COMPONENT_T *camera, int hflip, int vflip) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, MMAL_PARAM_MIRROR_NONE};
	if (hflip && vflip)
		mirror.value = MMAL_PARAM_MIRROR_BOTH;
	else
		if (hflip)
			mirror.value = MMAL_PARAM_MIRROR_HORIZONTAL;
		else
			if (vflip)
				mirror.value = MMAL_PARAM_MIRROR_VERTICAL;
	mmal_port_parameter_set(camera->output[0], &mirror.hdr);
	mmal_port_parameter_set(camera->output[1], &mirror.hdr);
	return mmal_status_to_int(mmal_port_parameter_set(camera->output[2], &mirror.hdr));
}

int raspicam_get_ROI(MMAL_COMPONENT_T * camera, double * x, double * y, double * w, double * h) {
	MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
	int ret = mmal_status_to_int(mmal_port_parameter_get(camera->control, &crop.hdr));
	if(!ret) {
		*x = 0.0;
		*y = 0.0;
		*w = 1.0;
		*h = 1.0;
		return ret;
	}
	*x = crop.rect.x;
	*y = crop.rect.y;
	*w = crop.rect.width;
	*h = crop.rect.height;
	return ret;
}

int raspicam_set_ROI(MMAL_COMPONENT_T *camera, double x, double y, double w, double h) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
	crop.rect.x = (65536 * x);
	crop.rect.y = (65536 * y);
	crop.rect.width = (65536 * w);
	crop.rect.height = (65536 * h);
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &crop.hdr));
}


int raspicam_get_shutter_speed(MMAL_COMPONENT_T * camera, int * shutterSpeed) {
	return mmal_status_to_int(mmal_port_parameter_get_int32(camera->control, MMAL_PARAMETER_SHUTTER_SPEED, shutterSpeed));
}

int raspicam_set_shutter_speed(MMAL_COMPONENT_T *camera, int speed) {
	if(camera == NULL) return CONTROL_FAILED;
	return mmal_status_to_int(mmal_port_parameter_set_uint32(camera->control, MMAL_PARAMETER_SHUTTER_SPEED, speed));
}

int raspicam_set_DRC(MMAL_COMPONENT_T *camera, MMAL_PARAMETER_DRC_STRENGTH_T strength) {
	if(camera == NULL) return CONTROL_FAILED;
	MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}, strength};
	return mmal_status_to_int(mmal_port_parameter_set(camera->control, &drc.hdr));
}

int raspicam_set_default_params(MMAL_COMPONENT_T * camera) {
	raspicam_set_saturation(camera, 0);
	raspicam_set_sharpness(camera, 0);
	raspicam_set_contrast(camera, 0);
	raspicam_set_brightness(camera, 50);
	raspicam_set_iso(camera, 0);
	raspicam_set_metering_mode(camera, MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE);
	raspicam_set_exposure_compensation(camera, 0);
	raspicam_set_exposure_mode(camera, MMAL_PARAM_EXPOSUREMODE_AUTO);
	raspicam_set_awb_mode(camera, MMAL_PARAM_AWBMODE_AUTO);
	raspicam_set_awb_gains(camera, 0.0f, 0.0f);
	raspicam_set_imageFX(camera, MMAL_PARAM_IMAGEFX_NONE);
	raspicam_set_rotation(camera, 0);
	raspicam_set_flips(camera, 1, 1);
	raspicam_set_ROI(camera, 0.0, 0.0, 1.0, 1.0);
	raspicam_set_shutter_speed(camera, SHUTTER_SPEED_AUTO);
	raspicam_set_DRC(camera, MMAL_PARAMETER_DRC_STRENGTH_OFF);
	return CONTROL_SUCCESS;	
}
