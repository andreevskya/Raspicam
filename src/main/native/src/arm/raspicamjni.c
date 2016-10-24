#include <stdio.h>
#include <jni.h>
#include <mmal.h>
#include <interface/vcos/vcos.h>
#include "raspicam.h"
#include "camcontrol.h"
#include "raspicamjni.h"

#define JNI_TRUE 1
#define JNI_FALSE 0
#define JAVA_MMAL_STATUS_ENUM "Lru/andreevskya/raspberrypi/raspicam/Status;"
#define JAVA_MMAL_IMAGE_EFFECT_ENUM "Lru/andreevskya/raspberrypi/raspicam/ImageEffect;"
#define JAVA_MMAL_AWB_MODE_ENUM "Lru/andreevskya/raspberrypi/raspicam/AutoWhiteBalance;"
#define JAVA_MAL_EXPOSURE_METERING_MODE_ENUM "Lru/andreevskya/raspberrypi/raspicam/ExposureMeteringMode;"
#define JAVA_FLIP_ENUM "Lru/andreevskya/raspberrypi/raspicam/Flip;"
#define JAVA_MMAL_EXPOSURE_MODE_ENUM "Lru/andreevskya/raspberrypi/raspicam/ExposureMode;"

CAM_DATA camera = {0};

jobject mmal_status_to_java_enum(JNIEnv * env, MMAL_STATUS_T status) {
	jclass mmalStatusCls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/Status");
 	jfieldID fid;
	switch(status) {
		case MMAL_SUCCESS:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "SUCCESS", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOMEM:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOMEM", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOSPC:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOSPC", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_EINVAL:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "EINVAL", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOSYS:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOSYS", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOENT:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOENT", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENXIO:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENXIO", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_EIO:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "EIO", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ESPIPE:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ESPIPE", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ECORRUPT:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ECORRUPT", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOTREADY:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOTREADY", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ECONFIG:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ECONFIG", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_EISCONN:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "EISCONN", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_ENOTCONN:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "ENOTCONN", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_EAGAIN:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "EAGAIN", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_EFAULT:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "EFAULT", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		case MMAL_STATUS_MAX:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "STATUS_MAX", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
		default:
			fid = (*env)->GetStaticFieldID(env, mmalStatusCls, "UNKNOWN", JAVA_MMAL_STATUS_ENUM);
			return (*env)->GetStaticObjectField(env, mmalStatusCls, fid);
	}
}

