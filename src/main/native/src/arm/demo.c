#include <stdio.h>
#include "raspicam.h"
#include "camcontrol.h"

void write_to_file(char * name, unsigned char * data, int data_length) {
	FILE * file = fopen(name, "wb");
	fwrite(data, data_length, 1, file);
	fclose(file);
}

int main(int argc, char * argv[]) {
	CAM_DATA camera = {0};
	MMAL_STATUS_T status = raspicam_open(&camera, FRAME_WIDTH_DEFAULT, FRAME_HEIGHT_DEFAULT);
	if(status != MMAL_SUCCESS) {
		printf("Failed to open camera");
	} else {
		unsigned char * buffer = NULL;
		int buffer_length = 0;
		raspicam_set_ROI(camera.camera, 0.2, 0.2, 0.5, 0.5);
		status = raspicam_take_snapshot(&camera, &buffer, &buffer_length);
		if( status != MMAL_SUCCESS) {
			printf("failed to take snapshot");
			return 0;
		} else {
			write_to_file("/home/pi/1.jpg", buffer, buffer_length);
			free(buffer);
			printf("OK\n");
		}
	}
	raspicam_close(&camera);
	return 0;
}
