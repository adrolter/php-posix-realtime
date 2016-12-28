--TEST--
Basic test for posix_is_clock_supported()
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php
var_dump(function_exists('posix_is_clock_supported'));

var_dump(posix_is_clock_supported(PSXRT_CLK_REALTIME));

// Only fail if CLOCK_MONOTONIC is defined and is_clock_supported() fails when
//	testing it
var_dump(defined('PSXRT_CLK_MONOTONIC') && !posix_is_clock_supported(PSXRT_CLK_MONOTONIC));

var_dump(posix_is_clock_supported(42));

--EXPECTF--
bool(true)
bool(true)
bool(false)
bool(false)
