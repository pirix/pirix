#pragma once

typedef struct registers registers;
typedef registers* (irq_handler)(registers*);

/**
 * @defgroup IRQ Interrupts
 * Interrupt Handling
 */

/**
 * Initialize interrupts.
 *
 * @ingroup IRQ
 */
void irq_init();

/**
 * Register a handler for an specific IRQ.
 *
 * @param irq The IRQ number to use.
 * @param handler The IRQ handler function.
 * @ingroup IRQ
 */
void irq_register(unsigned irq, irq_handler* handler);

/**
 * Unregister the handler for an specfic IRQ.
 *
 * @param irq The IRQ number to unregister.
 * @ingroup IRQ
 */
void irq_unregister(unsigned irq);
