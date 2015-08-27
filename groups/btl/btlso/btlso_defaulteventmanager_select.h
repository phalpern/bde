// btlso_defaulteventmanager_select.h                                 -*-C++-*-
#ifndef INCLUDED_BTLSO_DEFAULTEVENTMANAGER_SELECT
#define INCLUDED_BTLSO_DEFAULTEVENTMANAGER_SELECT

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a 'select'-based socket-event multiplexer.
//
//@AUTHOR: Andrei Basov (abasov), Rohan Bhindwale (rbhindwa)
//
//@CLASSES:
//   btlso::DefaultEventManager<btlso::Platform::SELECT>:
//                                      'select'-based socket-event multiplexer
//
//@SEE_ALSO: btlso_eventmanager btlso_defaulteventmanager btlso_timemetrics
//
//@DESCRIPTION: This component provides a class,
// 'btlso::DefaultEventManager<btlso::Platform::SELECT>', that implements the
// 'btlso::EventManager' protocol using the 'select' system call available on
// platforms conforming to POSIX.1g.
//
// Generally speaking, a platform imposes a limit on the number of socket
// handles that can be registered with 'select'.  This limit, which is a
// compile time constant (and, therefore, cannot be increased at run time), can
// be as low as 64 (e.g., Windows) or as high as 1024 (e.g., Solaris).
// However, it is often required to handle more than this maximum number of
// simultaneous connections.  This component does not provide a solution to
// this problem but provides an accessor function, 'canRegisterSockets', that
// allows clients to identify if this event manager is at the socket
// registration limit.  If that is the case then clients can create more
// objects of this class for registering their sockets.
//
///Component Diagram
///-----------------
// This specialized component is one of the specializations of the
// 'btlso_defaulteventmanager' component; the other components are shown
// (schematically) on the following diagram:
//..
//                          _bteso_defaulteventmanager_
//                 _______/     |       |        |     \________
//                 *_poll  *_pollset *_select *_devpoll  *_epoll
//
//..
///Thread-safety
///-------------
// The 'select'-based multiplexers provided by this component depend on a
// 'bdema'-style allocator to supply memory.  The allocator is installed
// at creation and is NOT managed by the multiplexers.  If the allocator is not
// *thread* *safe* (*thread* *enabled*) then the instances of this
// component using that allocator will consequently not be *thread* *safe*
// (*thread* *enabled*).  Moreover, if two distinct instances of a multiplexer
// share the same allocator, thread-safety is guaranteed if and only if the
// allocator is *thread* *enabled*.  If no allocator is provided at creation,
// the default allocator, which is *thread* *enabled*, is used.
// Otherwise, the following is guaranteed:
//..
//   o A single instance of this event manager is *not* *thread* *safe* with
//     respect to operations invoked on the same instance from different
//     threads: such operations are *not* guaranteed to work correctly.
//   o An instance of this event manager *is* *thread* *safe* with respect to
//     operations invoked on *different* *instances* from different threads:
//     there will be no side-effects from such operations (which, generally
//     speaking, means that there is no 'static' data).
//   o Distinct instances of this event manager are *thread* *enabled* meaning
//     that operations invoked on distinct instances from different threads can
//     proceed in parallel.
//   o This event manager is not *async-safe*, meaning that one or more
//     functions cannot be invoked safely from a signal handler.
//..
///Performance
///-----------
// Given that S is the number of socket events registered, the two classes of
// this component each provide the following (typical) complexity guarantees:
//..
//  +=======================================================================+
//  |        FUNCTION          | EXPECTED COMPLEXITY | WORST CASE COMPLEXITY|
//  +-----------------------------------------------------------------------+
//  | dispatch                 |        O(S)         |       O(S^2)         |
//  +-----------------------------------------------------------------------+
//  | registerSocketEvent      |        O(1)         |        O(S)          |
//  +-----------------------------------------------------------------------+
//  | deregisterSocketEvent    |        O(1)         |        O(S)          |
//  +-----------------------------------------------------------------------+
//  | deregisterSocket         |        O(1)         |        O(S)          |
//  +-----------------------------------------------------------------------+
//  | deregisterAll            |        O(S)         |        O(S)          |
//  +-----------------------------------------------------------------------+
//  | numSocketEvents          |        O(1)         |        O(S)          |
//  +-----------------------------------------------------------------------+
//  | numEvents                |        O(1)         |        O(1)          |
//  +-----------------------------------------------------------------------+
//  | isRegistered             |        O(1)         |        O(S)          |
//  +=======================================================================+
//..
//
///USAGE EXAMPLE
///-------------
// For the comprehensive usage example, see 'btlso_defaulteventmanager_poll'
// component.

#ifndef INCLUDED_BTLSCM_VERSION
#include <btlscm_version.h>
#endif

#ifndef INCLUDED_BTLSO_PLATFORM
#include <btlso_platform.h>
#endif

