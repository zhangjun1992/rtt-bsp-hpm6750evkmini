/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <sys/stat.h>
#include "hpm_debug_console.h"
#include "hpm_uart_drv.h"

static UART_Type* g_console_uart = NULL;

uint32_t console_init(console_config_t *cfg)
{
    if (cfg->type == console_type_uart)
    {
        uart_config_t config = {0};
        uart_default_config((UART_Type *)cfg->base, &config);
        config.src_freq_in_hz = cfg->src_freq_in_hz;
        config.baudrate = cfg->baudrate;
        uart_init((UART_Type *)cfg->base, &config);
        g_console_uart = (UART_Type *)cfg->base;
    }
    return 0;
}

uint8_t console_receive_byte()
{
    uint8_t c;
    while (status_success != uart_receive_byte(g_console_uart, &c)) {};
    return c;
}

void console_send_byte(uint8_t c)
{
    uart_send_byte(g_console_uart, c);
}

#ifdef __SEGGER_RTL_VERSION
#include "__SEGGER_RTL_Int.h"
static int _stdin_ungot  = EOF;
struct __SEGGER_RTL_FILE_impl { /* NOTE: Provides implementation for FILE */
    int stub; /* only needed so impl has size != 0. */
};

static FILE __SEGGER_RTL_stdin_file  = { 0 };  /* stdin reads from UART */
static FILE __SEGGER_RTL_stdout_file = { 0 };  /* stdout writes to UART */
static FILE __SEGGER_RTL_stderr_file = { 0 };  /* stderr writes to UART */

FILE *stdin  = &__SEGGER_RTL_stdin_file;  /* NOTE: Provide implementation of stdin for RTL. */
FILE *stdout = &__SEGGER_RTL_stdout_file; /* NOTE: Provide implementation of stdout for RTL. */
FILE *stderr = &__SEGGER_RTL_stderr_file; /* NOTE: Provide implementation of stderr for RTL. */

int __SEGGER_RTL_X_file_write(__SEGGER_RTL_FILE *file, const char *data, unsigned size)
{
    int count;
    (void)file;
    for(count = 0; count < size; count++)
    {
        if (data[count] == '\n')
        {
            uart_send_byte(g_console_uart, '\r');
        }
        uart_send_byte(g_console_uart, data[count]);
    }
    uart_flush(g_console_uart);
    return count;

}

int __SEGGER_RTL_X_file_read(__SEGGER_RTL_FILE *file, char *s, unsigned size)
{
    (void)file;
    while (status_success != uart_receive_byte(g_console_uart, (uint8_t *)s)) {}
    return 1;
}

int __SEGGER_RTL_X_file_stat(__SEGGER_RTL_FILE *stream)
{
    return 0;
}

int __SEGGER_RTL_X_file_bufsize(__SEGGER_RTL_FILE *stream)
{
    return 1;
}

int __SEGGER_RTL_X_file_unget(__SEGGER_RTL_FILE *stream, int c)
{
    if (stream == stdin) {
        if (c != EOF && _stdin_ungot == EOF) {
            _stdin_ungot = c;
        } else {
            c = EOF;
        }
    } else {
        c = EOF;
    }
    return c;
}
#endif

int _write(int file, char *data, int size)
{
    int count;
    (void)file;
    for(count = 0; count < size; count++)
    {
        if (data[count] == '\n')
        {
            uart_send_byte(g_console_uart, '\r');
        }
        uart_send_byte(g_console_uart, data[count]);
    }
    uart_flush(g_console_uart);
    return count;
}

int _read(int file, char *s, int size)
{
    (void)file;
    while (status_success != uart_receive_byte(g_console_uart, (uint8_t *)s)) {}
    return 1;
}

int _fstat(int file, struct stat *s)
{
    s->st_mode = S_IFCHR;
    return 0;
}
