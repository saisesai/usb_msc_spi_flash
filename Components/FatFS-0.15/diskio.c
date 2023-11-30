/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sfud.h"

/* Definitions of physical drive number for each drive */
#define DEV_SPI1_FLASH 0
#define DEV_SPI1_FLASH_SS 4096
#define DEV_SPI1_FLASH_SC 2048


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv) {
  DSTATUS status = STA_NOINIT;
  switch (pdrv) {
    case DEV_SPI1_FLASH:
      status = RES_OK;
      break;
    default:
      status = STA_NOINIT;
  }
  return status;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv) {
  DSTATUS stat;
  switch (pdrv) {
    case DEV_SPI1_FLASH:
      stat = RES_OK;
      break;
    default:
      stat = STA_NOINIT;
  }
  return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
  DRESULT res;
  switch (pdrv) {
    case DEV_SPI1_FLASH: {
      //printf("[FATFS] read %u bytes from %u.\r\n",  count*DEV_SPI1_FLASH_SS, sector*DEV_SPI1_FLASH_SS);
      const sfud_flash* flash = sfud_get_device_table() + 0;
      if (sfud_read(flash, sector*DEV_SPI1_FLASH_SS, count*DEV_SPI1_FLASH_SS, buff) != SFUD_SUCCESS) {
        printf("[FATFS] failed to read %u bytes from %u.\r\n",  count*DEV_SPI1_FLASH_SS, sector*DEV_SPI1_FLASH_SS);
        res = RES_ERROR;
        break;
      }
      res = RES_OK;
      break;
    }
    default:
      res = RES_PARERR;
  }
  return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
  DRESULT res;
  switch (pdrv) {
    case DEV_SPI1_FLASH: {
      //printf("[FATFS] write %u bytes from %u.\r\n",  count*DEV_SPI1_FLASH_SS, sector*DEV_SPI1_FLASH_SS);
      const sfud_flash* flash = sfud_get_device_table() + 0;
      if (sfud_erase(flash, sector*DEV_SPI1_FLASH_SS, count*DEV_SPI1_FLASH_SS) != SFUD_SUCCESS) {
        printf("[FATFS] failed to erase %u bytes from %u.\r\n",  count*DEV_SPI1_FLASH_SS, sector*DEV_SPI1_FLASH_SS);
        res = RES_ERROR;
        break;
      }
      if (sfud_write(flash, sector*DEV_SPI1_FLASH_SS, count*DEV_SPI1_FLASH_SS, buff) != SFUD_SUCCESS) {
        printf("[FATFS] failed to write %u bytes from %u.\r\n",  count*DEV_SPI1_FLASH_SS, sector*DEV_SPI1_FLASH_SS);
        res = RES_ERROR;
        break;
      }
      res = RES_OK;
      break;
    }
    default:
      res = RES_PARERR;
  }
  return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv,  BYTE cmd, void *buff) {
  DRESULT res = RES_OK;
  switch (pdrv) {
    case DEV_SPI1_FLASH: {
      switch(cmd) {
         case CTRL_SYNC:
          // each write operation to the medium is completed in the disk_write function.
          break;
        case GET_SECTOR_COUNT:
          *(DWORD *)buff = DEV_SPI1_FLASH_SC;
          printf("[FATFS] sector count: %u\r\n", DEV_SPI1_FLASH_SC);
          break;
        case GET_SECTOR_SIZE:
          *(WORD *)buff = DEV_SPI1_FLASH_SS;
          printf("[FATFS] sector size: %u\r\n", DEV_SPI1_FLASH_SS);
          break;
        case GET_BLOCK_SIZE:
          *(DWORD *)buff = 1;
          printf("[FATFS] block size: %u\r\n", 1);
          break;
        default:
          res = RES_PARERR;
      }
      break;
    }
    default:
      res = RES_PARERR;
  }
  return res;
}

DWORD get_fattime(void) {
  return (DWORD)(2023 - 80) << 25 | /* Year */
         (DWORD)(1 + 1) << 21 |     /* Month */
         (DWORD)1 << 16 |           /* Mday */
         (DWORD)1 << 11 |           /* Hour */
         (DWORD)1 << 5 |            /* Min */
         (DWORD)1 >> 1;             /* Sec */
}


