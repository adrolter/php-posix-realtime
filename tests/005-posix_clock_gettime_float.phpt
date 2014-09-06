--TEST--
Float output test for posix_clock_gettime()
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php
$clockId = defined(PSXRT_CLK_REALTIME_COARSE) ? PSXRT_CLK_REALTIME_COARSE : PSXRT_CLK_REALTIME;
$returnAs = PSXRT_AS_FLOAT;
$clockRes = posix_clock_getres($clockId);
$a = posix_clock_gettime($clockId, $returnAs);

// A is a float
var_dump($a);

--EXPECTF--
%r(float|double)%r(%f)
