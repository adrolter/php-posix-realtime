--TEST--
String output test for posix_clock_gettime()
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php
// Test against a "coarse" clock if available for non-1ns resolution in tests
$clockId = defined('PSXRT_CLK_REALTIME_COARSE') ? PSXRT_CLK_REALTIME_COARSE : PSXRT_CLK_REALTIME;
$returnAs = PSXRT_AS_STRING;
$clockRes = posix_clock_getres($clockId);
$a = posix_clock_gettime($clockId, $returnAs);
$b = posix_clock_gettime($clockId, $returnAs, false);
$c = posix_clock_gettime($clockId, $returnAs, (pow(10, 9) / 2));
$d = posix_clock_gettime($clockId, $returnAs, PSXRT_FLOOR_TO_CLOCKRES);
$dParts = explode('.', $d);

// A is a float string
var_dump($a);

// B is a float string
var_dump($b);

// Difference between A length and B length is not more than 1 char
//	(one of them may end in truncated 0)
var_dump(abs(strlen($a) - strlen($b)) <= 1);

// C is a float string floored to nearest 1/2 second
var_dump($c);

// D is a float string
var_dump($d);

// D is floored to nearest multiple of the clock's resolution
var_dump((int) sprintf('%0-9.9s', $dParts[1]) % $clockRes == 0);

--EXPECTF--
string(%d) "%d.%d"
string(%d) "%d.%d"
bool(true)
string(%d) "%d.%r(0|5)%r"
string(%d) "%d.%d"
bool(true)
