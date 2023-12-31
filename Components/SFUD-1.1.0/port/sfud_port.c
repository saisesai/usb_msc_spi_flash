/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>

#include "gpio.h"
#include "spi.h"

typedef struct spi1_user_data {
  SPI_HandleTypeDef *hspi;
} spi1_user_data_t;

static char log_buf[256];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

static void spi_lock(const sfud_spi *spi) {
  ((spi1_user_data_t *)(spi->user_data))->hspi->Lock = HAL_LOCKED;
}

static void spi_unlock(const sfud_spi *spi) {
  ((spi1_user_data_t *)(spi->user_data))->hspi->Lock = HAL_UNLOCKED;
}

static void spi_delay_1ms(void) { HAL_Delay(1); }

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf,
                               size_t write_size, uint8_t *read_buf,
                               size_t read_size) {
  HAL_StatusTypeDef rc;
  // read and write data
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // cs on
  if (write_size) { // write data
    SFUD_ASSERT(write_buf);
    //rc = HAL_SPI_Transmit(&hspi1, (uint8_t *)write_buf, write_size, 100);
    rc = HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)write_buf, write_size);
    if (rc != HAL_OK) {
      return SFUD_ERR_WRITE;
    }
    while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
  }
  if (read_size) {  // read data
    SFUD_ASSERT(read_buf);
    //rc = HAL_SPI_Receive(&hspi1, (uint8_t *)read_buf, read_size, 100);
    rc = HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)read_buf, read_size);
    if (rc != HAL_OK) {
      return SFUD_ERR_READ;
    }
    while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);
  }
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // cs off
#ifdef SFUD_DEBUG_MODE
  // print rx and tx data
  size_t i;
  write_buf -= write_size;
  printf("write %u:", write_size);
  for (i = 0; i < write_size; i++) printf(" %02x", write_buf[i]);
  printf("\r\n");
  read_buf -= read_size;
  printf("read %u:", read_size);
  for (i = 0; i < read_size; i++) printf(" %02x", read_buf[i]);
  printf("\r\n");
#endif  // SFUD_DEBUG_MODE
  return SFUD_SUCCESS;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr,
                          sfud_qspi_read_cmd_format *qspi_read_cmd_format,
                          uint8_t *read_buf, size_t read_size) {
  sfud_err result = SFUD_SUCCESS;

  /**
   * add your qspi read flash data code
   */

  return result;
}
#endif /* SFUD_USING_QSPI */

sfud_err sfud_spi_port_init(sfud_flash *flash) {
  sfud_err result = SFUD_SUCCESS;
  switch (flash->index) {
    case SFUD_W25Q64JV_DEVICE_INDEX: {
      flash->spi.wr = spi_write_read;
      flash->spi.lock = spi_lock;
      flash->spi.unlock = spi_unlock;
      flash->spi.user_data = NULL;
      flash->retry.delay = spi_delay_1ms;
      flash->retry.times = 60 * 1000;
      break;
    }
  }
  return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format,
                    ...) {
  va_list args;

  /* args point to the first variable parameter */
  va_start(args, format);
  printf("[SFUD](%s:%ld) ", file, line);
  /* must use vprintf to print */
  vsnprintf(log_buf, sizeof(log_buf), format, args);
  printf("%s\n", log_buf);
  va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
  va_list args;

  /* args point to the first variable parameter */
  va_start(args, format);
  printf("[SFUD] ");
  /* must use vprintf to print */
  vsnprintf(log_buf, sizeof(log_buf), format, args);
  printf("%s\n", log_buf);
  va_end(args);
}
