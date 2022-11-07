/*
 * Zynq USB HID Host implementation using TinyUSB stack.
 * This is pretty basic right now and needs plenty of work in
 * the future - be warned.
 * 
 * Connect devices to the OTG port on Zedboard. I have only used 
 * one device at a time with no hubs.  You will have to modify the
 * code to use a hub and support multiple devices simultaneously!
 * 
 * See Github here: https://github.com/hathach/tinyusb
 * Most of the credit needs to go to hathach for his USB lib :-)
 * 
 * 
 * So far it does seem to work with a generic USB mouse,
 * generic keyboard and a couple of Waveshare Ilitek based touch screen 
 * displays with thes values for defines in hid_touch.h:
 * 13.3" touch parameters
 * #define ILI_MAX_X			16384
 * #define ILI_MAX_Y			9600
 * https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-1/13.3inch-hdmi-lcd-h-with-case.htm
 * 
 * 15.6" screen parameters
 * #define ILI_MAX_X			9600
 * #define ILI_MAX_Y			9600
 * https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-1/15.6inch-hdmi-lcd-h-with-case.htm
 * 
 * 
 * NOTE: for Waveshare screens VGA connection, this cable is also required:
 * https://www.waveshare.com/Mini-HDMI-Male-to-VGA-Female-Cable.htm
 * 
 *
 * Copyright (c) 2021 Pete J. Bone - PJBES
 *
 */