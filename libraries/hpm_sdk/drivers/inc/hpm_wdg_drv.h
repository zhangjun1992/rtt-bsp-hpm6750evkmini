/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef HPM_WDG_DRV_H
#define HPM_WDG_DRV_H

/**
 * @brief WDG APIs
 * @defgroup wdg_interface WDG driver APIs
 * @ingroup wdg_interfaces
 * @{
 */


#include "hpm_common.h"
#include "hpm_wdg_regs.h"

/**
 * @brief WDG Reset Interval definitions
 */
typedef enum reset_interval_enum {
    reset_interval_clock_period_mult_128 = 0,
    reset_interval_clock_period_mult_256,
    reset_interval_clock_period_mult_512,
    reset_interval_clock_period_mult_1k,
    reset_interval_clock_period_mult_2k,
    reset_interval_clock_period_mult_4k,
    reset_interval_clock_period_mult_8k,
    reset_interval_clock_period_mult_16k,
    reset_interval_max = reset_interval_clock_period_mult_16k,
    reset_interval_out_of_range,
} reset_interval_t;

/**
 * @brief WDG Interrupt interval definitions
 */
typedef enum interrupt_interval_enum {
    interrupt_interval_clock_period_multi_64,
    interrupt_interval_clock_period_multi_256,
    interrupt_interval_clock_period_multi_1k,
    interrupt_interval_clock_period_multi_2k,
    interrupt_interval_clock_period_multi_4k,
    interrupt_interval_clock_period_multi_8k,
    interrupt_interval_clock_period_multi_16k,
    interrupt_interval_clock_period_multi_32k,
    interrupt_interval_clock_period_multi_128k,
    interrupt_interval_clock_period_multi_512k,
    interrupt_interval_clock_period_multi_2m,
    interrupt_interval_clock_period_multi_4m,
    interrupt_interval_clock_period_multi_8m,
    interrupt_interval_clock_period_multi_32m,
    interrupt_interval_clock_period_multi_128m,
    interrupt_interval_clock_period_multi_512m,
    interrupt_interval_clock_period_multi_2g,
    interrupt_interval_max = interrupt_interval_clock_period_multi_2g,
    interrupt_interval_out_of_range,
} interrupt_interval_t;

/**
 * @brief WDG clock source definitions
 */
typedef enum wdg_clksrc_enum {
    wdg_clksrc_extclk,                  /**< WDG clock source: external clock */
    wdg_clksrc_pclk,                    /**< WDG clock source: Peripheral clock */
    wdg_clksrc_max = wdg_clksrc_pclk
} wdg_clksrc_t;

/**
 * @brief WDG Control configuration structure
 * @note WDG reset time = reset_interval + interrupt interval
 */
typedef struct wdg_control_struct {
    reset_interval_t reset_interval;            /**< WDG reset interval */
    interrupt_interval_t interrupt_interval;    /**< WDG interrupt interval */
    bool reset_enable;                          /**< WDG reset enable */
    bool interrupt_enable;                      /**< WDG interrupt enable */
    wdg_clksrc_t clksrc;                        /**< WDG clock source */
    bool wdg_enable;                            /**< WDG enable */
} wdg_control_t;

#define WDG_WRITE_ENABLE_MAGIC_NUM (0x5AA5UL)   /**< WDG enable magic number */
#define WDG_RESTART_MAGIC_NUM (0xCAFEUL)        /**< WDG restart magic number */

#define WDG_EXT_CLK_FREQ (32768UL) /**< WDG External CLock frequency: 32768 Hz */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WDG write enable function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_write_enable(WDG_Type *base)
{
    base->WREN = WDG_WRITE_ENABLE_MAGIC_NUM;
}

/**
 * @brief WDG Enable function
 *
 * @param [in] base  WDG base address
 */
static inline void wdg_enable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL |= WDG_CTRL_EN_MASK;
}

/**
 * @brief WDG Disable function
 *
 * @param [in] base  WDG base address
 */
static inline void wdg_disable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL &= ~WDG_CTRL_EN_MASK;
}

/**
 * @brief WDG reset enable function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_reset_enable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL |= WDG_CTRL_RSTEN_MASK;
}

/**
 * @brief WDG reset disable function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_reset_disable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL &= ~WDG_CTRL_RSTEN_MASK;
}


/**
 * @brief WDG interrupt enable function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_interrupt_enable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL |= WDG_CTRL_INTEN_MASK;
}

/**
 * @brief WDG interrupt disable function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_interrupt_disable(WDG_Type *base)
{
    wdg_write_enable(base);
    base->CTRL &= ~WDG_CTRL_INTEN_MASK;
}

/**
 * @brief WDG Clock Source selection function
 *
 * @param [in] base WDG base address
 * @param [in] clksrc WDG clock source
 *   @arg wdg_clksrc_extclk     External clock
 *   @arg wdg_clksrc_pclk       Peripheral clock
 */
static inline void wdg_clksrc_select(WDG_Type *base, wdg_clksrc_t clksrc)
{
    if (clksrc == wdg_clksrc_extclk) {
        base->CTRL &= ~WDG_CTRL_CLKSEL_MASK;
    }
    else {
        base->CTRL |= WDG_CTRL_CLKSEL_MASK;
    }
}

/**
 * @brief WDG restart function
 *
 * @param [in] base WDG base address
 */
static inline void wdg_restart(WDG_Type *base)
{
    wdg_write_enable(base);
    base->RESTART = WDG_RESTART_MAGIC_NUM;
}

/**
 * @brief WDG Get Status function
 *
 * @param [in] base WDG base address
 * @retval WDG status register value
 */
static inline uint32_t wdg_get_status(WDG_Type *base)
{
    return base->ST;
}

/**
 * @brief WDG clear status function
 *
 * @param [in] base WDG base address
 * @param [in] status_mask WDG status mask value
 */
static inline void wdg_clear_status(WDG_Type *base, uint32_t status_mask)
{
    base->ST = status_mask;
}

/**
 * @brief WDG initialization function
 *
 * @param [in] base WDG base address
 * @param [in] wdg_ctrl WDG control structure
 * @retval API execution status
 */
hpm_stat_t wdg_init(WDG_Type *base, wdg_control_t *wdg_ctrl);

/**
 * @brief Get the Reset interval value based on the WDG source clock frequency and the expected reset interval
 *        in terms of microseconds
 *
 * @param [in] src_freq WDG source clock frequency
 * @param [in] reset_us Expected Reset interval in terms of microseconds
 * @retval Converted WDG reset interval
 */
reset_interval_t wdg_get_reset_interval(const uint32_t src_freq, const uint32_t reset_us);

/**
 * @brief Get the interrupt interval value based on the WDG source clock frequency and the expected interrupt interval
 *        in terms of microseconds
 *
 * @param [in] src_freq WDG source clock frequency
 * @param [in] interval_us Expected Interrupt interval in terms of microseconds
 * @retval Converted WDG interrupt interval
 */
interrupt_interval_t wdg_get_interrupt_interval(const uint32_t src_freq, uint32_t interval_us);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* HPM_WDG_DRV_H */
