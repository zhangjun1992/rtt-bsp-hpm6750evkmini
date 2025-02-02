/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef HPM_USB_DRV_H
#define HPM_USB_DRV_H
/*---------------------------------------------------------------------*
 * Includes
 *---------------------------------------------------------------------*/
#include "hpm_common.h"
#include "hpm_usb_regs.h"
#include "hpm_soc_feature.h"

/**
 * @brief USB driver APIs
 * @defgroup usb_interface USB driver APIs
 * @ingroup communication_interfaces
 * @{
 */

/*---------------------------------------------------------------------*
 *  Macro Constant Declarations
 *---------------------------------------------------------------------*/
#define USB_PHY_INIT_DELAY_COUNT  (16U) /**< a delay count for USB phy initialization */
#define USB_HOST_FRAMELIST_SIZE   (8U)  /**< a frame list size in USB host mode */

/*---------------------------------------------------------------------*
 *  Macro Enum Declarations
 *---------------------------------------------------------------------*/
/**
 * @brief USB transfer direction types
 */
typedef enum {
    usb_dir_out     = 0,
    usb_dir_in      = 1,
    usb_dir_in_mask = 0x80
} usb_dir_t;

/**
 * @brief USB transfer types
 */
typedef enum {
    usb_xfer_control = 0,
    usb_xfer_isochronous,
    usb_xfer_bulk,
    usb_xfer_interrupt
} usb_xfer_type_t;

/**
 * @brief USB controller work modes
 */
typedef enum {
    usb_ctrl_mode_otg    = 0,
    usb_ctrl_mode_device = 2,
    usb_ctrl_mode_host   = 3
} usb_controller_mode_t;

/*---------------------------------------------------------------------*
 * Structure Declarations
 *---------------------------------------------------------------------*/
/**
 * @brief Control request structure
 */
typedef struct  __attribute__ ((packed)){
    union {
        struct  __attribute__ ((packed)) {
            uint8_t recipient :  5;
            uint8_t type      :  2;
            uint8_t direction :  1;
        } bmRequestType_bit;

        uint8_t bmRequestType;
    };

    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} usb_control_request_t;

/**
 * @brief Endpoint config structure
 */
typedef struct {
    uint8_t  xfer;
    uint8_t  ep_addr;
    uint16_t max_packet_size;
} usb_endpoint_config_t;

#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

/*---------------------------------------------------------------------*
 * Common API
 *---------------------------------------------------------------------*/

/**
 * @brief Get the mask of all enabled interrupts
 *
 * @param[in] ptr A USB peripheral base address.
 * @retval Mask of all enabled interrupts.
 */
static inline uint32_t usb_get_interrupts(USB_Type *ptr)
{
    return ptr->USBINTR;
}

/**
 * @brief Enable interrupts
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] mask Mask value for interrupt events
 */
static inline void usb_enable_interrupts(USB_Type *ptr, uint32_t mask)
{
    ptr->USBINTR |= mask;
}

/**
 * @brief Get all USB status flags
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The USB interrupt status flags
 */
static inline uint32_t usb_get_status_flags(USB_Type *ptr)
{
    return ptr->USBSTS;
}

/**
 * @brief Clear status flags
 *
 * Only the specified flags can be cleared by writing USBSTS register.
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] mask Mask value for flags to be cleared.
 */

static inline void usb_clear_status_flags(USB_Type *ptr, uint32_t mask)
{
    ptr->USBSTS |= mask;
}

/**
 * @brief Get USB suspend status
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The USB controller suspend status
 */
static inline uint8_t usb_get_suspend_status(USB_Type *ptr)
{
    return USB_PORTSC1_SUSP_GET(ptr->PORTSC1);
}

/**
 * @brief Get USB reset status
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The USB controller reset status
 */
static inline bool usb_get_port_reset_status(USB_Type *ptr)
{
    return USB_PORTSC1_PR_GET(ptr->PORTSC1);
}

/**
 * @brief Get USB current connect status
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The USB controller reset status
 */
static inline bool usb_get_port_ccs(USB_Type *ptr)
{
    return USB_PORTSC1_CCS_GET(ptr->PORTSC1);
}

/**
 * @brief Get USB port speed status
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The USB controller port speed status
 */
static inline uint8_t usb_get_port_speed(USB_Type *ptr)
{
    return USB_PORTSC1_PSPD_GET(ptr->PORTSC1);
}

/*---------------------------------------------------------------------*
 * Device API
 *---------------------------------------------------------------------*/

/**
 * @brief USB device bus reset
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] ep0_max_packet_size The maximum packet size of endpoint 0
 */
void usb_dcd_bus_reset(USB_Type *ptr, uint16_t ep0_max_packet_size);

/**
 * @brief Initialize controller to device mode
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_dcd_init(USB_Type *ptr);

/**
 * @brief Deinitialize controller to device
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_dcd_deinit(USB_Type *ptr);

/**
 * @brief Wakeup from host
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_dcd_remote_wakeup(USB_Type *ptr);

/**
 * @brief Open an endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] config A pointer to the specified endpoint config struct
 */
