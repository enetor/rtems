/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define TEST_INIT

#include <tmacros.h>
#include "test_support.h"

const char rtems_test_name[] = "SP 75";

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code  sc;
  rtems_id           mutex;
  Per_CPU_Control   *cpu_self;

  TEST_BEGIN();

  sc = rtems_semaphore_create(
    rtems_build_name('M', 'U', 'T', 'X'),
    0,
    RTEMS_BINARY_SEMAPHORE,
    0,
    &mutex
  );
  directive_failed(sc, "rtems_semaphore_create");

  /*
   *  Call semaphore obtain with dispatching disabled.  Reenable
   *  dispatching before checking the status returned since
   *  directive_failed() checks for dispatching being enabled.
   */
  puts( "rtems_semaphore_obtain - with dispatching disabled" );
  cpu_self = _Thread_Dispatch_disable();
    sc = rtems_semaphore_obtain(mutex, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);
  _Thread_Dispatch_enable(cpu_self);
  directive_failed(sc, "rtems_semaphore_obtain");

  TEST_END();
  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_SEMAPHORES        1
#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