#ifdef BTLSO_PLATFORM_WIN_SOCKETS
  #ifndef INCLUDED_WINSOCK2
    #include <winsock2.h>
    #define INCLUDED_WINSOCK2
  #endif
#endif

#ifdef BTLSO_PLATFORM_BSD_SOCKETS
  #ifndef INCLUDED_SYS_SELECT
    #include <sys/select.h>
    #define INCLUDED_SYS_SELECT
  #endif
#endif

#ifndef INCLUDED_BTLSO_DEFAULTEVENTMANAGERIMPL
#include <btlso_defaulteventmanagerimpl.h>
#endif

#ifndef INCLUDED_BTLSO_EVENTMANAGER
#include <btlso_eventmanager.h>
#endif

#ifndef INCLUDED_BTLSO_EVENT
#include <btlso_event.h>
#endif

#ifndef INCLUDED_BDLMA_CONCURRENTPOOLALLOCATOR
#include <bdlma_concurrentpoolallocator.h>
#endif

#ifndef INCLUDED_BDLT_TIMEINTERVAL
#include <bsls_timeinterval.h>
#endif

#ifndef INCLUDED_BSLMA_ALLOCATOR
#include <bslma_allocator.h>
#endif

#ifndef INCLUDED_BSL_UNORDERED_MAP
#include <bsl_unordered_map.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

namespace BloombergLP {


namespace btlso {class TimeMetrics;
            // =======================================================
            // class DefaultEventManager<Platform::SELECT>
            // =======================================================

template <>
class DefaultEventManager<Platform::SELECT> : public EventManager {
    // This class implements the 'EventManager' protocol to provide an
    // event manager that uses the 'select' system call.

  public:
    enum {
        k_MAX_NUM_HANDLES = FD_SETSIZE  // maximum number of socket handles
                                        // that can be registered with an
                                        // event manager (of this type)
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
      , BTESO_MAX_NUM_HANDLES = k_MAX_NUM_HANDLES
      , MAX_NUM_HANDLES = k_MAX_NUM_HANDLES
#endif // BDE_OMIT_INTERNAL_DEPRECATED
    };

  private:
    typedef bsl::unordered_map<Event,
                               EventManager::Callback,
                               EventHash>              EventMap;

    // Due to the initialization dependency between 'd_eventsAllocator'
    // and 'd_events' their declaration order should always be as follows.

    bdlma::ConcurrentPoolAllocator d_eventsAllocator;   // event map allocator

    EventMap            d_events;     // socket events and associated callbacks

    fd_set              d_readSet;    // set of descriptors monitored for
                                      // incoming data

    int                 d_numRead;    // number of sockets in the read set

    fd_set              d_writeSet;   // set of descriptors monitored for
                                      // outgoing data

    int                 d_numWrite;   // number of sockets in the write set

    fd_set              d_exceptSet;  // set of descriptors monitored for
                                      // exceptions

    int                 d_maxFd;      // maximum number of socket descriptors

    TimeMetrics  *d_timeMetric; // time metrics given to this object

    mutable bsl::vector<Event> d_signaledReads;
    mutable bsl::vector<Event> d_signaledWrites;
                                      // temporary arrays used to dispatch
                                      // callbacks

    // PRIVATE ACCESSORS
    bool checkInternalInvariants() const;
        // Verify that every socket handle that is registered in the
        // specified 'events' is set in the appropriate set (e.g., either
        // 'readSet' or 'writeSet' depending on whether or not this is a
        // READ or WRITE event).  Return 'true' on success, and 'false'
        // otherwise.

    int dispatchCallbacks(int           numEvents,
                          const fd_set& readSet,
                          const fd_set& writeSet,
                          const fd_set& exceptSet) const;
        // Dispatch the specified 'numEvents' callbacks from the specified
        // 'readSet', 'writeSet', and 'exceptSet' file descriptor sets that
        // were signalled as ready.

  public:
    // CREATORS
    explicit
    DefaultEventManager(bslma::Allocator  *basicAllocator = 0);
    explicit
    DefaultEventManager(TimeMetrics *timeMetric,
                              bslma::Allocator  *basicAllocator = 0);
        // Create a 'select'-based event manager.  Optionally specify a
        // 'timeMetric' to report time spent in CPU-bound and IO-bound
        // operations.  If 'timeMetric' is not specified or is 0, these metrics
        // are not reported.  Optionally specify a 'basicAllocator' used to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    ~DefaultEventManager();
        // Destroy this object.  Note that the registered callbacks are NOT
        // invoked.

