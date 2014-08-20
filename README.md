# POSIX Clocks Interface for PHP

This project provides an interface to the real-time, monotonic, CPU-time,
and various other clocks available on systems that implement the POSIX.1b
standard (e.g., Linux, BSD, Solaris, OS X, etc).

## Installation
-----

~~~
$ git clone https://github.com/adrianguenter/php-posix-clocks.git
$ cd php-posix-clocks
$ phpize
$ ./configure
$ make && make test
$ sudo make install
~~~

### Debian:

~~~
$ sudo cp posixclocks.deb.ini /etc/php5/mods-available/posixclocks.ini
$ sudo php5enmod posixclocks
~~~

### Other:

Add the line `extension=posixclocks.so` to your php.ini

## Why you should use it

http://us3.php.net/manual/en/function.microtime.php#101628

http://blog.habets.pp.se/2010/09/gettimeofday-should-never-be-used-to-measure-time


Function Descriptions
--------------------------------------------------------------------------------------
 float posix_clock_gettime (int $clock_id_flag) -
   Provides an interface to the POSIX clock_gettime function.
   Returns the float value of the clock's current time.
 
 float posix_clock_getres (int $clock_id_flag) -
   Provides an interface to the POSIX clock_getres function.
  Returns the float value of the clock's resolution.

Parameters
--------------------------------------------------------------------------------------
 clock_id_flag - 
  The supported clocks are implementation specific, except the system-wide realtime
  clock (CLOCK_REALTIME) which is guaranteed to be supported on all systems with
  clock_gettime(2) support. Sufficiently recent versions of GNU libc and the Linux
  kernel support the following clocks:
  
  * CLOCK_REALTIME - System-wide realtime clock.
  * CLOCK_MONOTONIC - Clock that cannot be set and represents monotonic time since
     some unspecified starting point.
  * CLOCK_PROCESS_CPUTIME_ID - High-resolution per-process timer from the CPU.
  * CLOCK_THREAD_CPUTIME_ID - Thread-specific CPU-time clock.

  More clocks may be implemented. The interpretation of the corresponding time values
  and the effect on timers is unspecified.


Installation
--------------------------------------------------------------------------------------
 > cd extension-source-path
 > ./configure --enable-posixclocks
 > make
 > make install

 It might be necessary to add the line
 extension=posixclocks.so
 to your php.ini(s) in the Dynamic Extensions section.

 If you have PHP-CLI you can test with
 > php -f posixclocks-test.php
 and benchmark with
 > php -f posixclocks-bench.php