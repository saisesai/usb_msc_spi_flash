#ifndef LFS_PORT_H
#define LFS_PORT_H

#include "lfs.h"

extern lfs_t lfs;

int lfs_port_init(void);

int lfs_port_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

int lfs_port_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

int lfs_port_erase(const struct lfs_config *c, lfs_block_t block);

int lfs_port_sync(const struct lfs_config *c);

#endif // LFS_PORT_T
