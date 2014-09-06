--TEST--
Basic test for posix_clock_gettime()
--SKIPIF--
<?php if (!extension_loaded("posixclocks")) print "skip"; ?>
--FILE--
<?php
var_dump(function_exists('posix_clock_gettime'));

var_dump(posix_clock_gettime());

--EXPECTF--
bool(true)
string(%d) "%d.%d"
