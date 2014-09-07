--TEST--
Basic test for posix_is_clock_supported()
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php
var_dump(function_exists('posix_is_clock_supported'));

var_dump(posix_is_clock_supported(PSXRT_CLK_REALTIME) === true);

// Pass if CLOCK_MONOTONIC is not defined
var_dump(!defined('PSXRT_CLK_MONOTONIC') || posix_is_clock_supported(PSXRT_CLK_MONOTONIC) === true);

var_dump(posix_is_clock_supported(PHP_INT_MAX) === false);

--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
