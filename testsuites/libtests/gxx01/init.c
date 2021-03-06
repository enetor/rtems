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

#include <errno.h>
#include <tmacros.h>
#include "test_support.h"
#include <rtems/gxx_wrappers.h>

const char rtems_test_name[] = "GXX 1";

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);
void test_recursive_mutex(void);
void test_mutex(void);
void once_function(void);
void test_once(void);
void key_dtor(void *ptr);
void test_key(void);

void test_recursive_mutex(void)
{
  int                sc;
   __gthread_mutex_t mutex;

  mutex = 0;
  puts( "rtems_gxx_recursive_mutex_init() - OK" );
  rtems_gxx_recursive_mutex_init(&mutex);
  rtems_test_assert( mutex != 0 );

  puts( "rtems_gxx_recursive_mutex_trylock() - OK" );
  sc = rtems_gxx_recursive_mutex_trylock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_recursive_mutex_trylock() - Nest" );
  sc = rtems_gxx_recursive_mutex_trylock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_recursive_mutex_unlock() - Unnest" );
  sc = rtems_gxx_recursive_mutex_unlock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_recursive_mutex_unlock() - OK" );
  sc = rtems_gxx_recursive_mutex_unlock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_recursive_mutex_lock() - OK" );
  sc = rtems_gxx_recursive_mutex_lock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_recursive_mutex_unlock() - OK" );
  sc = rtems_gxx_recursive_mutex_unlock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_mutex_destroy(mutex) - OK" );
  sc = rtems_gxx_mutex_destroy(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_mutex_destroy(mutex) - NOT OK" );
  sc = rtems_gxx_mutex_destroy(&mutex);
  rtems_test_assert( sc == -1 );
}

void test_mutex(void)
{
  int                sc;
   __gthread_mutex_t mutex;

  mutex = 0;
  puts( "rtems_gxx_mutex_init() - OK" );
  rtems_gxx_mutex_init(&mutex);
  rtems_test_assert( mutex != 0 );

  puts( "rtems_gxx_mutex_trylock() - OK" );
  sc = rtems_gxx_mutex_trylock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_mutex_unlock() - OK" );
  sc = rtems_gxx_mutex_unlock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_mutex_lock() - OK" );
  sc = rtems_gxx_mutex_lock(&mutex);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_mutex_unlock() - OK" );
  sc = rtems_gxx_mutex_unlock(&mutex);
  rtems_test_assert( sc == 0 );
}

void once_function(void)
{
  puts( "Running once method" );
}

void test_once(void)
{
  __gthread_once_t once;
  int              sc;

  once = 0;

  puts( "Call once method the first time" );
  sc = rtems_gxx_once(&once, once_function);
  rtems_test_assert( sc == 0 );

  puts( "Call once method the second time" );
  sc = rtems_gxx_once(&once, once_function);
  rtems_test_assert( sc == 0 );
}

volatile bool key_dtor_ran;

void key_dtor(void *ptr)
{
  key_dtor_ran = true;
}

void test_key(void)
{
  int              sc;
  __gthread_key_t  key;
  void            *p;

  puts( "rtems_gxx_key_create(&key, NULL) - OK" );
  sc = rtems_gxx_key_create(&key, NULL);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_key_delete(key) - OK" );
  sc = rtems_gxx_key_delete(key);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_key_create(&key, key_dtor) - OK" );
  sc = rtems_gxx_key_create(&key, key_dtor);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_getspecific(key) not set - OK" );
  p = rtems_gxx_getspecific(key);
  rtems_test_assert( p == NULL );

  puts( "rtems_gxx_setspecific(key, 0x1234) - OK" );
  sc = rtems_gxx_setspecific(key, (void *)0x1234);
  rtems_test_assert( sc == 0 );

  puts( "rtems_gxx_getspecific(key) already existing - OK" );
  p = rtems_gxx_getspecific(key);
  rtems_test_assert( p == (void *)0x1234 );

  puts( "rtems_gxx_key_delete(key) - OK" );
  sc = rtems_gxx_key_delete(key);
  rtems_test_assert( sc == 0 );
  /* pthread_key man-page: the dtor should _not_ be called */
  rtems_test_assert( key_dtor_ran != true );

  key = calloc( 1, sizeof( *key ) );
  rtems_test_assert( key != NULL );

  puts( "rtems_gxx_getspecific(key) non-existent - OK" );
  p = rtems_gxx_getspecific( key );
  rtems_test_assert( p == NULL );

  puts( "rtems_gxx_key_delete(key) - NOT OK" );
  sc = rtems_gxx_key_delete( key );
  rtems_test_assert( sc != 0 );

  puts( "rtems_gxx_setspecific(NULL, 0x1234) - NOT OK" );
  sc = rtems_gxx_setspecific( NULL, (void *)0x1234 );
  rtems_test_assert( sc == EINVAL );

  puts( "rtems_gxx_getspecific(NULL) - OK" );
  p = rtems_gxx_getspecific( NULL );
  rtems_test_assert( p == NULL );

  puts( "rtems_gxx_key_delete(NULL) - NOT OK" );
  sc = rtems_gxx_key_delete( NULL );
  rtems_test_assert( sc == EINVAL );
}

rtems_task Init(
  rtems_task_argument argument
)
{
  TEST_BEGIN();

  test_mutex();
  puts( "" );

  test_recursive_mutex();
  puts( "" );

  test_once();
  puts( "" );

  test_key();
  puts( "" );

  TEST_END();

  rtems_test_exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_KEYS            1
#define CONFIGURE_MAXIMUM_POSIX_KEY_VALUE_PAIRS 1

#define CONFIGURE_MAXIMUM_TASKS        1
#define CONFIGURE_MAXIMUM_SEMAPHORES   2
#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
