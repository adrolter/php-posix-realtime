# POSIX Clocks for PHP

This project provides an interface to the real-time, monotonic, CPU-time,
and various other clocks available on systems that implement the POSIX.1b
standard (e.g., Linux, BSD, Solaris, OS X, etc)

#### But we already have `microtime()`, you say...

Using microtime is perfectly fine if you need a timestamp and can live with
microsecond resolution. The problem arises when you need to accurately measure
differences (deltas) in time, especially over relatively long periods.
Say, for example, you need to measure the amount of time a large script takes
to execute.

The `microtime()` PHP function relies on the `gettimeofday(2)` system call,
which is quite useful for pinpointing a certain moment in Earthling history.
But, if this realtime system clock is adjusted (maybe by [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol),
or maybe by your friendly neighborhood sysadmin) while you're trying
to _measure time_...well, [you're gonna have a bad...you catch the drift](http://blog.habets.pp.se/2010/09/gettimeofday-should-never-be-used-to-measure-time).

In this case you need access to your system's _monotonic_ clock, or better yet,
its _raw_ monotonic clock, if available.

This extension provides access to **all** the clocks supported by your system,
often with nanosecond resolution. See below for a list of common clocks
and their PHP constants.


<br>
---
## Installation
---

```
git clone https://github.com/adrianguenter/php-posix-clocks.git
cd php-posix-clocks
phpize
./configure
make && make test
sudo make install
```

---
##### Debian/Ubuntu:
```
sudo cp posixclocks.deb.ini /etc/php5/mods-available/posixclocks.ini
sudo php5enmod posixclocks
```

---
##### Other:
Add the line `extension=posixclocks.so` to your php.ini


<br>
---
## Interface
---

* double **posix_clock_gettime** ( int _$clock_id_ = _POSIX_CLOCK_REALTIME_ )
    - Provides an interface to [`clock_gettime(2)`](http://man7.org/linux/man-pages/man2/clock_gettime.2.html)
    - Returns a double representing the clock's current value
 
* double **posix_clock_getres** ( int _$clock_id_ = _POSIX_CLOCK_REALTIME_ )
    - Provides an interface to [`clock_getres(2)`](http://man7.org/linux/man-pages/man2/clock_getres.2.html)
    - Returns a double representing the clock's resolution/precision

The supported clocks are implementation specific, except the system-wide realtime
clock (POSIX_CLOCK_REALTIME) which is guaranteed to be supported on all systems with
clock_gettime(2) support. Sufficiently recent versions of GNU libc and the Linux
kernel support the following clocks:


Clock ID (POSIX_CLOCK_*) | Description
:-------:|------------
REALTIME | System-wide clock that measures real (i.e., wall-clock) time. This clock is affected by discontinuous jumps in the system time (e.g., manual clock updates, NTP clock updates, etc).
MONOTONIC | Clock that cannot be set and represents monotonic time since some unspecified starting point. This clock is not affected by discontinuous jumps in the system time.
PROCESS_CPUTIME_ID | Per-process CPU-time clock (measures CPU time consumed by all threads in the process).
THREAD_CPUTIME_ID | Thread-specific CPU-time clock.
MONOTONIC_RAW | `Linux >= 2.6.28` Similar to `MONOTONIC`, but provides access to a raw hardware-based time that is not subject to NTP adjustments or the incremental adjustments performed by adjtime.
REALTIME_COARSE | `Linux >= 2.6.32` A faster but less precise version of `REALTIME`. Use when you need very fast, but not fine-grained timestamps.
MONOTONIC_COARSE | `Linux >= 2.6.32` A faster but less precise version of `MONOTONIC`. Use when you need very fast, but not fine-grained timestamps.
BOOTTIME | `Linux >= 2.6.39` Identical to `MONOTONIC`, except it also includes any time that the system is suspended. This allows applications to get a suspend-aware monotonic clock.

More clocks may be implemented by the system, and can be used by passing the corresponding
integer ID in place of a predefined constant. If a clock ID does not exist on the compiling
system, its PHP POSIX_CLOCK_* equivalent will be undefined within the extension.

<br>
---

http://man7.org/linux/man-pages/man2/clock_gettime.2.html

http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html

http://us3.php.net/manual/en/function.microtime.php#101628

http://blog.habets.pp.se/2010/09/gettimeofday-should-never-be-used-to-measure-time