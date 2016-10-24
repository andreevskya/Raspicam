#ifndef CAMCONTROL_H_
#define CAMCONTROL_H_

#define SHARPNESS_MIN -100
#define SHARPNESS_MAX 100
#define CONTRAST_MIN -100
#define CONTRAST_MAX 100
#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 100
#define SATURATION_MIN -100
#define SATURATION_MAX 100
#define EXPOSURE_COMPENSATION_MIN -10
#define EXPOSURE_COMPENSATION_MAX 10
#define ROTATION_MIN 0
#define ROTATION_MAX 359
#define SHUTTER_SPEED_AUTO 0
#define SHUTTER_SPEED_MAX 6000000

typedef struct
{
   int enable;
   int u,v;
} MMAL_PARAM_COLOURFX_T;

int raspicam_set_default_params(MMAL_COMPONENT_T * camera);

int raspicam_get_saturation(MMAL_COMPONENT_T * camera, int * saturation);
int raspicam_set_saturation(MMAL_COMPONENT_T * camera, int saturation);

int raspicam_get_sharpness(MMAL_COMPONENT_T * camera, int * sharpness);
int raspicam_set_sharpness(MMAL_COMPONENT_T * camera, int sharpness);

int raspicam_get_contrast(MMAL_COMPONENT_T * camera, int * contrast);
int raspicam_set_contrast(MMAL_COMPONENT_T * camera, int contrast);

int raspicam_get_brightness(MMAL_COMPONENT_T * camera, int * brightness);
int raspicam_set_brightness(MMAL_COMPONENT_T * camera, int brightness);

int raspicam_set_iso(MMAL_COMPONENT_T * camera, int iso);

int raspicam_get_metering_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_EXPOSUREMETERINGMODE_T * mode);
int raspicam_set_metering_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_EXPOSUREMETERINGMODE_T mode);

int raspicam_get_exposure_compensation(MMAL_COMPONENT_T * camera, int * compensation);
int raspicam_set_exposure_compensation(MMAL_COMPONENT_T *camera, int exp_comp);

int raspicam_get_exposure_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_EXPOSUREMODE_T * mode);
int raspicam_set_exposure_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_EXPOSUREMODE_T mode);

int raspicam_get_awb_mode(MMAL_COMPONENT_T * camera, MMAL_PARAM_AWBMODE_T * awb_mode);
int raspicam_set_awb_mode(MMAL_COMPONENT_T *camera, MMAL_PARAM_AWBMODE_T awb_mode);

int raspicam_set_awb_gains(MMAL_COMPONENT_T *camera, float r_gain, float b_gain);

int raspicam_get_imageFX(MMAL_COMPONENT_T * camera, MMAL_PARAM_IMAGEFX_T * effect);
int raspicam_set_imageFX(MMAL_COMPONENT_T *camera, MMAL_PARAM_IMAGEFX_T imageFX);

int raspicam_set_colourFX(MMAL_COMPONENT_T *camera, const MMAL_PARAM_COLOURFX_T *colourFX);

int raspicam_get_rotation(MMAL_COMPONENT_T * camera, int * rotation);
int raspicam_set_rotation(MMAL_COMPONENT_T *camera, int rotation);

int raspicam_get_flips(MMAL_COMPONENT_T * camera, int * hflip, int * vflip);
int raspicam_set_flips(MMAL_COMPONENT_T *camera, int hflip, int vflip);

int raspicam_get_ROI(MMAL_COMPONENT_T * camera, double * x, double * y, double * w, double * h);
int raspicam_set_ROI(MMAL_COMPONENT_T *camera, double x, double y, double w, double h);

int raspicam_get_shutter_speed(MMAL_COMPONENT_T * camera, int * shutterSpeed);
int raspicam_set_shutter_speed(MMAL_COMPONENT_T *camera, int speed);

int raspicam_set_DRC(MMAL_COMPONENT_T *camera, MMAL_PARAMETER_DRC_STRENGTH_T strength);

#endif /* CAMCONTROL_H_ */
