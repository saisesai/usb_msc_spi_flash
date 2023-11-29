#include "lfs_port.h"

#include "main.h"
#include "sfud.h"

lfs_t lfs;
const struct lfs_config lfs_cfg = {
    .read = lfs_port_read,
    .prog = lfs_port_prog,
    .erase = lfs_port_erase,
    .sync = lfs_port_sync,
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 2048,
    .block_cycles = 500,
    .cache_size = 16,
    .lookahead_size = 16,
};

int lfs_port_init(void) {
  int rc;
  rc = lfs_mount(&lfs, &lfs_cfg);
  if (rc != LFS_ERR_OK) {
    printf("[LFS] failed to mount flash, flash maybe unformatted.\r\n");
    rc = lfs_format(&lfs, &lfs_cfg);
    if (rc != LFS_ERR_OK) {
      printf("[LFS] failed to format flash.\r\n");
      return rc;
    }
    printf("[LFS] flash format successful.\r\n");
    rc = lfs_mount(&lfs, &lfs_cfg);
    if (rc != LFS_ERR_OK) {
      printf("[LFS] failed to mount flash.\r\n");
      return rc;
    }
  }
  printf("[LFS] flash mount successful.\r\n");
  return LFS_ERR_OK;
}

int lfs_port_read(const struct lfs_config *cfg, lfs_block_t block,
                  lfs_off_t off, void *buffer, lfs_size_t size) {
  LFS_ASSERT(off % cfg->read_size == 0);
  LFS_ASSERT(size % cfg->read_size == 0);
  LFS_ASSERT(block < cfg->block_count);
  const sfud_flash *flash = sfud_get_device_table() + 0;
  uint32_t addr = block * cfg->block_size + off;
  sfud_err rc = sfud_read(flash, addr, size, buffer);
  if (rc != SFUD_SUCCESS) {
    printf("[LFS] failed to read %u bytes from %x.\r\n", size, addr);
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int lfs_port_prog(const struct lfs_config *cfg, lfs_block_t block,
                  lfs_off_t off, const void *buffer, lfs_size_t size) {
  LFS_ASSERT(off % cfg->prog_size == 0);
  LFS_ASSERT(size % cfg->prog_size == 0);
  LFS_ASSERT(block < cfg->block_count);
  const sfud_flash *flash = sfud_get_device_table() + 0;
  uint32_t addr = block * cfg->block_size + off;
  sfud_err rc = sfud_write(flash, addr, size, buffer);
  if (rc != SFUD_SUCCESS) {
    printf("[LFS] failed to write %u bytes to %x.\r\n", size, addr);
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int lfs_port_erase(const struct lfs_config *cfg, lfs_block_t block) {
  LFS_ASSERT(block < cfg->block_count);
  const sfud_flash *flash = sfud_get_device_table() + 0;
  uint32_t addr = block * cfg->block_size;
  sfud_err rc = sfud_erase(flash, addr, cfg->block_size);
  if (rc != SFUD_SUCCESS) {
    printf("[LFS] failed to erase %u bytes from %x.\r\n", cfg->block_size,
           addr);
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int lfs_port_sync(const struct lfs_config *cfg) { return LFS_ERR_OK; }
