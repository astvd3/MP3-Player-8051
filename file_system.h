#ifndef _file_system_H
#define _file_system_H

#include "main.h"



#define BPB_Not_Found 8
#define FAT32 (4)
#define FAT16 (2)
#define more_entries (0x8000) 
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)
#define no_entry_found (0x80000000)  // msb set to indicate error
#define Disk_Error (0xF0)
#define No_Disk_Error (0)


typedef struct
{
  uint8_t SecPerClus;
  uint8_t FATtype;
  uint8_t BytesPerSecShift;
  uint8_t FATshift;
  uint16_t BytesPerSec;
  uint32_t FirstRootDirSec;
  uint32_t FirstDataSec;
  uint32_t StartofFAT;
  uint32_t RootDirSecs;
} FS_values_t;

// ------ Public function prototypes -------------------------------

uint8_t read8(uint16_t offset, uint8_t *array_name);

uint16_t read16(uint16_t offset, uint8_t *array_name);

uint32_t read32(uint16_t offset, uint8_t *array_name);

uint8_t mount_drive(void);

uint32_t First_Sector(uint32_t Cluster_Num);

uint32_t Find_Next_Clus(uint32_t cluster_num, uint8_t * array);

uint16_t Print_Directory(uint32_t Sector_num, uint8_t xdata * array_in);

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t xdata * array_in); 

FS_values_t * Export_Drive_values(void);

uint8_t open_file(uint32_t Cluster_Num, uint8_t xdata * array_in);

#endif
