package ru.andreevskya.raspberrypi.raspicam;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Обёртка над утилитой raspistill, входящей в поставку Raspbian для Малинки.
 * Утилита предназначена для захвата изображений с камеры.
 */
public class RaspberryPiCamera {

    private static final int RASPISTILL_RETURN_CODE_SUCCESS = 0;
    private static final int JPEG_QUALITY = 85;
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

    private final Logger logger;
    private int width = STILL_IMAGE_WIDTH;
    private int height = STILL_IMAGE_HEIGHT;
    private int saturation = SATURATION_DEFAULT;
    private int sharpness = SHARPNESS_DEFAULT;
    private int contrast = CONTRAST_DEFAULT;
    private int brightness = BRIGHTNESS_DEFAULT;
    private int shutterSpeed = SHUTTER_SPEED_DEFAULT;
    private int rotation = 0;
    private boolean flipVertical = false;
    private boolean flipHorizontal = false;
    private RectangleF roi = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
    private ExposureMode exposureMode = ExposureMode.AUTO;
    private ExposureMeteringMode exposureMeteringMode = ExposureMeteringMode.AVERAGE;
    private AutoWhiteBalance autoWhiteBalance = AutoWhiteBalance.AUTO;
    private ImageEffect imageEffect = ImageEffect.NONE;

    public RaspberryPiCamera() {
        logger = LogManager.getLogger(this.getClass());
    }

    public boolean takeSnapshot(File destination) {
        List<String> commandList = new ArrayList<>();
        commandList.add("raspistill");
        commandList.add("-w");
        commandList.add(String.valueOf(this.width));
        commandList.add("-h");
        commandList.add(String.valueOf(this.height));
        commandList.add("-e");
        commandList.add("jpg");
        commandList.add("-n");
        commandList.add("-o");
        commandList.add(destination.getAbsolutePath());
        commandList.add("-q");
        commandList.add(String.valueOf(JPEG_QUALITY));
        commandList.add("-t");
        commandList.add(String.valueOf(1));
        commandList.add("-sh");
        commandList.add(String.valueOf(this.sharpness));
        commandList.add("-co");
        commandList.add(String.valueOf(this.contrast));
        commandList.add("-br");
        commandList.add(String.valueOf(this.brightness));
        commandList.add("-sa");
        commandList.add(String.valueOf(this.saturation));
        commandList.add("-ex");
        commandList.add(this.getExposureMode(this.exposureMode));
        commandList.add("-awb");
        commandList.add(this.getAwb(this.autoWhiteBalance));
        commandList.add("-ifx");
        commandList.add(this.getImageEffect(this.imageEffect));
        commandList.add("-rot");
        commandList.add(String.valueOf(this.rotation));
        commandList.add("-ss");
        commandList.add(String.valueOf(this.shutterSpeed));

        if (this.flipHorizontal) {
            commandList.add("-hf");
        }
        if (this.flipVertical) {
            commandList.add("-vf");
        }
        if (this.roi != null) {
            commandList.add("-roi");
            commandList.add(String.format("%f,%f,%f,%f", this.roi.getX(), this.roi.getY(), this.roi.getWidth(), this.roi.getHeight()));
        }
        ProcessBuilder pb = new ProcessBuilder(commandList);
        try {
            Process p = pb.start();
            return p.waitFor() == RASPISTILL_RETURN_CODE_SUCCESS;
        } catch (IOException | InterruptedException ex) {
            logger.error("Failed to take snapshot from raspistill.", ex);
            return false;
        }
    }

