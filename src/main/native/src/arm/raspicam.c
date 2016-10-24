#include <stdio.h>
#include <interface/vcos/vcos.h>
#include <mmal.h>
#include <mmal_logging.h>
#include <mmal_buffer.h>
#include <util/mmal_util.h>
#include <util/mmal_util_params.h>
#include <util/mmal_default_components.h>
#include <util/mmal_connection.h>
#include <mmal_parameters_camera.h>
#include "raspicam.h"

static void raspicam_control_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
   mmal_buffer_header_release(buffer);
}

MMAL_STATUS_T raspicam_init_encoders(CAM_DATA * camera) {
	MMAL_STATUS_T status = mmal_component_create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER, &camera->encoder);

	if (status != MMAL_SUCCESS) {
    	vcos_log_error("Unable to create JPEG encoder component");
    	goto error;
  	}
  	
  	if (!camera->encoder->input_num || !camera->encoder->output_num) {
    	status = MMAL_ENOSYS;
    	vcos_log_error("JPEG encoder doesn't have input/output ports");
    	goto error;
	}
  	camera->encoder_input = camera->encoder->input[0];
  	camera->encoder_output = camera->encoder->output[0];

	mmal_format_copy(camera->encoder_output->format, camera->encoder_input->format);

	camera->encoder_output->format->encoding = MMAL_ENCODING_JPEG;

	camera->encoder_output->buffer_size = camera->encoder_output->buffer_size_recommended;

  	if(camera->encoder_output->buffer_size < camera->encoder_output->buffer_size_min)
    	camera->encoder_output->buffer_size = camera->encoder_output->buffer_size_min;

	camera->encoder_output->buffer_num = camera->encoder_output->buffer_num_recommended;

	if(camera->encoder_output->buffer_num < camera->encoder_output->buffer_num_min)
		camera->encoder_output->buffer_num = camera->encoder_output->buffer_num_min;

	status = mmal_port_format_commit(camera->encoder_output);

	if(status != MMAL_SUCCESS) {
    	vcos_log_error("Unable to set format on video encoder output port");
    	goto error;
	}

	status = mmal_port_parameter_set_uint32(camera->encoder_output, MMAL_PARAMETER_JPEG_Q_FACTOR, JPEG_QUALITY);

	if(status != MMAL_SUCCESS) {
	    vcos_log_error("Unable to set JPEG quality");
	    goto error;
	}
  	status = mmal_component_enable(camera->encoder);

	if(status != MMAL_SUCCESS) {
    	vcos_log_error("Unable to enable video encoder component");
    	goto error;
	}

  	camera->encoder_pool = mmal_port_pool_create(camera->encoder_output,
  												 camera->encoder_output->buffer_num,
  												 camera->encoder_output->buffer_size);

	if(!camera->encoder_pool) {
    	vcos_log_error("Failed to create buffer header pool for encoder output port %s", camera->encoder_output->name);
    	goto error;
	}
  	return status;
	error:
		DESTROY_MMAL_COMPONENT(camera->encoder);
  		return status;
}

MMAL_STATUS_T raspicam_preview_init(CAM_DATA * camera) {
	MMAL_STATUS_T status = mmal_component_create("vc.null_sink", &camera->null_sink_component);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Unable to create null sink component");
        goto error;
	}
	status = mmal_component_enable(camera->null_sink_component);
	if (status != MMAL_SUCCESS) {
		vcos_log_error("Unable to enable preview/null sink component (%u)", status);
		goto error;
	}
	camera->port_preview = camera->camera->output[CAMERA_PREVIEW_PORT];
	MMAL_ES_FORMAT_T * format = camera->port_preview->format;
	format->encoding = MMAL_ENCODING_OPAQUE;
	format->encoding_variant = MMAL_ENCODING_I420;   
    format->es->video.width = VCOS_ALIGN_UP(320, 32);
    format->es->video.height = VCOS_ALIGN_UP(240, 16);
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = 320;
    format->es->video.crop.height = 240;
    format->es->video.frame_rate.num = PREVIEW_FRAME_RATE_NUM;
    format->es->video.frame_rate.den = PREVIEW_FRAME_RATE_DEN;
   	status = mmal_port_format_commit(camera->port_preview);
   	if (status != MMAL_SUCCESS) {
		vcos_log_error("camera viewfinder format couldn't be set");
		goto error;
	}
	return status;
	error:
		DESTROY_MMAL_COMPONENT(camera->null_sink_component);
		return status;
}

