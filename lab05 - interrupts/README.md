# Lab 5 Interrupts, timers, tasks and RTOS

## Lab Introduction
- External interrupt signal is often triggered by an external event (e.g., input data waiting for processing). Interrupt signals happen at unexpected time (or asynchronous to the processor timing). Therefore, depending on the preset priority of the interrupt signals, we context-switch from the current running application to handle the request of interrupt signals (by an interrupt service routine; a function designed to process a specific interrupt signal). The implementation of interrupt scheme is important to relieve processor (or software) to constantly polling external devices (usually waste of time and power).
- In the second part, we learn how to schedule functions to be executed at a fixed time or repeated at an interval. We use timer or ticker to achieve the scheduled event. This is useful in control process. For example, we can send a periodical beacon to neighbor nodes to make sure that the interconnected system is well and alive.
- For the last part, we introduce Thread and EventQueue (both are mbed RTOS classes). These classes can be used to execute multiple tasks (independently) and to schedule ISR functions at designed times and with preset priorities. They are more flexible than a simple timer and ticker.

## Equipment List
- B-L4S5I-IOT01A * 1
- USB Type-A male to USB Micro-B male * 1 (compatible with B-L4S5I-IOT01A)
