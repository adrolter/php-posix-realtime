--TEST--
Basic test for posix_is_clock_supported()
--SKIPIF--
<?php if (!extension_loaded("posixclocks")) print "skip"; ?>
--FILE--
<?php
var_dump(function_exists('posix_is_clock_supported'));

var_dump(posix_is_clock_supported(PSXCLK_C_REALTIME));

var_dump((defined('PSXCLK_C_MONOTONIC') && posix_is_clock_supported(PSXCLK_C_MONOTONIC)) || false);

var_dump(posix_is_clock_supported(PHP_INT_MAX));

--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(false)