MMAL_STATUS_T raspicam_init(CAM_DATA * camera, int width, int height) {
	MMAL_COMPONENT_T * cam;
	MMAL_PORT_T * port_still;
	MMAL_STATUS_T status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &cam);
	
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to create camera component.");
		goto error;
	}
	
	MMAL_PARAMETER_INT32_T camera_num = {{MMAL_PARAMETER_CAMERA_NUM, sizeof(camera_num)}, 0};
	status = mmal_port_parameter_set(cam->control, &camera_num.hdr);
	
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Could not select camera 0. Error code %d", status);
		goto error;
	}
	
	if(!cam->output_num) {
		status = MMAL_ENOSYS;
		vcos_log_error("Camera does not have output ports.");
		goto error;
	}
	
	status = mmal_port_parameter_set_uint32(cam->control, MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG, 0);
	
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Could not set sensor mode. Error code %d", status);
		goto error;
	}
	
	port_still = cam->output[CAMERA_CAPTURE_PORT];
	
	status = mmal_port_enable(cam->control, raspicam_control_callback);
	
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Unable to enable control port. Error code %d", status);
		goto error;
	}
	
	MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {
		{ MMAL_PARAMETER_CAMERA_CONFIG, sizeof(cam_config) },
		.max_stills_w = width,
		.max_stills_h = height,
		.stills_yuv422 = 0,
		.one_shot_stills = 1,
		.max_preview_video_w = 320,
		.max_preview_video_h = 240,
		.num_preview_video_frames = 3,
		.stills_capture_circular_buffer_height = 0,
		.fast_preview_resume = 0,
		.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
	};
	mmal_port_parameter_set(cam->control, &cam_config.hdr);

	MMAL_ES_FORMAT_T *format;
	format = port_still->format;
   	format->encoding = MMAL_ENCODING_OPAQUE;
   	format->es->video.width = VCOS_ALIGN_UP(width, 32);
   	format->es->video.height = VCOS_ALIGN_UP(height, 16);
   	format->es->video.crop.x = 0;
   	format->es->video.crop.y = 0;
   	format->es->video.crop.width = width;
   	format->es->video.crop.height = height;
   	format->es->video.frame_rate.num = STILLS_FRAME_RATE_NUM;
   	format->es->video.frame_rate.den = STILLS_FRAME_RATE_DEN;

	status = mmal_port_format_commit(port_still);
	
	if (status != MMAL_SUCCESS) {
		vcos_log_error("Camera still format couldn't be set. Error code %d", status);
	    goto error;
	}
	if(port_still->buffer_num < VIDEO_OUTPUT_BUFFERS_NUM)
		port_still->buffer_num = VIDEO_OUTPUT_BUFFERS_NUM;

	status = mmal_component_enable(cam);
	
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Camera component couldn't be enabled. Error code %d", status);
		goto error;
	}
	
	camera->camera = cam;
	camera->port_still = port_still;
	
	return status;
	error:
		DESTROY_MMAL_COMPONENT(cam);
	return status;
}

static MMAL_STATUS_T raspicam_connect_ports(MMAL_PORT_T *output_port, MMAL_PORT_T *input_port, MMAL_CONNECTION_T **connection) {
	MMAL_STATUS_T status;
	status =  mmal_connection_create(connection, output_port, input_port, 
		MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
	if(status == MMAL_SUCCESS) {
		status =  mmal_connection_enable(*connection);
		if(status != MMAL_SUCCESS)
			mmal_connection_destroy(*connection);
	}
	return status;
}

static void raspicam_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer) {
	int complete = 0;
	SNAPSHOT_DATA* pData = (SNAPSHOT_DATA*)port->userdata;
	if(buffer->length) {
		mmal_buffer_header_mem_lock(buffer);
    	if(pData->buffer == NULL) {
      		pData->buffer = (unsigned char*)malloc(buffer->length);
      		pData->buffer_size = buffer->length;
      		memcpy(pData->buffer, buffer->data, buffer->length);
    	} else {
      		unsigned char* newBuffer = (unsigned char*)malloc(pData->buffer_size + buffer->length);
      		memcpy(newBuffer, pData->buffer, pData->buffer_size);
      		free(pData->buffer);
      		memcpy(newBuffer + pData->buffer_size, buffer->data, buffer->length);
     		pData->buffer = newBuffer;
      		pData->buffer_size = pData->buffer_size + buffer->length;
    	}
		mmal_buffer_header_mem_unlock(buffer);
	}
	if (buffer->flags & (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED)) {
    	complete = 1;
    }
	mmal_buffer_header_release(buffer);
  	if (port->is_enabled) {
    	MMAL_STATUS_T status = MMAL_SUCCESS;
    	MMAL_BUFFER_HEADER_T* new_buffer;
    	new_buffer = mmal_queue_get(pData->pool->queue);
    	if(new_buffer) {
     	 	status = mmal_port_send_buffer(port, new_buffer);
    	}
    	if(!new_buffer || status != MMAL_SUCCESS)
      		vcos_log_error("Unable to return a buffer to the encoder port");
  	}
  	if(complete)
    	vcos_semaphore_post(&(pData->complete_semaphore));
}

