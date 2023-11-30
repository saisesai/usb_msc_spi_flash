#ifndef USB_PORT_DESC_H
#define USB_PORT_DESC_H

#include "usbd_core.h"
#include "usbd_msc.h"

#define MSC_IN_EP  0x81
#define MSC_OUT_EP 0x02

#define USBD_VID           0xFFFF
#define USBD_PID           0xFFFF
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + MSC_DESCRIPTOR_LEN)

#define MSC_MAX_MPS 64

#define BLOCK_DEV_NAME      "stm32"
#define BLOCK_SIZE          (4096)
#define BLOCK_COUNT         (2048)

extern const uint8_t msc_storage_descriptor[];

#endif // USB_PORT_DESC_H
