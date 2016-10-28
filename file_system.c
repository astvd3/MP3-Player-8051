#include "file_system.h"
#include "read_sector.h"
#include "sdcard.h"
#include <stdio.h>
#include "print_bytes.h"
#include "Long_Serial_In.h"

uint8_t xdata sector_contents[512];
FS_values_t idata Drive_values;

uint32_t read32(uint16_t offset, uint8_t *array_name)
{
	uint32_t return_value=0;
	uint8_t temp,index;
	for(index=0;index<4;index++)
	{
		temp=*(array_name+offset+(3-index));
		return_value = return_value<<8;
		return_value|=temp;

	}
	return return_value;
}

uint16_t read16(uint16_t offset, uint8_t *array_name)
{
	uint16_t return_value=0;
	uint8_t temp,index;
	for(index=0;index<2;index++)
	{
		temp=*(array_name+offset+(1-index));
		return_value = return_value<<8;
		return_value|=temp;

	}
	return return_value;
}

uint8_t read8(uint16_t offset, uint8_t *array_name)
{
	uint8_t return_value=0;
	return_value=*(array_name+offset);
	return return_value;
}

uint8_t mount_drive(void)
{
	uint8_t  xdata error_flag=0,SectorsPerCluster,NumberFATs;
	uint16_t xdata BytesPerSector,RsvdSectorCount,RootEntryCount,TotalSectors16,FATSize16;
	uint32_t xdata MBR_RelSec,TotalSectors32,FATSize32,RootCluster,FATSz,TotalSetors,NumDataSectors,ClustersCount;

	error_flag= Read_Sector(0,512,sector_contents);
	if(error_flag!=no_errors)
	{
		return error_flag;
	}
	if(sector_contents[0]!= 0xEB && sector_contents[0]!= 0xE9)
	{
		MBR_RelSec=read32(0x01c6,sector_contents);
		printf("\n Sector number of BPB is: %lu \n",MBR_RelSec);
	}
	
	error_flag= Read_Sector(MBR_RelSec,512,sector_contents);
	if(error_flag!=no_errors)
	{
		return error_flag;
	}
	if(sector_contents[0]!= 0xEB && sector_contents[0]!= 0xE9)
	{
		printf("\n  This sector is not BPB \n");
		error_flag = BPB_Not_Found;
		return error_flag;
	}

	BytesPerSector=read16(0x0B,sector_contents);
	SectorsPerCluster=read8(0x0D,sector_contents);
	RsvdSectorCount=read16(0x0E,sector_contents);
	NumberFATs=read8(0x10,sector_contents);
	RootEntryCount=read16(0x11,sector_contents);
	TotalSectors16=read16(0x13,sector_contents);
	FATSize16=read16(0x16,sector_contents);
	TotalSectors32=read32(0x20,sector_contents);
	FATSize32=read32(0x24,sector_contents);
	RootCluster=read32(0x2C,sector_contents);

	Drive_values.BytesPerSec=BytesPerSector;
	Drive_values.SecPerClus=SectorsPerCluster;

	//Root Directory Sectors
	Drive_values.RootDirSecs= ((RootEntryCount*32)+ (BytesPerSector-1))/BytesPerSector;

	// FAT Size
	if(FATSize16==0)
	{
		FATSz=FATSize32;
	}
	else
	{
		FATSz=FATSize16;
	}

	//Total Sectors
	if(TotalSectors16==0)
	{
		TotalSetors=TotalSectors32;
	}
	else
	{
		TotalSetors=TotalSectors16;
	}


	//Number of Data Sectors
	NumDataSectors=TotalSetors-(RsvdSectorCount+(NumberFATs*FATSz)+Drive_values.RootDirSecs);

	//Count of Clusters
	ClustersCount=NumDataSectors/SectorsPerCluster;

	//FAT Type
	if(ClustersCount<65525)
	{
		Drive_values.FATtype=FAT16;
	}
	else
	{
		Drive_values.FATtype=FAT32;
	}

	printf("\n FAT Type:%bx \n", Drive_values.FATtype); 

	//Start of FAT
	Drive_values.StartofFAT=RsvdSectorCount+MBR_RelSec;
	printf("\n Start of FAT:%lu \n", Drive_values.StartofFAT); 


	//First Data Sector
	Drive_values.FirstDataSec=RsvdSectorCount+(NumberFATs*FATSz)+Drive_values.RootDirSecs+MBR_RelSec;
	printf("\n First Data Sector:%lu \n", Drive_values.FirstDataSec); 


	// Starting sector of root directory
	Drive_values.FirstRootDirSec=((RootCluster-2)*SectorsPerCluster)+Drive_values.FirstDataSec;
	printf("\n Starting sector of root directory:%lu \n", Drive_values.FirstRootDirSec); 


}


