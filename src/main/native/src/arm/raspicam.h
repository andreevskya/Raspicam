#ifndef RASPICAM_H_
#define RASPICAM_H_

#include <mmal.h>
#include <mmal_logging.h>
#include <mmal_buffer.h>
#include <util/mmal_util.h>
#include <util/mmal_util_params.h>
#include <util/mmal_default_components.h>
#include <util/mmal_connection.h>

#define FRAME_WIDTH_DEFAULT 2592
#define FRAME_HEIGHT_DEFAULT 1944
#define JPEG_QUALITY_DEFAULT 85

#define DESTROY_MMAL_COMPONENT(x) if(x != NULL) {mmal_component_destroy(x); x = NULL;}

#define CAMERA_PREVIEW_PORT 0
#define CAMERA_CAPTURE_PORT 2

#define PREVIEW_FRAME_RATE_NUM 0
#define PREVIEW_FRAME_RATE_DEN 1
#define STILLS_FRAME_RATE_NUM 0
#define STILLS_FRAME_RATE_DEN 1
#define VIDEO_OUTPUT_BUFFERS_NUM 3
#define JPEG_QUALITY 85

typedef struct {
	MMAL_COMPONENT_T * camera;
	MMAL_PORT_T * port_still;
	MMAL_COMPONENT_T *encoder;
	MMAL_PORT_T *encoder_input;
	MMAL_PORT_T *encoder_output;
	MMAL_CONNECTION_T *encoder_connection;
	MMAL_POOL_T *encoder_pool;
	
	MMAL_COMPONENT_T *null_sink_component;
	MMAL_CONNECTION_T *preview_connection;
	MMAL_PORT_T * port_preview;
} CAM_DATA;

typedef struct {
	VCOS_SEMAPHORE_T complete_semaphore;
	MMAL_POOL_T *pool;
	unsigned char * buffer;
	int buffer_size;
} SNAPSHOT_DATA;

MMAL_STATUS_T raspicam_open(CAM_DATA * camera, int width, int height);
MMAL_STATUS_T raspicam_take_snapshot(CAM_DATA * camera, unsigned char ** image, int * image_size);
void raspicam_close(CAM_DATA * camera);
#endif /* RASPICAM_H_ */
