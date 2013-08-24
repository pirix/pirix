#pragma once

typedef struct boot_info boot_info;

/**
 * @defgroup Kernel Kernel Functions
 * General kernel functions
 */

/**
 * The kernel main function. This function gets called by the
 * architecture dependend startup code.
 *
 * @param info The boot information structure.
 * @note This function will never return.
 * @see boot_info
 * @ingroup Kernel
 */
void main(boot_info* info);

/**
 * Induce a kernel panic.
 *
 * @param cause The cause for the panic or `NULL`.
 * @note This function will never return.
 * @ingroup Kernel
 */
void panic(const char* cause);