uint32_t First_Sector(uint32_t Cluster_Num)
{
	uint32_t Sector_number=0;
	if(Cluster_Num==0)
	{
		Sector_number=Drive_values.FirstRootDirSec;
	}
	else
	{
		Sector_number=((Cluster_Num-2)*Drive_values.SecPerClus)+Drive_values.FirstDataSec;
	}
	return Sector_number;
}

uint32_t Find_Next_Clus(uint32_t cluster_num, uint8_t * array)
{
	uint32_t Next_cluster,ThisFATSectorNum,ThisFATEntOffset;

	ThisFATSectorNum= Drive_values.StartofFAT+ ((cluster_num*Drive_values.FATtype)/Drive_values.BytesPerSec);
	Read_Sector(ThisFATSectorNum,512,array);
	ThisFATEntOffset=(cluster_num*Drive_values.FATtype)%Drive_values.BytesPerSec;
	Next_cluster=read32(ThisFATEntOffset,array);
	Next_cluster&=0x0FFFFFFF;
	return Next_cluster;
}


/***********************************************************************
DESC: Returns a pointer to the global structure Drive_values to export to other files
INPUT: void
RETURNS: Pointer to the structure Drive_values
CAUTION: 
************************************************************************/
FS_values_t * Export_Drive_values(void)
{
   return &Drive_values;
}


/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  Print_Directory(uint32_t Sector_num, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t * values;


   values=array_in;
   entries=0;
   i=0;
   if (Drive_values.FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=Drive_values.RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=Drive_values.SecPerClus;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  printf("%5d. ",entries);  // print entry number with a fixed width specifier
		      for(j=0;j<8;j++)
			  {
			     out_val=read8(i+j,values);   // print the 8 byte name
			     putchar(out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     printf("[DIR]\n");
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     putchar(0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     putchar(0x0d);
                 putchar(0x0a);
			  }
		    }
		}
		    i=i+32;  // next entry
		    if(i>510)
		    {
			  Sector++;
              if((Sector-Sector_num)<max_sectors)
			  {
                 error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
			     if(error_flag!=no_errors)
			     {
			        entries=0;   // no entries found indicates disk read error
				    temp8=0;     // forces a function exit
			     }
                 i=0;
			  }
			  else
			  {
			     entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			     temp8=0;                       // forces a function exit
			  }
		    }
         
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
   return entries;
 }


/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Drive_values.FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=Drive_values.RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=Drive_values.SecPerClus;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(Drive_values.FATtype==FAT32)
                {
                   return_clus=read8(21+i,values);
				   return_clus=return_clus<<8;
                   return_clus|=read8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read8(26+i,values);
			    attr=read8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		    }
		}
		    i=i+32;  // next entry
		    if(i>510)
		    {
			  Sector++;
			  if((Sector-Sector_num)<max_sectors)
			  {
                 error_flag=Read_Sector(Sector,Drive_values.BytesPerSec,values);
			     if(error_flag!=no_errors)
			     {
			         return_clus=no_entry_found;
                     temp8=0; 
			     }
			     i=0;
			  }
			  else
			  {
			     temp8=0;                       // forces a function exit
			  }
		    }
         
	  }while(temp8!=0);
	}
	else
	{
	   return_clus=no_entry_found;
	}
	if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
 }

uint8_t open_file(uint32_t Cluster_Num, uint8_t xdata * array_in)
{
	uint32_t FileSector;
	uint8_t error_flag,user_input;

	printf("\nFile\n\n");

	FileSector = First_Sector(Cluster_Num);

	do
	{
		if(FileSector <= FileSector+Drive_values.SecPerClus)
		{
			error_flag= Read_Sector(FileSector,512,array_in);
			if(error_flag!=no_errors)
			{
				return error_flag;
			}
			
		}
		else
		{
			Cluster_Num=Find_Next_Clus(Cluster_Num, array_in);
			FileSector = First_Sector(Cluster_Num);
		}
		printf("\nCluster No: %lu, Sector No: %lu\n \n",Cluster_Num,FileSector);
		print_memory(array_in, 512);
		printf(" \nPress 1 to continue or 0 to exit: \n");
		user_input = long_serial_input();
		if(user_input==1)
		{
			FileSector++;
		
		}

	 }
	 while(user_input==1 && Cluster_Num!=0xFFFFFFFF );

	
	
}
