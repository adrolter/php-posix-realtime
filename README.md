# POSIX Clocks for PHP

This project provides an interface to the real-time, monotonic, CPU-time,
and various other clocks available on systems that implement the POSIX.1b
standard (e.g., Linux, BSD, Solaris, OS X, etc)

#### How is this an improvement on the `microtime()` built-in?

Using microtime is perfectly fine if you need a timestamp and can live with
microsecond resolution. The problem arises when you need to accurately measure
differences (deltas) in time, especially over relatively long periods.
Say, for example, you need to measure the amount of time a large script takes
to execute.

The `microtime()` PHP function relies on the `gettimeofday(2)` system call,
which is quite useful for pinpointing a certain moment in Earthling history.
But, if this realtime system clock is adjusted (maybe by [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol),
or maybe by your friendly neighborhood sysadmin) while you're trying
to *measure time*...well, [you're gonna have a bad...you catch the drift](http://blog.habets.pp.se/2010/09/gettimeofday-should-never-be-used-to-measure-time).

In this case you need access to your system's _monotonic_ clock, or better yet,
its *raw* monotonic clock, if available.

This extension provides access to **all** the clocks supported by your system,
often with nanosecond resolution. See below for a list of common clocks
and their PHP constants.


<br>
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

##### Enabling on Debian/Ubuntu:
```
sudo cp posixclocks.deb.ini /etc/php5/mods-available/posixclocks.ini
sudo php5enmod posixclocks
```

##### Enabling everywhere else:
Add the line `extension=posixclocks.so` to your php.ini


<br>
<a name="interface"/>
## Interface
---

<a name="interface-clock_gettime"/>
### posix_clock_gettime

<a name="interface-clock_gettime-desc"/>
**-- Description --**

`mixed` **`posix_clock_gettime`** ( [<br>
&nbsp;&nbsp;`int` **`$clock_id = PSXCLK_CLOCK_REALTIME`** ,<br>
&nbsp;&nbsp;`int` **`$return_as = PSXCLK_AS_STRING`** ,<br>
&nbsp;&nbsp;`bool` **`$apply_resolution = false`**<br>
] )

Provides an interface to [`clock_gettime(2)`](http://man7.org/linux/man-pages/man2/clock_gettime.2.html).

<a name="interface-clock_gettime-params"/>
**-- Parameters --**

**_clock_id_** The clock to retrieve; defaults to `PSXCLK_CLOCK_REALTIME` if not provided.

**_return_as_** Defines the return value type as detailed in the [return values](#interface-clock_gettime-retvals)
section below. Defaults to `PSXCLK_AS_STRING`; other valid values are
`PSXCLK_AS_FLOAT` and `PSXCLK_AS_TIMESPEC`.

**_apply_resolution_** Whether or not to first apply the clock's resolution to
the value before returning it. Defaults to `false`.

<a name="interface-clock_gettime-retvals"/>
**-- Return Values --**

**`string`** By default (`return_as` = `PSXCLK_AS_STRING`) a string representing
the decimal value of the requested clock is returned.

**`float`** If `return_as` = `PSXCLK_AS_FLOAT`, a float (double) representing
the decimal value of the requested clock is returned.
_**WARNING:** The floating point type may not be large enough to represent
certain time values._

**`object`**  If `return_as` = `PSXCLK_AS_TIMESPEC`, an object of class
`stdClass` representing the underlying C data structure ([`struct timespec`](http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html))
is returned.

```
stdClass Object {
   string|int  tv_sec        Seconds (string if value overflows integer)
          int  tv_nsec       Nanoseconds
  [       int  res_nsec   ]  Resolution in nanoseconds (if apply_resolution = true)
  [       int  tv_nsec_raw]  Nanoseconds before applying resolution (if apply_resolution = true)
}
```

<br>

---

<a name="interface-clock_getres"/>
### posix_clock_getres

<a name="interface-clock_getres-desc"/>
**-- Description --**

`int` **`posix_clock_getres`** ( [ 
  `int` **`$clock_id = PSXCLK_CLOCK_REALTIME`**
] )

Provides an interface to [`clock_getres(2)`](http://man7.org/linux/man-pages/man2/clock_getres.2.html).

<a name="interface-clock_getres-params"/>
**-- Parameters --**

**_clock_id_** The clock whose resolution should be retrieved; defaults to
`PSXCLK_CLOCK_REALTIME` if not provided.

<a name="interface-clock_getres-retvals"/>
**-- Return Values --**

Returns an integer representing the requested clock's
resolution/precision in nanoseconds.

<a name="interface-clock_getres-examples"/>
**-- Example --**

`var_dump(posix_clock_getres(PSXCLK_CLOCK_MONOTONIC_COARSE));`

```
int(4000000)
```

<br>
<a name="clocks"/>
## Clocks
---

The supported clocks are implementation specific, except the system-wide realtime
clock `PSXCLK_CLOCK_REALTIME` which is guaranteed to be supported on all systems
with clock_gettime(2) support. Sufficiently recent versions of GNU libc and the
Linux kernel support the following clocks:


Clock ID (PSXCLK_CLOCK_&lt;_ID_&gt;) | Description
:-------:|------------
`REALTIME` | System-wide clock that measures real (i.e., wall-clock) time. This clock is affected by discontinuous jumps in the system time (e.g., manual clock updates, NTP clock updates, etc).
`MONOTONIC` | Clock that cannot be set and represents monotonic time since some unspecified starting point. This clock is not affected by discontinuous jumps in the system time.
`PROCESS_CPUTIME_ID` | Per-process CPU-time clock (measures CPU time consumed by all threads in the process).
`THREAD_CPUTIME_ID` | Thread-specific CPU-time clock.
`MONOTONIC_RAW` | `Linux >= 2.6.28` Similar to `MONOTONIC`, but provides access to a raw hardware-based time that is not subject to NTP adjustments or the incremental adjustments performed by adjtime.
`REALTIME_COARSE` | `Linux >= 2.6.32` A faster but less precise version of `REALTIME`. Use when you need very fast, but not fine-grained timestamps.
`MONOTONIC_COARSE` | `Linux >= 2.6.32` A faster but less precise version of `MONOTONIC`. Use when you need very fast, but not fine-grained timestamps.
`BOOTTIME` | `Linux >= 2.6.39` Identical to `MONOTONIC`, except it also includes any time that the system is suspended. This allows applications to get a suspend-aware monotonic clock.
<a name="clock_gettime_return_values"/>
More clocks may be implemented by the system, and can be used by passing the corresponding
integer ID in place of a predefined constant. If a clock ID does not exist on the compiling
system, its equivalent PHP constant (PSXCLK_CLOCK_&lt;*ID*&gt;) will be undefined.
<br>

---

<a name="references"/>

http://man7.org/linux/man-pages/man2/clock_gettime.2.html

http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html

http://us3.php.net/manual/en/function.microtime.php#101628

http://blog.habets.pp.se/2010/09/gettimeofday-should-never-be-used-to-measure-time