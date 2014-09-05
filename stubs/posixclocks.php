<?php

/**
 * System-wide clock that measures real (i.e., wall-clock) time. This clock
 * is affected by discontinuous jumps in the system time (e.g., manual clock
 * updates, NTP clock updates, etc).
 */
define('PSXCLK_C_REALTIME', 0);

/**
 * Clock that cannot be set and represents monotonic time since some
 * unspecified starting point. This clock is not affected by discontinuous
 * jumps in the system time.
 */
define('PSXCLK_C_MONOTONIC', 1);

/**
 * Per-process CPU-time clock (measures CPU time consumed by all threads in
 * the process).
 */
define('PSXCLK_C_PROCESS_CPUTIME_ID', 2);


/**
 * Thread-specific CPU-time clock.
 */
define('PSXCLK_C_THREAD_CPUTIME_ID', 3);

/**
 * (Linux >= 2.6.28) Similar to CLOCK_MONOTONIC, but provides access to a raw
 * hardware-based time that is not subject to NTP adjustments or the
 * incremental adjustments performed by adjtime.
 */
define('PSXCLK_C_MONOTONIC_RAW', 4);

/**
 * (Linux >= 2.6.32) A faster but less precise version of CLOCK_REALTIME.
 * Use when you need very fast, but not fine-grained timestamps.
 */
define('PSXCLK_C_REALTIME_COARSE', 5);

/**
 * (Linux >= 2.6.32) A faster but less precise version of CLOCK_MONOTONIC.
 * Use when you need very fast, but not fine-grained timestamps.
 */
define('PSXCLK_C_MONOTONIC_COARSE', 6);

/**
 * (Linux >= 2.6.39) Identical to CLOCK_MONOTONIC, except it also includes
 * any time that the system is suspended. This allows applications to get
 * a suspend-aware monotonic clock.
 */
define('PSXCLK_C_BOOTTIME', 7);

/**
 * An object of class stdClass representing the underlying C data structure
 * (struct timespec) is returned.
 */
define('PSXCLK_AS_TIMESPEC', 10);

/**
 * A float (double) representing the decimal value of the requested clock is
 * returned.
 */
define('PSXCLK_AS_FLOAT', 20);

/**
 * A string representing the decimal value of the requested clock is returned.
 */
define('PSXCLK_AS_STRING', 30);

/**
 * Floors the clock's decimal fraction value to the nearest multiple of the 
 * clock's resolution.
 */
define('PSXCLK_FLOOR_TO_CLOCKRES', -1);

/**
 * Determines whether or not the specified clock is supported on the system.
 * @param int $clock_id
 * @return bool
 */
function posix_clock_available($clock_id) {}

/**
 * Retrieves the value (time) of the specified clock.
 * @param int $clock_id
 * @param int $return_as
 * @return string|float|stdClass
 */
function posix_clock_gettime(
	$clock_id = PSXCLK_C_REALTIME,
	$return_as = PSXCLK_AS_STRING,
	$floor_to = false
) {}

/**
 * Retrieves the resolution (precision) of the specified clock.
 * @param int $clock_id
 * @return int
 */
function posix_clock_getres($clock_id = PSXCLK_C_REALTIME) {}
