--TEST--
Object output test for posix_clock_gettime()
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php
function test_xuWKgAe16e($obj, $floor_to, $clockRes)
{
	$nanosecsInSec = pow(10, 9);

	// $obj is a "stdClass" object
	if (!is_object($obj) || !($obj instanceof \stdClass)) {
		return false;
	}

	// $obj has a tv_sec property of type integer
	if (!isset($obj->tv_sec) || !is_int($obj->tv_sec)) {
		return false;
	}

	// $obj has a tv_nsec property of type integer >= 0 && < 10^9
	if (
		!isset($obj->tv_nsec) ||
		!is_int($obj->tv_nsec) ||
		!($obj->tv_nsec >= 0) ||
		!($obj->tv_nsec < $nanosecsInSec)
	) {
		return false;
	}

	if ($floor_to === false ) {
		if (isset($obj->floored_to_nsec) || isset($obj->tv_nsec_raw)) {
			return false;
		}
	}
	else {
		// $obj has a floored_to_nsec property of type integer
		if (!isset($obj->floored_to_nsec) || !is_int($obj->floored_to_nsec)) {
			return false;
		}

		// $obj has a tv_nsec_raw property of type integer
		if (!isset($obj->tv_nsec_raw) || !is_int($obj->tv_nsec_raw)) {
			return false;
		}

		if ($floor_to === PSXRT_FLOOR_TO_CLOCKRES) {
			// floored_to_nsec is equal to the clock's reported resolution
			if ($obj->floored_to_nsec !== $clockRes) {
				return false;
			}
		}

		if ($obj->tv_nsec % $obj->floored_to_nsec !== 0) {
			return false;
		}
	}

	return true;
}

// Test against a "coarse" clock if available for non-1ns resolution in tests
$clockId = defined('PSXRT_CLK_REALTIME_COARSE') ? PSXRT_CLK_REALTIME_COARSE : PSXRT_CLK_REALTIME;
$returnAs = PSXRT_AS_TIMESPEC;
$clockRes = posix_clock_getres($clockId);
$a = posix_clock_gettime($clockId, $returnAs);
$b = posix_clock_gettime($clockId, $returnAs, false);
$d = posix_clock_gettime($clockId, $returnAs, PSXRT_FLOOR_TO_CLOCKRES);

var_dump(test_xuWKgAe16e($a, false, $clockRes));

var_dump(test_xuWKgAe16e($b, false, $clockRes));

var_dump(test_xuWKgAe16e($d, PSXRT_FLOOR_TO_CLOCKRES, $clockRes));

--EXPECTF--
bool(true)
bool(true)
bool(true)