jobject mmal_image_effect_to_java_enum(JNIEnv * env, MMAL_PARAM_IMAGEFX_T effect) {
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ImageEffect");
 	jfieldID fid;
	switch(effect) {
		case MMAL_PARAM_IMAGEFX_NONE:
			fid = (*env)->GetStaticFieldID(env, cls, "NONE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_NEGATIVE:
			fid = (*env)->GetStaticFieldID(env, cls, "NEGATIVE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_SOLARIZE:
			fid = (*env)->GetStaticFieldID(env, cls, "SOLARIZE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_SKETCH:
			fid = (*env)->GetStaticFieldID(env, cls, "SKETCH", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_DENOISE:
			fid = (*env)->GetStaticFieldID(env, cls, "DENOISE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_EMBOSS:
			fid = (*env)->GetStaticFieldID(env, cls, "EMBOSS", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_OILPAINT:
			fid = (*env)->GetStaticFieldID(env, cls, "OILPAINT", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_HATCH:
			fid = (*env)->GetStaticFieldID(env, cls, "HATCH", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_GPEN:
			fid = (*env)->GetStaticFieldID(env, cls, "GPEN", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_PASTEL:
			fid = (*env)->GetStaticFieldID(env, cls, "PASTEL", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_WATERCOLOUR:
			fid = (*env)->GetStaticFieldID(env, cls, "WATERCOLOR", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_FILM:
			fid = (*env)->GetStaticFieldID(env, cls, "FILM", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_BLUR:
			fid = (*env)->GetStaticFieldID(env, cls, "BLUR", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_SATURATION:
			fid = (*env)->GetStaticFieldID(env, cls, "SATURATION", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_COLOURSWAP:
			fid = (*env)->GetStaticFieldID(env, cls, "COLORSWAP", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_WASHEDOUT:
			fid = (*env)->GetStaticFieldID(env, cls, "WASHEDOUT", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_POSTERIZE:
			fid = (*env)->GetStaticFieldID(env, cls, "POSTERIZE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_COLOURPOINT:
			fid = (*env)->GetStaticFieldID(env, cls, "COLORPOINT", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_COLOURBALANCE:
			fid = (*env)->GetStaticFieldID(env, cls, "COLORBALANCE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_IMAGEFX_CARTOON:
			fid = (*env)->GetStaticFieldID(env, cls, "CARTOON", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		default:
			fid = (*env)->GetStaticFieldID(env, cls, "NONE", JAVA_MMAL_IMAGE_EFFECT_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
	}
}

jobject mmal_awb_mode_to_java_enum(JNIEnv * env, MMAL_PARAM_AWBMODE_T mode) {
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/AutoWhiteBalance");
 	jfieldID fid;
	switch(mode) {
		case MMAL_PARAM_AWBMODE_OFF:
			fid = (*env)->GetStaticFieldID(env, cls, "OFF", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_AUTO:
			fid = (*env)->GetStaticFieldID(env, cls, "AUTO", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_SUNLIGHT:
			fid = (*env)->GetStaticFieldID(env, cls, "SUNLIGHT", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_CLOUDY:
			fid = (*env)->GetStaticFieldID(env, cls, "CLOUDY", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_SHADE:
			fid = (*env)->GetStaticFieldID(env, cls, "SHADE", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_TUNGSTEN:
			fid = (*env)->GetStaticFieldID(env, cls, "TUNGSTEN", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_FLUORESCENT:
			fid = (*env)->GetStaticFieldID(env, cls, "FLUORESCENT", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_INCANDESCENT:
			fid = (*env)->GetStaticFieldID(env, cls, "INCANDESCENT", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_FLASH:
			fid = (*env)->GetStaticFieldID(env, cls, "FLASH", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_AWBMODE_HORIZON:
			fid = (*env)->GetStaticFieldID(env, cls, "HORIZON", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		default:
			fid = (*env)->GetStaticFieldID(env, cls, "OFF", JAVA_MMAL_AWB_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
	}
}

void get_camera_data(JNIEnv * env, jobject thisObj, CAM_DATA * camera) {
	jclass raspicam_class = (*env)->GetObjectClass(env, thisObj);
  	jfieldID cameraPointer = (*env)->GetFieldID(env, raspicam_class, "cameraStuff", "Ljava/nio/ByteBuffer;");
  	jobject bb = (*env)->GetObjectField(env, thisObj, cameraPointer);
  	CAM_DATA * tmp_cam = (CAM_DATA*)(*env)->GetDirectBufferAddress(env, bb);
  	(*camera).camera = tmp_cam->camera;
  	(*camera).port_still = tmp_cam->port_still;
  	(*camera).encoder = tmp_cam->encoder;
  	(*camera).encoder_input = tmp_cam->encoder_input;
  	(*camera).encoder_output = tmp_cam->encoder_output;
  	(*camera).encoder_connection = tmp_cam->encoder_connection;
  	(*camera).encoder_pool = tmp_cam->encoder_pool;
}

void save_camera_state(JNIEnv * env, jobject thisObj, CAM_DATA * camera) {
  	jclass raspicam_class = (*env)->GetObjectClass(env, thisObj);
  	jfieldID cameraPointer = (*env)->GetFieldID(env, raspicam_class, "cameraStuff", "Ljava/nio/ByteBuffer;");
  	
  	jobject bb = (*env)->NewDirectByteBuffer(env, camera, sizeof(CAM_DATA));
  	(*env)->SetObjectField(env, thisObj, cameraPointer, bb);
}

MMAL_COMPONENT_T * get_camera(JNIEnv * env, jobject thisObj) {
	get_camera_data(env, thisObj, &camera);
	return camera.camera;
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_open__(JNIEnv * env, jobject thisObj) {
	return Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_open__II(env, thisObj, FRAME_WIDTH_DEFAULT, FRAME_HEIGHT_DEFAULT);
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_open__II(JNIEnv * env, jobject thisObj, 
	jint width, jint height) {
	MMAL_STATUS_T status = raspicam_open(&camera, width, height);
	if(status != MMAL_SUCCESS)
		return mmal_status_to_java_enum(env, status);
	save_camera_state(env, thisObj, &camera);
	return mmal_status_to_java_enum(env, status);
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_takeSnapshot(JNIEnv * env, jobject thisObj,
 jstring filename) {
 	get_camera_data(env, thisObj, &camera);
	MMAL_STATUS_T status;
	unsigned char * buffer;
	int bl = 0;
	status = raspicam_take_snapshot(&camera, &buffer, &bl);
	if(status != MMAL_SUCCESS) {
		raspicam_close(&camera);
		return mmal_status_to_java_enum(env, status);
	}
	const char *nativeString = (*env)->GetStringUTFChars(env, filename, 0);
	FILE * file = fopen(nativeString, "wb");
	fwrite(buffer, bl, 1, file);
	fclose(file);
	free(buffer);
	(*env)->ReleaseStringUTFChars(env, filename, nativeString);
	return mmal_status_to_java_enum(env, status);
}

JNIEXPORT void JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_close(JNIEnv * env, jobject thisObj) {
	get_camera_data(env, thisObj, &camera);
	raspicam_close(&camera);
}


JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setSaturation
  (JNIEnv * env, jobject thisObj, jint saturation) {
 	return raspicam_set_saturation(get_camera(env, thisObj), saturation);
}


JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getSaturation
  (JNIEnv * env, jobject thisObj) {
  	int value = 0;
  	if(!raspicam_get_saturation(get_camera(env, thisObj), &value)) {
  		vcos_log_error("Failed to get saturation value.");
  	}
	return value;  
}
  
JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setExposureCompensation
  (JNIEnv * env, jobject thisObj, jint compensation) {
	return raspicam_set_exposure_compensation(get_camera(env, thisObj), compensation);
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getExposureCompensation
  (JNIEnv * env, jobject thisObj) {
  	int value = 0;
  	if(!raspicam_get_exposure_compensation(get_camera(env, thisObj), &value)) {
  		vcos_log_error("Failed to get exposure compensation value.");
  	}
	return value;  
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setSharpness(JNIEnv * env, jobject thisObj,
jint sharpness) {
	return raspicam_set_sharpness(get_camera(env, thisObj), (int)sharpness);
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getSharpness(JNIEnv * env, jobject thisObj) {
	int value = 0;
	if(!raspicam_get_sharpness(get_camera(env, thisObj), &value)) {
		vcos_log_error("Failed to get sharpness value.");
	}
	return value;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setContrast(JNIEnv * env, jobject thisObj,
jint contrast) {
	return raspicam_set_contrast(get_camera(env, thisObj), (int)contrast);
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getContrast(JNIEnv * env, jobject thisObj) {
	int value = 0;
	if( !raspicam_get_contrast(get_camera(env, thisObj), &value)) {
		vcos_log_error("Failed to get constrast value.");
	}
	return value;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setBrightness(JNIEnv * env, jobject thisObj,
jint brightness) {
	int ret = raspicam_set_brightness(camera.camera, (int)brightness);
	return ret;
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getBrightness(JNIEnv * env, jobject thisObj) {
	int value = 0;
	if( !raspicam_get_brightness(get_camera(env, thisObj), &value)) {
		vcos_log_error("Failed to get brightness value.");
	}
	return value;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setShutterSpeed(JNIEnv * env, jobject thisObj,
 jint shutterSpeed) {
 	return raspicam_set_shutter_speed(get_camera(env, thisObj), shutterSpeed);
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getShutterSpeed(JNIEnv * env, jobject thisObj) {
	int value = 0;
	if( !raspicam_get_shutter_speed(get_camera(env, thisObj), &value)) {
		vcos_log_error("Failed to get shutter speed value.");
	}
	return value;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setRotation(JNIEnv * env, jobject thisObj,
jint rotation) {
	return raspicam_set_rotation(get_camera(env, thisObj), rotation);
}

JNIEXPORT jint JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getRotation(JNIEnv * env, jobject thisObj) {
	int value = 0;
	if( !raspicam_get_rotation(get_camera(env, thisObj), &value)) {
		vcos_log_error("Failed to get rotation value.");
	}
	return value;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setFlip(JNIEnv * env, jobject thisObj,
jboolean hflip, jboolean vflip) {
	return raspicam_set_flips(get_camera(env, thisObj), hflip, vflip);
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getFlip(JNIEnv * env, jobject thisObj) {
	int h = 0;
	int v = 0;
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/Flip");
 	jfieldID fid;
 	
 	if( !raspicam_get_flips(get_camera(env, thisObj), &h, &v)) {
 		vcos_log_error("Failed to get flip values");
 		fid = (*env)->GetStaticFieldID(env, cls, "NONE", JAVA_FLIP_ENUM);
		return (*env)->GetStaticObjectField(env, cls, fid);
 	}
 	if( h && v) {
 		fid = (*env)->GetStaticFieldID(env, cls, "BOTH", JAVA_FLIP_ENUM);
		return (*env)->GetStaticObjectField(env, cls, fid);
 	}
 	if( h) {
 		fid = (*env)->GetStaticFieldID(env, cls, "HORIZONTAL", JAVA_FLIP_ENUM);
		return (*env)->GetStaticObjectField(env, cls, fid);
 	}
 	
 	if(v) {
 		fid = (*env)->GetStaticFieldID(env, cls, "VERTICAL", JAVA_FLIP_ENUM);
		return (*env)->GetStaticObjectField(env, cls, fid);
 	}
	
	fid = (*env)->GetStaticFieldID(env, cls, "NONE", JAVA_FLIP_ENUM);
	return (*env)->GetStaticObjectField(env, cls, fid);
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setRoi(JNIEnv * env, jobject thisObj,
jfloat x, jfloat y, jfloat width, jfloat height) {
	return raspicam_set_ROI(get_camera(env, thisObj), x, y, width, height);
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getRoi(JNIEnv * env, jobject thisObj) {
	double x, y, w, h;
	if( !raspicam_get_ROI(get_camera(env, thisObj), &x, &y, &w, &h)) {
		vcos_log_error("Failed to get ROI value.");
		x = 0.0;
		y = 0.0;
		w = 1.0;
		h = 1.0;
	}
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/RectangleF");
	jmethodID cnstrctr, setX, setY, setWidth, setHeight;
	
	cnstrctr = (*env)->GetMethodID(env, cls, "<init>", "()V");
	setX = (*env)->GetMethodID(env, cls, "setX", "(F)V");
	setY = (*env)->GetMethodID(env, cls, "setY", "(F)V");
	setWidth = (*env)->GetMethodID(env, cls, "setWidth", "(F)V");
	setHeight = (*env)->GetMethodID(env, cls, "setHeight", "(F)V");
	
	jobject rectangle = (*env)->NewObject(env, cls, cnstrctr);
	
	(*env)->CallVoidMethod(env, rectangle, setX, (float)x);
	(*env)->CallVoidMethod(env, rectangle, setY, (float)y);
	(*env)->CallVoidMethod(env, rectangle, setWidth, (float)w);
	(*env)->CallVoidMethod(env, rectangle, setHeight, (float)h);
	return rectangle;
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setExposureMode(JNIEnv * env, jobject thisObj,
 jobject exposureMode) {
 	jmethodID envelopeGetValueMethod = (*env)->GetMethodID(env, (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ExposureMode"), "ordinal", "()I");
    jint value = (*env)->CallIntMethod(env, exposureMode, envelopeGetValueMethod);
    switch (value) {
		case 0: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_OFF);
		case 1: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_AUTO);
		case 2: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_NIGHT);
		case 3: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_NIGHTPREVIEW);
		case 4: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_BACKLIGHT);
		case 5: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_SPOTLIGHT);
		case 6: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_SPORTS);
		case 7: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_SNOW);
		case 8: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_BEACH);
		case 9: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_VERYLONG);
		case 10: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_FIXEDFPS);
		case 11: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_ANTISHAKE);
		case 12: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_FIREWORKS);
		default: return raspicam_set_exposure_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMODE_OFF);
    }
 }

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getExposureMode(JNIEnv * env, jobject thisObj) {
	MMAL_PARAM_EXPOSUREMODE_T mode;
	if( !raspicam_get_exposure_mode(get_camera(env, thisObj), &mode)) {
		vcos_log_error("Failed to get exposure mode value.");
		mode = MMAL_PARAM_EXPOSUREMODE_OFF;
	}
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ExposureMode");
 	jfieldID fid;
	switch(mode) {
		case MMAL_PARAM_EXPOSUREMODE_OFF:
			fid = (*env)->GetStaticFieldID(env, cls, "OFF", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_AUTO:
			fid = (*env)->GetStaticFieldID(env, cls, "AUTO", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_NIGHT:
			fid = (*env)->GetStaticFieldID(env, cls, "NIGHT", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_NIGHTPREVIEW:
			fid = (*env)->GetStaticFieldID(env, cls, "NIGHTPREVIEW", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_BACKLIGHT:
			fid = (*env)->GetStaticFieldID(env, cls, "BACKLIGHT", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_SPOTLIGHT:
			fid = (*env)->GetStaticFieldID(env, cls, "SPOTLIGHT", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_SPORTS:
			fid = (*env)->GetStaticFieldID(env, cls, "SPORTS", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_SNOW:
			fid = (*env)->GetStaticFieldID(env, cls, "SNOW", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_BEACH:
			fid = (*env)->GetStaticFieldID(env, cls, "BEACH", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_VERYLONG:
			fid = (*env)->GetStaticFieldID(env, cls, "VERYLONG", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_FIXEDFPS:
			fid = (*env)->GetStaticFieldID(env, cls, "FIXEDFPS", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_ANTISHAKE:
			fid = (*env)->GetStaticFieldID(env, cls, "ANTISHAKE", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMODE_FIREWORKS:
			fid = (*env)->GetStaticFieldID(env, cls, "FIREWORKS", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		default:
			fid = (*env)->GetStaticFieldID(env, cls, "OFF", JAVA_MMAL_EXPOSURE_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
	}
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setExposureMeteringMode(JNIEnv * env, jobject thisObj, jobject exposureMeteringMode) {
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ExposureMeteringMode");
	jmethodID getValue = (*env)->GetMethodID(env, cls, "ordinal", "()I");
    jint value = (*env)->CallIntMethod(env, exposureMeteringMode, getValue);
    switch (value) {
		case 0: return raspicam_set_metering_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE);
		case 1: return raspicam_set_metering_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMETERINGMODE_SPOT);
		case 2: return raspicam_set_metering_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMETERINGMODE_BACKLIT);
		case 3: return raspicam_set_metering_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMETERINGMODE_MATRIX);
		default: return raspicam_set_metering_mode(get_camera(env, thisObj), MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE);
    }
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getExposureMeteringMode(JNIEnv * env, jobject thisObj) {
	MMAL_PARAM_EXPOSUREMETERINGMODE_T mode;
	
	if(!raspicam_get_metering_mode(get_camera(env, thisObj), &mode)) {
		vcos_log_error("Failed to get exposure metering mode value");
		mode = MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE;
	}
	
	jclass cls = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ExposureMeteringMode");
 	jfieldID fid;
	switch(mode) {
		case MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE:
			fid = (*env)->GetStaticFieldID(env, cls, "AVERAGE", JAVA_MAL_EXPOSURE_METERING_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMETERINGMODE_SPOT:
			fid = (*env)->GetStaticFieldID(env, cls, "SPOT", JAVA_MAL_EXPOSURE_METERING_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMETERINGMODE_BACKLIT:
			fid = (*env)->GetStaticFieldID(env, cls, "BACKLIT", JAVA_MAL_EXPOSURE_METERING_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		case MMAL_PARAM_EXPOSUREMETERINGMODE_MATRIX:
			fid = (*env)->GetStaticFieldID(env, cls, "MATRIX", JAVA_MAL_EXPOSURE_METERING_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
		default:
			fid = (*env)->GetStaticFieldID(env, cls, "AVERAGE", JAVA_MAL_EXPOSURE_METERING_MODE_ENUM);
			return (*env)->GetStaticObjectField(env, cls, fid);
	}
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setAutoWhiteBalance(JNIEnv * env, jobject thisObj,
 jobject autoWhiteBalance) {
 	jclass mmalAwbModeClass = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/AutoWhiteBalance");
	jmethodID getValue = (*env)->GetMethodID(env, mmalAwbModeClass, "ordinal", "()I");
    jint value = (*env)->CallIntMethod(env, autoWhiteBalance, getValue);
    switch (value) {
		case 0: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_OFF);
		case 1: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_AUTO);
		case 2: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_SUNLIGHT);
		case 3: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_CLOUDY);
		case 4: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_SHADE);
		case 5: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_TUNGSTEN);
		case 6: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_FLUORESCENT);
		case 7: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_INCANDESCENT);
		case 8: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_FLASH);
		case 9: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_HORIZON);
		default: return raspicam_set_awb_mode(get_camera(env, thisObj), MMAL_PARAM_AWBMODE_OFF);
    }
 }

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getAutoWhiteBalance(JNIEnv * env, jobject thisObj) {
	MMAL_PARAM_AWBMODE_T mode;
	if(!raspicam_get_awb_mode(get_camera(env, thisObj), &mode)) {
		vcos_log_error("Failed to get auto white balance value.");
		return mmal_awb_mode_to_java_enum(env, MMAL_PARAM_AWBMODE_OFF);
	}
	return mmal_awb_mode_to_java_enum(env, mode);
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_setImageEffect(JNIEnv * env, jobject thisObj, 
jobject imageEffect) {
	jclass mmalImageEffectClass = (*env)->FindClass(env, "ru/andreevskya/raspberrypi/raspicam/ImageEffect");
	jmethodID envelopeGetValueMethod = (*env)->GetMethodID(env, mmalImageEffectClass, "ordinal", "()I");
    jint value = (*env)->CallIntMethod(env, imageEffect, envelopeGetValueMethod);
    switch (value) {
		case 0: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_NONE);
		case 1: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_NEGATIVE);
		case 2: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_SOLARIZE);
		case 3: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_SKETCH);
		case 4: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_DENOISE);
		case 5: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_EMBOSS);
		case 6: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_OILPAINT);
		case 7: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_HATCH);
		case 8: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_GPEN);
		case 9: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_PASTEL);
		case 10: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_WATERCOLOUR);
		case 11: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_FILM);
		case 12: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_BLUR);
		case 13: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_SATURATION);
		case 14: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_COLOURSWAP);
		case 15: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_WASHEDOUT);
		case 16: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_POSTERIZE);
		case 17: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_COLOURPOINT);
		case 18: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_COLOURBALANCE);
		case 19: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_CARTOON);
		default: return raspicam_set_imageFX(get_camera(env, thisObj), MMAL_PARAM_IMAGEFX_NONE);
    }
}

JNIEXPORT jobject JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_getImageEffect(JNIEnv * env, jobject thisObj) {
	MMAL_PARAM_IMAGEFX_T effect;
	if(!raspicam_get_imageFX(get_camera(env, thisObj), &effect)) {
		vcos_log_error("Failed to get image effect value.");
		return mmal_image_effect_to_java_enum(env, MMAL_PARAM_IMAGEFX_NONE);
	}
	return mmal_image_effect_to_java_enum(env, effect);
}

JNIEXPORT jboolean JNICALL Java_ru_andreevskya_raspberrypi_raspicam_RaspberryPiCameraNative_reset(JNIEnv * env, jobject thisObj) {
	return raspicam_set_default_params(get_camera(env, thisObj));
}
