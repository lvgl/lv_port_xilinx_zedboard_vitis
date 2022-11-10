# Zedboard VGA template project
Due to being very grateful for the existence of open source projects such as FreeRTOS, LVGL and TinyUSB, this is an implementation of asymetrical multi-processing (AMP) FreeRTOS +TCP +FAT with LVGL graphics library driving custom a 'VGA' IP implemented in the FPGA fabric for the Zedboard.  I just wanted to give something back to the community and hopefully this will prove a useful template for the Zedboard/Xilinx Zynq 7000 platform. See: https://www.avnet.com/wps/portal/us/products/avnet-boards/avnet-board-families/zedboard/ and: https://www.xilinx.com/products/silicon-devices/soc/zynq-7000.html

**Notes about implementation**

***ARM Core 0*** runs an instance of FreeRTOS implementing the FAT file system, the TCP/IP stack and the graphics library.

***ARM Core 1*** also runs an instance of FreeRTOS with just one task which drives the Zedboard OLED display.

Currently the home grown fabric VGA interface has worked successfully up to a resolution of 1440 x 900 @ 60 Hz.  The configuration set for the example is 1024 x 768 @ 60 Hz.  There are a number of standard resolutions defined in the code which can be used, but currently there is no dynamic selection, a resolution change requires a new firmware build.  It wouldn't be difficult to add some code to change the display resolution at runtime and you could also create your own table entries for other resolutions. This site is a good source of VGA parameters: http://tinyvga.com/vga-timing 

There is a very basic implementation of USB HID for keyboard, mouse and touch screen, which is heavily based on the Github TinyUSB project from @hathachX see here:
https://github.com/hathach/tinyusb
I have modified this code to work with the 'Zynq' in a non-cached way and created a couple of source files which contain the 'Zynq' specific implementation.

I have implemented a basic tab based GUI which enables configuration of the basics for a FreeRTOS +TCP/IP based system along with the primary colour of the GUI and the Dark/Light theme.  There is an SD based file system using FreeRTOS +FAT on which a rudimentary log system has been implemented.  There is a basic FTP server, enabling access to the file system to collect the log and configuration file, it also enables the update of the BOOT.BIN file on the system for firmware update.  I have implementated the LVGL widgets demo modified to run on a tab in the GUI also.  There is a 'System Info' tab which shows the FreeRTOS tasks and stats this is not particularly great for a production system though as it is quite CPU hungry.

**To get started with the project.** 
Required Hardware:
Zedboard with SD card installed, if a USB mouse is used a VGA compatable monitor connected to the Zedboard VGA connector or a Touch screen connected to the VGA port and the USB port (See links below for Waveshare(Ilitek touch) screens I have used.) \
https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-1/13.3inch-hdmi-lcd-h-with-case.htm \
https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-1/15.6inch-hdmi-lcd-h-with-case.htm \
**NOTE: for Waveshare screens VGA connection, this cable is also required:**
https://www.waveshare.com/Mini-HDMI-Male-to-VGA-Female-Cable.htm

***Here is a 'suggested' approach to adding the project to the Xilinx Vitis development environment.***

This approach allows the project to connect to the various git repositories on line that are required and maintain links to the currently checked out versions of the third party libraries etc.

Create the following directory structure choosing your own name and location for the Project_Root_Folder(for this example we are using 'E:\ZedboardTemplate'), **Note: the subfolder names must be as specified here for the relative paths to work in the Vitis environment)**:

![image](./Folders.png)
			  
1. Open the Xilinx Vitis environment and specify a new workspace as per the description above.
![image](./VitisWS.png)

2. Open the Vitis Git perspective and add the submodules one at a time selecting 'Clone a Git repository'(Tip: If you copy the url for the repository prior to clicking 'Clone a Git repository it will automatically fill in the next window for you):
![image](./VitisGit.png)  
**Firstly the FreeRTOS Kernel**  
https://github.com/FreeRTOS/FreeRTOS-Kernel.git  
![image](./VitisGitFRK.png)  
Click Next>  
![image](./VitisGitFRK1.png)  
Select main branch and click Next>  
![image](./VitisGitFRK2.png)  
Select the path to the Zed_GitSubs directory click Finish>  
When the clone is complete you will see the repository in the list:  
![image](./VitisGitFRKPst.png)  
**Now the FreeRTOS-Plus-TCP Module**  
https://github.com/FreeRTOS/FreeRTOS-Plus-TCP.git  
![image](./VitisGitFRKPst1.png)  
![image](./VitisGitFRT.png)  
Click Next>  
![image](./VitisGitFRT1.png)  
Select main and click Next>  
![image](./VitisGitFRT2.png)  
Select the path to the Zed_GitSubs directory click Finish>  
When the clone is complete you will see the repository in the list:  
![image](./VitisGitFRTPst.png)  
**Now the Lab-Project-FreeRTOS-FAT Module**  
https://github.com/FreeRTOS/Lab-Project-FreeRTOS-FAT.git  
![image](./VitisGitFRTPst1.png)  
![image](./VitisGitFRF.png)  
Click Next>  
![image](./VitisGitFRF1.png)  
Select main and click Next>  
![image](./VitisGitFRF2.png)  
Select the path to the Zed_GitSubs directory click Finish>  
When the clone is complete you will see the repository in the list:  
![image](./VitisGitFRFPst.png)  
**Now the LVGL library**  
https://github.com/FreeRTOS/FreeRTOS-Kernel.git  
![image](./VitisGitFRFPst1.png)  
![image](./VitisGitLVGL.png)  
Click Next>  
![image](./VitisGitLVGL1.png)  
Select master and click Next>  
![image](./VitisGitLVGL2.png)  
Select the path to the Zed_GitSubs directory click Finish>  
When the clone is complete you will see the repository in the list:  
![image](./VitisGitLVGLPst.png)  
**Now this Project**  


You are now ready to either create a debug session and load the code over jtag into the Zedboard via USB or you can build the *'ZedBoard_System'* project and copy the *'ZedBoard_System\Debug\sd_card\BOOT.BIN'* file to your SD card then insert it into the Zedboard and it should boot the project.

You can connect a serial terminal @ 115200 BAUD to the USB UART port on the Zedboard to see various status and debug messages during boot and operation.

***NOTE: If you need to edit the fabric/Zynq configuration or add to the project, I have also included the Vivado project here in the folder 'Zedboard_VGA_FPGA' it is not easy to make this work well with Github and Vivado but if you take the folder structure and copy it to your work area and open the Vivado project it should work.  You may need to set up the user repository so it can see the IP projects in the main folder.***
