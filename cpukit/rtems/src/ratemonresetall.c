/**
 * @file
 *
 * @brief rtems_rate_monotonic_reset_all_statistics
 * @ingroup ClassicRateMon Rate Monotonic Scheduler
 */

/*
 *  COPYRIGHT (c) 1989-2010.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/rtems/ratemonimpl.h>

/*
 *  rtems_rate_monotonic_reset_all_statistics
 */
void rtems_rate_monotonic_reset_all_statistics( void )
{
  Objects_Id        id;

   /*
    * Prevent allocation or deallocation of any of the periods while we are
    * cycling.
    */
  _Objects_Allocator_lock();

    /*
     * Cycle through all possible ids and try to reset each one.  If it
     * is a period that is inactive, we just get an error back.  No big deal.
     */
    for ( id=_Rate_monotonic_Information.minimum_id ;
          id <= _Rate_monotonic_Information.maximum_id ;
          id++ ) {
      (void) rtems_rate_monotonic_reset_statistics( id );
    }

  _Objects_Allocator_unlock();
}