void usb_dcd_edpt_open(USB_Type *ptr, usb_endpoint_config_t *config);
/**
 * @brief Submit a transfer
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] ep_idx An index of the specified endpoint
 */
void usb_dcd_edpt_xfer(USB_Type *ptr, uint8_t ep_idx);

/**
 * @brief Stall endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] ep_addr An address of the specified endpoint
 */
void usb_dcd_edpt_stall(USB_Type *ptr, uint8_t ep_addr);

/**
 * @brief Clear stall
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] ep_addr An address of the specified endpoint
 */
void usb_dcd_edpt_clear_stall(USB_Type *ptr, uint8_t ep_addr);

/**
 * @brief Close a specified endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] ep_addr An address of the specified endpoint
 */
void usb_dcd_edpt_close(USB_Type *ptr, uint8_t ep_addr);

/**
 * @brief Connect by enabling internal pull-up resistor on D+/D-
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_dcd_connect(USB_Type *ptr);

/**
 * @brief Disconnect by disabling internal pull-up resistor on D+/D-
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_dcd_disconnect(USB_Type *ptr);

/**
 * @brief Get setup status of endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The status of setup endpoint
 */
static inline uint32_t usb_dcd_get_edpt_setup_status(USB_Type *ptr)
{
    return ptr->ENDPTSETUPSTAT;
}

/**
 * @brief Clear the setup status of all specified endpoints
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] mask A mask of all specified endpoints
 */
static inline void usb_dcd_clear_edpt_setup_status(USB_Type *ptr, uint32_t mask)
{
    ptr->ENDPTSETUPSTAT |= mask;
}

/**
 * @brief Set address
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] dev_addr An assigned endpoint address from USB host
 */
static inline void usb_dcd_set_address(USB_Type *ptr, uint8_t dev_addr)
{
    ptr->DEVICEADDR = USB_DEVICEADDR_USBADR_SET(dev_addr) | USB_DEVICEADDR_USBADRA_MASK;
}

/**
 * @brief Set endpoint list address
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] addr A start address of the endpoint qtd list
 */
static inline void usb_dcd_set_edpt_list_addr(USB_Type *ptr, uint32_t addr)
{
    ptr->ENDPTLISTADDR = addr;
}

/**
 * @brief Get device address
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The endpoint address
 */
static inline uint8_t usb_dcd_get_device_addr(USB_Type *ptr)
{
    return USB_DEVICEADDR_USBADR_GET(ptr->DEVICEADDR);
}

/**
 * @brief Get complete status of endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @retval The complete status od endpoint
 */
static inline uint32_t usb_dcd_get_edpt_complete_status(USB_Type *ptr)
{
    return ptr->ENDPTCOMPLETE;
}

/**
 * @brief Clear complete status of endpoint
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] mask A mask of the specified endpoints
 */
static inline void usb_dcd_clear_edpt_complete_status(USB_Type *ptr, uint32_t mask)
{
    ptr->ENDPTCOMPLETE |= mask;
}

/*---------------------------------------------------------------------*
 * Host API
 *---------------------------------------------------------------------*/
/**
 * @brief Initialize controller to host mode
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] int_mask A mask of all required interrupts
 * @param[in] framelist_size A size of the frame list
 */
bool usb_hcd_init(USB_Type *ptr, uint32_t int_mask, uint16_t framelist_size);

/**
 * @brief Initialize controller to host modeHost Reset port
 *
 * @param[in] ptr A USB peripheral base address
 */
void usb_hcd_port_reset(USB_Type *ptr);

/**
 * @brief Initialize controller to host modeHost set command register
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] mask A mask of all required commands
 */
static inline void usb_hcd_set_command(USB_Type *ptr ,uint32_t mask)
{
    ptr->USBCMD |= mask;
}

/**
 * @brief Get frame index
 *
 * @param[in] ptr A USB peripheral base address
 * @retval A index of the current frame list
 */
static inline uint32_t usb_hcd_get_frame_index(USB_Type *ptr)
{
    return ptr->FRINDEX;
}

/**
 * @brief Get port connect status change
 *
 * @param[in] ptr A USB peripheral base address
 * @retval A connect status change
 */
static inline bool usb_hcd_get_port_csc(USB_Type *ptr)
{
    return USB_PORTSC1_CSC_GET(ptr->PORTSC1);
}

/**
 * @brief Get port connect status changeSet async list address
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] addr An the start address of the async endpoint list
 */
static inline void usb_hcd_set_async_list_addr(USB_Type *ptr, uint32_t addr)
{
    ptr->ASYNCLISTADDR = addr;
}

/**
 * @brief Set periodic list address
 *
 * @param[in] ptr A USB peripheral base address
 * @param[in] addr An start address of the periodic endpoint list
 */
static inline void usb_hcd_set_periodic_list_addr(USB_Type *ptr, uint32_t addr)
{
    ptr->PERIODICLISTBASE = addr;
}

#if defined __cplusplus
}
#endif /* __cplusplus */

/** @} */
#endif /* HPM_USB_DRV_H */
