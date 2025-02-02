/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef HPM_DMAMUX_DRV_H
#define HPM_DMAMUX_DRV_H
#include "hpm_common.h"
#include "hpm_dmamux_regs.h"

/**
 *
 * @brief DMAMUX driver APIs
 * @defgroup dmamux_interface DMAMUX driver APIs
 * @{
 */


#ifdef __cplusplus
extern "C" {
#endif

static inline void dmamux_enable_channel(DMAMUX_Type *ptr, uint8_t ch_index)
{
    ptr->MUXCFG[ch_index] |= DMAMUX_MUXCFG_ENABLE_MASK;
}

static inline void dmamux_disable_channel(DMAMUX_Type *ptr, uint8_t ch_index)
{
    ptr->MUXCFG[ch_index] &= ~DMAMUX_MUXCFG_ENABLE_MASK;
}

/**
 * @brief   Config DMAMUX
 *
 * @param[in] ptr DMAMUX base address
 * @param[in] src DMAMUX source
 * @param[in] enable Set true to enable the channel
 */
static inline void dmamux_config(DMAMUX_Type *ptr, uint8_t ch_index, uint8_t src,  bool enable)
{
    ptr->MUXCFG[ch_index] = DMAMUX_MUXCFG_SOURCE_SET(src)
                       | DMAMUX_MUXCFG_ENABLE_SET(enable);
}


#ifdef __cplusplus
}
#endif
/**
 * @}
 */

#endif /* HPM_DMAMUX_DRV_H */

