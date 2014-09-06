--TEST--
Basic test for posix_clock_getres()
--SKIPIF--
<?php if (!extension_loaded("posixclocks")) print "skip"; ?>
--FILE--
<?php
var_dump(function_exists('posix_clock_getres'));

$nanosecsInSec = pow(10, 9);
$clockId = defined(PSXCLK_C_REALTIME_COARSE) ? PSXCLK_C_REALTIME_COARSE : PSXCLK_C_REALTIME;
$a = posix_clock_getres();
$b = posix_clock_getres($clockId);

// A is an int
var_dump($a);

// A is a non-zero nanosecond value smaller than a second
var_dump($a > 0 && $a < $nanosecsInSec);

// B is an int
var_dump($b);

// B is a non-zero nanosecond value smaller than a second
var_dump($b > 0 && $b < $nanosecsInSec);

// A and B must be equal unless B is not the default clock (REALTIME)
var_dump($a === $b || $clockId !== PSXCLK_C_REALTIME);

--EXPECTF--
bool(true)
int(%d)
bool(true)
int(%d)
bool(true)
bool(true)
