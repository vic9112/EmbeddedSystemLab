# erpc_c

erpc C library extracted from https://github.com/EmbeddedRPC/erpc. Codes are modified for mbed 6.

Modification of erpc_c from https://github.com/EmbeddedRPC/erpc:

1. Add DynamicMessageBufferFactory.h from https://github.com/EmbeddedPlanet/mbed-rpc
2. Replace transports/ with transports/ from https://github.com/EmbeddedPlanet/mbed-rpc
3. In port/ keep only the following files:
   erpc_config_internal.h			erpc_port_mbed.cpp
   erpc_endianness_agnostic_example.h	erpc_threading.h
   erpc_endianness_undefined.h		erpc_threading_mbed.cpp
   erpc_port.h 

4. Remove setup/
5. In config/erpc_config.h
   Uncomment 
   #define ERPC_THREADS (ERPC_THREADS_MBED)

6. In port/erpc_threading.h

   line 390:
   bool lock(void); 
   -->
   void lock(void);

   line 398:
   bool unlock(void);
   -->
   void unlock(void);

   In port/erpc_threading_mbed.cpp

   line 193:
   bool Mutex::lock(void)
   -->
   void Mutex::lock(void)

   line 198:
   bool Mutex::unlock(void)
   -->
   void Mutex::unlock(void)

7. In port/erpc_threading_mbed.cpp

   Add #include <chrono>

   In void Thread::sleep(uint32_t usecs):
   std::chrono::milliseconds duration(usecs/1000);
   rtos::ThisThread::sleep_for(duration);

   In bool Semaphore::get(uint32_t timeoutUsecs):
   std::chrono::milliseconds timeoutduration(timeoutUsecs/1000);
   m_count = m_sem->try_acquire_for(timeoutduration);

8. Replace include file extension .h with .hpp

   main.cpp:
   #include "erpc_simple_server.hpp"
   #include "erpc_basic_codec.hpp"
   #include "erpc_crc16.hpp"

   UARTTransport.h:
   #include "erpc_framed_transport.hpp"

   DynamicMessageBufferFactory.h:
   #include "erpc_message_buffer.hpp"
   