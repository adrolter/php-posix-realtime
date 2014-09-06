--TEST--
Float output test for posixclocks_clock_gettime()
--SKIPIF--
<?php if (!extension_loaded("posixclocks")) print "skip"; ?>
--FILE--
<?php
$clockId = defined(PSXCLK_C_REALTIME_COARSE) ? PSXCLK_C_REALTIME_COARSE : PSXCLK_C_REALTIME;
$returnAs = PSXCLK_AS_FLOAT;
$clockRes = posix_clock_getres($clockId);
$a = posix_clock_gettime($clockId, $returnAs);

// A is a float
var_dump($a);

--EXPECTF--
%r(float|double)%r(%f)
