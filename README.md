# usb_msc_spi_flash

Implementing a USB Drive with STM32 and SPI Flash.

## Working Environment

STM32CubeMX6.9.2 + Keil5 + ARMCC5.06 + Daplink

## Borad

Weact Studio BluePill+

STM32F103C8T6 + W25Q64JV(via spi1)

## Third Party Library

- SFUD: universal serial flash driver
- FatFS: fat filesystem
- CherryUSB: USB protocol stack

## Reproduction process

1. Compile the project.
2. Upload firmware to the board.
3. Connect usb port to pc, it will be recognized as a USB flash drive.
3. Format it to FAT(Default).
4. Reboot.
5. Debug message will output via UART0
6. Reconnect usb port and you will see the boot count file.