MMAL_STATUS_T raspicam_open(CAM_DATA * camera, int width, int height) {
	MMAL_STATUS_T status = raspicam_init(camera, width, height);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to init camera.");
		return status;
	}
	status = raspicam_preview_init(camera);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to init camera preview.");
		return status;
	}
	status = raspicam_init_encoders(camera);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to init encoder.");
		return status;
	}
	status = raspicam_connect_ports(camera->port_still, camera->encoder_input, &camera->encoder_connection);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to connect ports.");
		return status;
	}
	status = raspicam_connect_ports(camera->port_preview, camera->null_sink_component->input[0], &camera->preview_connection);
	if(status != MMAL_SUCCESS) {
		vcos_log_error("Failed to connect preview ports.");
		return status;
	}
	return status;
}

MMAL_STATUS_T raspicam_take_snapshot(CAM_DATA * camera, unsigned char ** image, int * image_size) {
	MMAL_STATUS_T status;
	SNAPSHOT_DATA data;
	data.buffer = NULL;
	data.buffer_size = 0;
	VCOS_STATUS_T vcos_status = vcos_semaphore_create(&data.complete_semaphore, "libraspicam-lma-sem", 0);
	vcos_assert(vcos_status == VCOS_SUCCESS);
	status = mmal_port_enable(camera->encoder_output, raspicam_encoder_buffer_callback);
	if( status != MMAL_SUCCESS) {
		vcos_log_error("Unable to enable encoder output.");
		vcos_semaphore_delete(&data.complete_semaphore);
		return status;
	}
	data.pool = camera->encoder_pool;
	int num, q;
	camera->encoder_output->userdata = (struct MMAL_PORT_USERDATA_T *)&data;
	num = mmal_queue_length(camera->encoder_pool->queue);
    for (q = 0; q < num; q++) {
		MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(camera->encoder_pool->queue);
        if (!buffer)
			vcos_log_error("Unable to get a required buffer %d from pool queue", q);
        if (mmal_port_send_buffer(camera->encoder_output, buffer)!= MMAL_SUCCESS)
            vcos_log_error("Unable to send a buffer to encoder output port (%d)", q);
    }
    if (mmal_port_parameter_set_boolean(camera->port_still, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
    	vcos_log_error("%s: Failed to start capture", __func__);
    } else {
    	vcos_semaphore_wait(&data.complete_semaphore);
    	mmal_port_disable(camera->encoder_output);
    	vcos_semaphore_delete(&data.complete_semaphore);
    	*image = data.buffer;
    	*image_size = data.buffer_size;
    	data.buffer = NULL;
    }
	return status;
}

void raspicam_close(CAM_DATA * camera) {
	if(camera->encoder_output && camera->encoder_output->is_enabled)
		mmal_port_disable(camera->encoder_output);
	if(camera->encoder_connection)
		mmal_connection_destroy(camera->encoder_connection);
	if(camera->encoder)
		mmal_component_disable(camera->encoder);
	if(camera)
		 mmal_component_disable(camera->camera);
	if(camera->encoder_pool) {
      mmal_port_pool_destroy(camera->encoder->output[0], camera->encoder_pool);
   	}
	DESTROY_MMAL_COMPONENT(camera->encoder);
	DESTROY_MMAL_COMPONENT(camera->camera);
}
