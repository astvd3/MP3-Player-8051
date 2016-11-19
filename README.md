

# MP3-Player-8051
## Embedded Systems Project

8051 based MP3 Player with SD Card support

### Compiling

Use a uVision 2 or greater (Or any c51 supported compiler) to compile the code into a hex file.

### Connection

If the c51 supports the ISP mode, use an ISP programmer to connect the Microcontroller to the PC.
Else, use a serial port to establish connection to the microcontroller.
For connecting the SDCARD to the Microcontroller, refer to the `sdcard-connection.jpg` file

### Downloading the Hex File

Use FLIP from Atmel or any c51 compatible downloader for downloading the hex file

### Run

In flip, start the application. 
In Putty or any similar serial terminal application, you will see a list of Files and Directories in your SDCard. Use keyboard to navigate through these files
