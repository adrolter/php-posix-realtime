/*
  Copyright 2014 Adrian Guenter  <adrianguenter@gmail.com>

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
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_posixclocks.h"


static int le_posixclocks;


PHP_MINIT_FUNCTION(posixclocks)
{
  REGISTER_LONG_CONSTANT("POSIX_CLOCK_REALTIME", CLOCK_REALTIME, CONST_CS | CONST_PERSISTENT);

  #ifdef CLOCK_MONOTONIC
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_MONOTONIC", CLOCK_MONOTONIC, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_PROCESS_CPUTIME_ID
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_THREAD_CPUTIME_ID
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_MONOTONIC_RAW
      REGISTER_LONG_CONSTANT("POSIX_CLOCK_MONOTONIC_RAW", CLOCK_MONOTONIC_RAW, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_REALTIME_COARSE
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_REALTIME_COARSE", CLOCK_REALTIME_COARSE, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_MONOTONIC_COARSE
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_MONOTONIC_COARSE", CLOCK_MONOTONIC_COARSE, CONST_CS | CONST_PERSISTENT);
  #endif

  #ifdef CLOCK_BOOTTIME
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_BOOTTIME", CLOCK_BOOTTIME, CONST_CS | CONST_PERSISTENT);
  #endif

  return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(posixclocks)
{
  return SUCCESS;
}


PHP_MINFO_FUNCTION(posixclocks)
{
  char isSupported[4];
  struct timespec res;
  double dResult;
  char precision[50];

  php_info_print_table_start();
  php_info_print_table_row(2, "POSIX Clocks Support", "enabled");
  php_info_print_table_end();

  php_info_print_table_start();
  php_info_print_table_header(2, "Clock", "Supported");
  php_info_print_table_row(2, "CLOCK_REALTIME", "Yes");

  #ifdef CLOCK_MONOTONIC
  strcpy(isSupported, "Yes");
  clock_getres(CLOCK_MONOTONIC, &res);
  dResult = res.tv_sec + res.tv_nsec / 1000000000.0;
  snprintf(precision, 50, "%f", dResult);
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(3, "CLOCK_MONOTONIC", &isSupported, precision);

  #ifdef CLOCK_PROCESS_CPUTIME_ID
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_PROCESS_CPUTIME_ID", &isSupported);

  #ifdef CLOCK_THREAD_CPUTIME_ID
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_THREAD_CPUTIME_ID", &isSupported);

  #ifdef CLOCK_MONOTONIC_RAW
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_MONOTONIC_RAW", &isSupported);

  #ifdef CLOCK_REALTIME_COARSE
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_REALTIME_COARSE", &isSupported);

  #ifdef CLOCK_MONOTONIC_COARSE
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_MONOTONIC_COARSE", &isSupported);

  #ifdef CLOCK_BOOTTIME
  strcpy(isSupported, "Yes");
  #else
  strcpy(isSupported, "No");
  #endif
  php_info_print_table_row(2, "CLOCK_BOOTTIME", &isSupported);

  php_info_print_table_end();
}


PHP_FUNCTION(posix_clock_gettime)
{
  long clkId = 0;
  struct timespec currTime;
  double dResult;

  if (ZEND_NUM_ARGS() > 1) {
    WRONG_PARAM_COUNT;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &clkId) != SUCCESS) {
    return;
  }

  if (clock_gettime(clkId, &currTime) == -1) {
    if (errno == EINVAL) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clkId);
      return;
    }
    else if (errno == EFAULT) {
      return;
    }
  }

  dResult = currTime.tv_sec + currTime.tv_nsec / 1000000000.0;

  RETURN_DOUBLE(dResult);
}


PHP_FUNCTION(posix_clock_getres)
{
  long clkId = 0;
  struct timespec res;
  double dResult;

  if (ZEND_NUM_ARGS() > 1) {
    WRONG_PARAM_COUNT;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &clkId) != SUCCESS) {
    return;
  }

  if (clock_getres(clkId, &res) == -1) {
    if (errno == EINVAL) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clkId);
      return;
    }
    else if (errno == EFAULT) {
      return;
    }
  }

  dResult = res.tv_sec + res.tv_nsec / 1000000000.0;

  RETURN_DOUBLE(dResult);
}


const zend_function_entry posixclocks_functions[] = {
  PHP_FE(posix_clock_gettime,  NULL)
  PHP_FE(posix_clock_getres,  NULL)
  PHP_FE_END
};


zend_module_entry posixclocks_module_entry = {
  STANDARD_MODULE_HEADER,
  "posixclocks",
  posixclocks_functions,
  PHP_MINIT(posixclocks),
  PHP_MSHUTDOWN(posixclocks),
  NULL,  /* PHP_RINIT */
  NULL,  /* PHP_RSHUTDOWN */
  PHP_MINFO(posixclocks),
  PHP_POSIXCLOCKS_VERSION,
  STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_POSIXCLOCKS
ZEND_GET_MODULE(posixclocks)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