    // MANIPULATORS
    int dispatch(int flags);
        // For each pending socket event, invoke the corresponding callback
        // registered with this event manager.  If no event is pending, wait
        // until either (1) at least one event occurs (in which case the
        // corresponding callback(s) is invoked) or (2) provided that the
        // specified 'flags' contains 'bteso_Flag::k_ASYNC_INTERRUPT', an
        // underlying system call is interrupted by a signal.  Return the
        // number of dispatched callbacks on success, and a negative value
        // otherwise; -1 is reserved to indicate that an underlying system
        // call was interrupted.  When such an interruption occurs this method
        // will return (-1) if 'flags' contains
        // 'bteso_Flag::k_ASYNC_INTERRUPT' and otherwise will automatically
        // restart (i.e., reissue the identical system call).  Note that all
        // callbacks are invoked in the same thread that invokes 'dispatch',
        // and the order of invocation, relative to the order of registration,
        // is unspecified.  Also note that -1 is never returned if 'option' is
        // not set to 'bteso_Flag::k_ASYNC_INTERRUPT'.

    int dispatch(const bsls::TimeInterval& timeout, int flags);
        // For each pending socket event, invoke the corresponding callback
        // registered with this event manager.  If no event is pending, wait
        // until either (1) at least one event occurs (in which case the
        // corresponding callback(s) is invoked), (2) the specified absolute
        // 'timeout' is reached, or (3) provided that the specified 'flags'
        // contains 'bteso_Flag::k_ASYNC_INTERRUPT', an underlying system
        // call is interrupted by a signal.  Return the number of dispatched
        // callbacks on success, 0 if 'timeout' is reached, and a negative
        // value otherwise; -1 is reserved to indicate that an underlying
        // system call was interrupted.  When such an interruption occurs this
        // method will return -1 if 'flags' contains
        // 'bteso_Flag::k_ASYNC_INTERRUPT', and otherwise will
        // automatically restart (i.e., reissue the identical system call).
        // Note that all callbacks are invoked in the same thread that invokes
        // 'dispatch', and the order of invocation, relative to the order of
        // registration, is unspecified.  Also note that -1 is never returned
        // unless 'flags' contains 'bteso_Flag::k_ASYNC_INTERRUPT'.

     int registerSocketEvent(const SocketHandle::Handle&   handle,
                             const EventType::Type         event,
                             const EventManager::Callback& callback);
        // Register with this event manager the specified 'callback' to be
        // invoked when the specified 'event' occurs on the specified socket
        // 'handle'.  Each socket event registration stays in effect until it
        // is subsequently deregistered; the callback is invoked each time the
        // corresponding event is detected.  'EventType::e_READ' and
        // 'EventType::e_WRITE' are the only events that can be
        // registered simultaneously for a socket.  If a registration attempt
        // is made for an event that is already registered, the callback
        // associated with this event will be overwritten with the new one.
        // Simultaneous registration of incompatible events for the same socket
        // 'handle' will result in undefined behavior.  Return 0 on success and
        // a non-zero value otherwise.  The behavior is undefined unless
        // 'canRegisterSockets()' is 'true'.

    void deregisterSocketEvent(const SocketHandle::Handle& handle,
                               EventType::Type             event);
        // Deregister from this event manager the callback associated with the
        // specified 'event' on the specified 'handle' so that said callback
        // will not be invoked should 'event' occur.

    int deregisterSocket(const SocketHandle::Handle& handle);
        // Deregister from this event manager all events associated with the
        // specified socket 'handle'.  Return the number of deregistered
        // callbacks.

    void deregisterAll();
        // Deregister from this event manager all events on every socket
        // handle.

    // ACCESSORS
    bool canRegisterSockets() const;
        // Return 'true' if this event manager can register additional sockets,
        // and 'false' otherwise.  Note that if 'canRegisterSockets' is
        // 'false' then a subsequent call to register an event (without an
        // intervening call to deregister an event) will result in undefined
        // behavior.

    bool hasLimitedSocketCapacity() const;
        // Return 'true' if this event manager has a limited socket capacity,
        // and 'false' otherwise.

    int isRegistered(const SocketHandle::Handle& handle,
                     const EventType::Type       event) const;
        // Return 1 if the specified 'event' is registered with this event
        // manager for the specified socket 'handle' and 0 otherwise.

    int numEvents() const;
        // Return the total number of all socket events currently registered
        // with this event manager.

    int numSocketEvents(const SocketHandle::Handle& handle) const;
        // Return the number of socket events currently registered with this
        // event manager for the specified 'handle'.
};

//-----------------------------------------------------------------------------
//                      INLINE FUNCTIONS' DEFINITIONS
//-----------------------------------------------------------------------------

           // =======================================================
           // class DefaultEventManager<Platform::SELECT>
           // =======================================================

// ACCESSORS
inline
int DefaultEventManager<Platform::SELECT>::numEvents() const
{
    return static_cast<int>(d_events.size());
}

inline
bool DefaultEventManager<Platform::SELECT>::
                                               hasLimitedSocketCapacity() const
{
    return true;
}
}  // close package namespace

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
