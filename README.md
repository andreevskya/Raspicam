# Raspicam
C library and Java binding to work with Raspberry Pi camera from Java

Build:
Native lib on Raspberry Pi:
1. Install JDK8
2. Go to src/main/native/src/arm
3. Run makefile to build the lib.

Java binding on everywhere:
1. Install JDK8
2. Build Maven project with
	mvn clean install