    private String getImageEffect(ImageEffect effect) {
        switch (effect) {
            case NONE:
                return "none";
            case NEGATIVE:
                return "negative";
            case SOLARIZE:
                return "solarise";
            case SKETCH:
                return "sketch";
            case DENOISE:
                return "denoise";
            case EMBOSS:
                return "emboss";
            case OILPAINT:
                return "oilpaint";
            case HATCH:
                return "hatch";
            case GPEN:
                return "gpen";
            case PASTEL:
                return "pastel";
            case WATERCOLOR:
                return "watercolour";
            case FILM:
                return "film";
            case BLUR:
                return "blur";
            case SATURATION:
                return "saturation";
            case COLORSWAP:
                return "colourswap";
            case WASHEDOUT:
                return "washedout";
            case POSTERISE:
                return "posterise";
            case COLORPOINT:
                return "colourpoint";
            case COLORBALANCE:
                return "colourbalance";
            case CARTOON:
                return "cartoon";
            default:
                return "none";
        }
    }

    private String getExposureMode(ExposureMode mode) {
        switch (mode) {
            case OFF:
                return "off";
            case AUTO:
                return "auto";
            case NIGHT:
                return "night";
            case NIGHTPREVIEW:
                return "nightpreview";
            case BACKLIGHT:
                return "backlight";
            case SPOTLIGHT:
                return "spotlight";
            case SPORTS:
                return "sports";
            case SNOW:
                return "snow";
            case BEACH:
                return "beach";
            case VERYLONG:
                return "verylong";
            case FIXEDFPS:
                return "fixedfps";
            case ANTISHAKE:
                return "antishake";
            case FIREWORKS:
                return "fireworks";
            default:
                return "off";
        }
    }

    private String getAwb(AutoWhiteBalance awb) {
        switch (awb) {
            case OFF:
                return "off";
            case AUTO:
                return "auto";
            case SUNLIGHT:
                return "sun";
            case CLOUDY:
                return "cloud";
            case SHADE:
                return "shade";
            case TUNGSTEN:
                return "tungsten";
            case FLUORESCENT:
                return "fluorescent";
            case INCANDESCENT:
                return "incandescent";
            case FLASH:
                return "flash";
            case HORIZON:
                return "horizon";
            default:
                return "off";
        }
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getSaturation() {
        return saturation;
    }

    public void setSaturation(int saturation) {
        this.saturation = saturation;
    }

    public int getSharpness() {
        return sharpness;
    }

    public void setSharpness(int sharpness) {
        this.sharpness = sharpness;
    }

    public int getContrast() {
        return contrast;
    }

    public void setContrast(int contrast) {
        this.contrast = contrast;
    }

    public int getBrightness() {
        return brightness;
    }

    public void setBrightness(int brightness) {
        this.brightness = brightness;
    }

    public int getShutterSpeed() {
        return shutterSpeed;
    }

    public void setShutterSpeed(int shutterSpeed) {
        this.shutterSpeed = shutterSpeed;
    }

    public int getRotation() {
        return rotation;
    }

    public void setRotation(int rotation) {
        this.rotation = rotation;
    }

    public boolean isFlipVertical() {
        return flipVertical;
    }

    public void setFlipVertical(boolean flipVertical) {
        this.flipVertical = flipVertical;
    }

    public boolean isFlipHorizontal() {
        return flipHorizontal;
    }

    public void setFlipHorizontal(boolean flipHorizontal) {
        this.flipHorizontal = flipHorizontal;
    }

    public RectangleF getRoi() {
        return roi;
    }

    public void setRoi(RectangleF roi) {
        this.roi = roi;
    }

    public ExposureMode getExposureMode() {
        return exposureMode;
    }

    public void setExposureMode(ExposureMode exposureMode) {
        this.exposureMode = exposureMode;
    }

    public ExposureMeteringMode getExposureMeteringMode() {
        return exposureMeteringMode;
    }

    public void setExposureMeteringMode(ExposureMeteringMode exposureMeteringMode) {
        this.exposureMeteringMode = exposureMeteringMode;
    }

    public AutoWhiteBalance getAutoWhiteBalance() {
        return autoWhiteBalance;
    }

    public void setAutoWhiteBalance(AutoWhiteBalance autoWhiteBalance) {
        this.autoWhiteBalance = autoWhiteBalance;
    }

    public ImageEffect getImageEffect() {
        return imageEffect;
    }

    public void setImageEffect(ImageEffect imageEffect) {
        this.imageEffect = imageEffect;
    }
}
