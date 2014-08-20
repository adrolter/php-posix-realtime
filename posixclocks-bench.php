<?php
/*
  Copyright 2011 Adrian Guenter  <adrianguenter@gmail.com>

  This file is part of php_posixclocks and licensed
  under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

echo "\n     PHP_POSIXCLOCKS BENCHMARK    \n";
echo "=================================\n";

// Number of times to run each function
define('ITERATIONS', 1000000);

// posix_clock_gettime()
$start = microtime(true);
for ($i = 0; $i < ITERATIONS; $i++) { $x = posix_clock_gettime(CLOCK_MONOTONIC); }
$gtTotal = (microtime(true) - $start);

// Take a break...
sleep(1);

// microtime()
$start = microtime(true);
for ($i = 0; $i < ITERATIONS; $i++) { $x = microtime(true); }
$mtTotal = (microtime(true) - $start);


// Output
echo " posix_clock_gettime:\t" . $gtTotal . "\n";
echo " microtime:\t" . $mtTotal . "\n";
echo "---------------------------------\n";

if ($gtTotal < $mtTotal) {
  $pcnt = round((($mtTotal / $gtTotal) * 100) - 100, 3);
  echo " posix_clock_gettime was {$pcnt}% faster\n";
}
else {
  $pcnt = round((($gtTotal / $mtTotal) * 100) - 100, 3);
  echo " microtime was {$pcnt}% faster\n";
}

echo "---------------------------------\n";
?>