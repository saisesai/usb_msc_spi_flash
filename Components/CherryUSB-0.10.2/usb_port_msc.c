#include "usb_port_msc.h"

#include "main.h"
#include "sfud.h"

struct usbd_interface intf0;

void usbd_event_handler(uint8_t event) {
  switch (event) {
    case USBD_EVENT_RESET:
      break;
    case USBD_EVENT_CONNECTED:
      break;
    case USBD_EVENT_DISCONNECTED:
      break;
    case USBD_EVENT_RESUME:
      break;
    case USBD_EVENT_SUSPEND:
      break;
    case USBD_EVENT_CONFIGURED:
      break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
      break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
      break;
    default:
      break;
  }
}

void usbd_msc_get_cap(uint8_t lun, uint32_t *block_num, uint16_t *block_size) {
  *block_num = BLOCK_COUNT;
  *block_size = BLOCK_SIZE;
}

int usbd_msc_sector_read(uint32_t sector, uint8_t *buffer, uint32_t length) {
  sfud_err rc;
  const sfud_flash* flash = sfud_get_device_table() + 0;
  rc = sfud_read(flash, sector*BLOCK_SIZE, length, buffer);
  if (rc != SFUD_SUCCESS) {
    printf("[USB_MSC] rc: %u: failed to read %u bytes from sector: %u\r\n", rc, length, sector);
    return -1;
  }
  //printf("[USB_MSC] read %u bytes from sector: %u\r\n", length, sector);
  return 0;
}

int usbd_msc_sector_write(uint32_t sector, uint8_t *buffer, uint32_t length) {
  //printf("[USB_MSC] start to write %u bytes for sector: %u\r\n", length, sector);
  sfud_err rc;
  const sfud_flash* flash = sfud_get_device_table() + 0;
  rc = sfud_erase(flash, sector*BLOCK_SIZE, length);
  if (rc != SFUD_SUCCESS) {
    printf("[USB_MSC] rc: %u: failed to erase %u bytes for sector: %u\r\n", rc, length, sector);
    return -1;
  }
  rc = sfud_write(flash, sector*BLOCK_SIZE, length, buffer);
  if (rc != SFUD_SUCCESS) {
    printf("[USB_MSC] rc: %u: failed to write %u bytes to sector: %u\r\n", rc, length, sector);
    return -1;
  }
  //printf("[USB_MSC] write %u bytes to sector: %u\r\n", length, sector);
  return 0;
}

void usb_port_msc_storage_init(void) {
  usbd_desc_register(msc_storage_descriptor);
  usbd_add_interface(usbd_msc_init_intf(&intf0, MSC_OUT_EP, MSC_IN_EP));
  usbd_initialize();
  printf("[USB] msc device init successful.\r\n");
}
