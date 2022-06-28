/*
 * File_Handling_RTOS.c
 *
 *  Created on: 26-June-2020
 *      Author: Controllerstech.com
 */

#include "File_Handling.h"
#include "stm32h7xx_hal.h"




/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

extern char USBFile[50][30]; 
extern char USBFileCounter;
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;



uint8_t Mount_USB (void)
{
	fresult = f_mount(&USBHFatFS, USBHPath, 1);
	if (fresult != FR_OK) 
		return 0;
	else
		return 1;
}

void Unmount_USB (void)
{
	fresult = f_mount(NULL, USBHPath, 1);
//	if (fresult == FR_OK) Send_Uart ("USB UNMOUNTED successfully...\n\n\n");
//	else Send_Uart("ERROR!!! in UNMOUNTING USB \n\n\n");
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_USB (char* pat)
{
	  
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "Dir: %s", USBHfno.fname);
              //ILI9341_WriteString(10, 70, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
						
            	free(buf);
                i = strlen(path);
                sprintf(&path[i], "/%s", USBHfno.fname);
                fresult = Scan_USB(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
           	   char *buf = malloc(30*sizeof(char));
               sprintf(buf,"%s/%s", path, USBHfno.fname);
							 if(buf[2]=='b' && buf[3]=='a' && buf[4]=='y' && buf[5]=='a' && buf[6]=='n' && buf[7]=='i' && buf[8]=='c'&& buf[9]=='o')
							 {
								 for(int x=0;x<30;x++)
								   USBFile[USBFileCounter][x]=buf[x]; 
								 USBFileCounter++;
							 }
							 
//               ILI9341_WriteString(10, 90, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
							
               free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_USB (void)
{
    DIR dir;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	fresult = f_unlink(USBHfno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(USBHfno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}




FRESULT Write_File (char *name, unsigned char *data, unsigned int datalength)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists", name);
    //ILI9341_WriteString(10, 170, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	    free(buf);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*", fresult, name);
        //ILI9341_WriteString(10, 170, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	        free(buf);
	        return fresult;
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "Opening file-->  *%s*  To WRITE data in it\n", name);
        //ILI9341_WriteString(10, 170, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	        free(buf);

	    	fresult = f_write(&USBHFile, data, datalength, &bw);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
         // ILI9341_WriteString(10, 170, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	    		free(buf);
	    	}

	    	/* Close file */
	    	fresult = f_close(&USBHFile);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
          //ILI9341_WriteString(10, 190, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	    		free(buf);
	    	}
	    	else
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n\n", name);
          //ILI9341_WriteString(10, 190, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
	    		free(buf);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name, unsigned char *data,unsigned int datalength)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERRROR!!! *%s* does not exists\n\n", name);
    //ILI9341_WriteString(10, 70, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		free(buf);
	    return fresult;
	}
	else
	{
		/* Open file to read */
		fresult = f_open(&USBHFile, name, FA_READ);

		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
//      ILI9341_WriteString(10, 100, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		    free(buf);
		    return fresult;
		}

		/* Read data from the file
		* see the function details for the arguments */

    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "Opening file-->  *%s*  To READ data from it\n", name);
//      ILI9341_WriteString(10, 120, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
        free(buf);

		char *buffer = malloc(sizeof(f_size(&USBHFile)));
		fresult = f_read (&USBHFile, data, datalength , &br);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			free(buffer);
		 	sprintf (buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
      //ILI9341_WriteString(10, 140, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		  	free(buf);
		}

		else
		{
//			Send_Uart(buffer);
			free(buffer);

			/* Close file */
			fresult = f_close(&USBHFile);
			if (fresult != FR_OK)
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
        //ILI9341_WriteString(10, 140, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
				free(buf);
			}
			else
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
        //ILI9341_WriteString(10, 160, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
				free(buf);
			}
		}
	    return fresult;
	}
}

FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult == FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!",name);
    //ILI9341_WriteString(10, 130, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		free(buf);
	    return fresult;
	}
	else
	{
		fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*", fresult, name);
      //ILI9341_WriteString(10, 130, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
			free(buf);
		    return fresult;
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
      //ILI9341_WriteString(10, 130, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
			free(buf);
		}

		fresult = f_close(&USBHFile);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult, name);
      //ILI9341_WriteString(10, 150, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
			free(buf);
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
      //ILI9341_WriteString(10, 150, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
			free(buf);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
//		Send_Uart (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
//	    	Send_Uart(buf);
	        free(buf);
	        return fresult;
	    }

    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "Opening file-->  *%s*  To UPDATE data in it\n", name);
//    	Send_Uart(buf);
        free(buf);

	    /* Writing text */
	    fresult = f_write(&USBHFile, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult, name);
//	    	Send_Uart(buf);
	    	free(buf);
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "*%s* UPDATED successfully\n", name);
//	    	Send_Uart(buf);
	    	free(buf);
	    }

	    /* Close file */
	    fresult = f_close(&USBHFile);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
//	    	Send_Uart(buf);
	    	free(buf);
	    }
	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
//	    	Send_Uart(buf);
	    	free(buf);
	     }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
//		Send_Uart (buf);
		free(buf);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* has been removed successfully\n\n", name);
//			Send_Uart (buf);
			free(buf);
		}

		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
//			Send_Uart (buf);
			free(buf);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "*%s* has been created successfully", name);
      //ILI9341_WriteString(10, 110, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
    	free(buf);
    }
    else
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,name);
      //ILI9341_WriteString(10, 110, buf, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
    	free(buf);
    }
    return fresult;
}

void Check_USB_Details (uint32_t *USB_Total_Size,uint32_t *USB_Free_Size)
{
    /* Check free space */
    f_getfree("", &fre_clust, &pUSBHFatFS);

    total = (uint32_t)((pUSBHFatFS->n_fatent - 2) * pUSBHFatFS->csize * 0.5);
    char *buf = malloc(30*sizeof(char));
    sprintf (buf, "USB  Total Size: %lu",total);
   
    free(buf);
    free_space = (uint32_t)(fre_clust * pUSBHFatFS->csize * 0.5);
    buf = malloc(30*sizeof(char));
    sprintf (buf, "USB Free Space: %lu",free_space);
    
    free(buf);
	  
	  *USB_Total_Size=total;
	  *USB_Free_Size=free_space;
	  
}

