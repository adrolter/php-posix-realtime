<?php

define('CLOCK_REALTIME', 0);

define('CLOCK_MONOTONIC', 1);

define('CLOCK_PROCESS_CPUTIME_ID', 2);

define('CLOCK_THREAD_CPUTIME_ID', 3);

function clock_gettime (int $clock_id_flag) {}

function clock_getres (int $clock_id_flag) {}
