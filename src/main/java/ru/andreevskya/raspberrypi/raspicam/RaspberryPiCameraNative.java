package ru.andreevskya.raspberrypi.raspicam;

import java.nio.ByteBuffer;

public class RaspberryPiCameraNative {
    static {
        System.loadLibrary("raspicamjni");
    }
    public static final int STILL_IMAGE_WIDTH = 2592;
    public static final int STILL_IMAGE_HEIGHT = 1944;
    public static final int SHARPNESS_MIN = -100;
    public static final int SHARPNESS_MAX = 100;
    public static final int SHARPNESS_DEFAULT = 0;
    public static final int CONTRAST_MIN = -100;
    public static final int CONTRAST_DEFAULT = 0;
    public static final int CONTRAST_MAX = 100;
    public static final int BRIGHTNESS_MIN = 0;
    public static final int BRIGHTNESS_DEFAULT = 50;
    public static final int BRIGHTNESS_MAX = 100;
    public static final int SATURATION_MIN = -100;
    public static final int SATURATION_DEFAULT = 0;
    public static final int SATURATION_MAX = 100;
    public static final int SHUTTER_SPEED_DEFAULT = 0;
    public static final int SHUTTER_SPEED_MAX = 6000000;

    /**
     * Используется в нативной либе, хранит указатели на камеру и её компоненты.
     */
    private ByteBuffer cameraStuff;

    public RaspberryPiCameraNative() {

    }

    public native Status open();
    public native Status open(int width, int height);
    public native Status takeSnapshot(String destination);
    public native void close();
    public native boolean setSaturation(int saturation);
    public native int getSaturation();
    public native boolean setExposureCompensation(int compensation);
    public native int getExposureCompensation();
    public native boolean setSharpness(int sharpness);
    public native int getSharpness();
    public native boolean setContrast(int contrast);
    public native int getContrast();
    public native boolean setBrightness(int brightness);
    public native int getBrightness();
    public native boolean setShutterSpeed(int shutterSpeed);
    public native int getShutterSpeed();
    public native boolean setRotation(int rotation);
    public native int getRotation();
    public native boolean setFlip(boolean vertical, boolean horizontal);
    public native Flip getFlip();
    public native boolean setRoi(float x, float y, float w, float h);
    public native RectangleF getRoi();
    public native boolean setExposureMode(ExposureMode mode);
    public native ExposureMode getExposureMode();
    public native boolean setExposureMeteringMode(ExposureMeteringMode mode);
    public native ExposureMeteringMode getExposureMeteringMode();
    public native boolean setAutoWhiteBalance(AutoWhiteBalance balance);
    public native AutoWhiteBalance getAutoWhiteBalance();
    public native boolean setImageEffect(ImageEffect effect);
    public native ImageEffect getImageEffect();
    public native boolean reset();
}
