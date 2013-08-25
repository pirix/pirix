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
void irq_register(int irq, irq_handler* handler);

/**
 * Unregister the handler for an specfic IRQ.
 *
 * @param irq The IRQ number to unregister.
 * @ingroup IRQ
 */
void irq_unregister(int irq);

/**
 * Unmask the specified IRQ.
 *
 * @param irq The IRQ number to allow.
 * @ingroup IRQ
 */
void irq_allow(int irq);

/**
 * Mask the specified IRQ.
 *
 * @param irq The IRQ number to disallow.
 * @ingroup IRQ
 */
void irq_disallow(int irq);

/**
 * Handle an IRQ. Finds the number of the IRQ which occured and calls
 * the handler via irq_run_handler().
 *
 * @param regs The registers of the interrupted process.
 * @return The registers of the thread to resume.
 * @ingroup IRQ
 */
registers* irq_handle(registers* regs);

/**
 * Runs the registered interrupt handler for an IRQ. Does nothing if
 * there is no registered handler.
 *
 * @param irq The number of the occured interrupt.
 * @param regs The registers of the interrupted process.
 * @return The registers of the thread to resume.
 * @ingroup IRQ
 */
registers* irq_run_handler(int irq, registers* regs);
