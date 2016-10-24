INC=-I/opt/vc/include/ -I/opt/vc/include/interface/mmal
LIBPATH=-L/opt/vc/lib
LIB=-lvcos -lmmal_vc_client -lpthread -lmmal -lmmal_core -lmmal_util -lmmal_components

default: libjni

libjni: raspicamjni.c
	gcc -Wall $(INC) -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include \
-I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux \
 $(LIBPATH) -shared -g -o ../../target/arm/libraspicamjni.so -fPIC raspicamjni.c raspicam.c camcontrol.c $(LIB)
 
demo: demo.c
	gcc -Wall $(INC) -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include \
-I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux \
 $(LIBPATH) -o raspicam demo.c raspicam.c camcontrol.c $(LIB)



