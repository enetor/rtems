/**
 * @file
 *
 * @brief Removes a Thread from the Simple Queue
 *
 * @ingroup ScoreScheduler
 */

/*
 *  COPYRIGHT (c) 2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulersimpleimpl.h>

void _Scheduler_simple_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
)
{
  Scheduler_simple_Context *context;
  bool                      prepend_it;

  if ( !_Thread_Is_ready( the_thread ) ) {
    /* Nothing to do */
    return;
  }

  context = _Scheduler_simple_Get_context( scheduler );
  _Scheduler_Node_get_priority( node, &prepend_it );

  _Scheduler_simple_Extract( scheduler, the_thread, node );

  if ( prepend_it ) {
    _Scheduler_simple_Insert_priority_lifo( &context->Ready, the_thread );
  } else {
    _Scheduler_simple_Insert_priority_fifo( &context->Ready, the_thread );
  }

  _Scheduler_simple_Schedule_body( scheduler, the_thread, false );
}
