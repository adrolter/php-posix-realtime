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
#include "posixclocks_ver.h"

static int le_posixclocks;


PHP_MINIT_FUNCTION(posixclocks)
{
  REGISTER_LONG_CONSTANT("PSXCLK_AS_TIMESPEC", PHP_PSXCLK_RETTYPE_TIMESPEC, PHP_PSXCLK_CONSTFLAGS);
  REGISTER_LONG_CONSTANT("PSXCLK_AS_FLOAT", PHP_PSXCLK_RETTYPE_FLOAT, PHP_PSXCLK_CONSTFLAGS);
  REGISTER_LONG_CONSTANT("PSXCLK_AS_STRING", PHP_PSXCLK_RETTYPE_STRING, PHP_PSXCLK_CONSTFLAGS);

  PHP_PSXCLK_DEFINE_CLOCK(REALTIME);

  #ifdef CLOCK_MONOTONIC
    PHP_PSXCLK_DEFINE_CLOCK(MONOTONIC);
  #endif

  #ifdef CLOCK_PROCESS_CPUTIME_ID
    PHP_PSXCLK_DEFINE_CLOCK(PROCESS_CPUTIME_ID);
  #endif

  #ifdef CLOCK_THREAD_CPUTIME_ID
    PHP_PSXCLK_DEFINE_CLOCK(THREAD_CPUTIME_ID);
  #endif

  #ifdef CLOCK_MONOTONIC_RAW
    PHP_PSXCLK_DEFINE_CLOCK(MONOTONIC_RAW);
  #endif

  #ifdef CLOCK_REALTIME_COARSE
    PHP_PSXCLK_DEFINE_CLOCK(REALTIME_COARSE);
  #endif

  #ifdef CLOCK_MONOTONIC_COARSE
    PHP_PSXCLK_DEFINE_CLOCK(MONOTONIC_COARSE);
  #endif

  #ifdef CLOCK_BOOTTIME
    PHP_PSXCLK_DEFINE_CLOCK(BOOTTIME);
  #endif

  return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(posixclocks)
{
  return SUCCESS;
}


PHP_MINFO_FUNCTION(posixclocks)
{
  char precision[50];
  struct timespec clock_res;

  #define PHP_PSXCLK_PRINTINFO_SUPPORTED(clock_id)                                \
    clock_getres(CLOCK_ ## clock_id, &res);                                       \
    snprintf(precision, 50, "%.0le", PHP_PSXCLK_TIMESPEC_TO_LDOUBLE(clock_res));  \
    php_info_print_table_row(3, #clock_id, "Yes", precision)

  #define PHP_PSXCLK_PRINTINFO_UNSUPPORTED(clock_id) \
    php_info_print_table_row(3, #clock_id, "No", "")

  php_info_print_table_start();
  php_info_print_table_row(2, "POSIX Clocks Support", "enabled");
  php_info_print_table_row(2, "Version", PHP_PSXCLK_VERSION);
  php_info_print_table_end();

  php_info_print_table_start();
  php_info_print_table_header(3, "Clock", "Supported", "Precision");

  PHP_PSXCLK_PRINTINFO_SUPPORTED(REALTIME);

  #ifdef CLOCK_MONOTONIC
    PHP_PSXCLK_PRINTINFO_SUPPORTED(MONOTONIC);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(MONOTONIC);
  #endif

  #ifdef CLOCK_PROCESS_CPUTIME_ID
    PHP_PSXCLK_PRINTINFO_SUPPORTED(PROCESS_CPUTIME_ID);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(PROCESS_CPUTIME_ID);
  #endif

  #ifdef CLOCK_THREAD_CPUTIME_ID
    PHP_PSXCLK_PRINTINFO_SUPPORTED(THREAD_CPUTIME_ID);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(THREAD_CPUTIME_ID);
  #endif

  #ifdef CLOCK_MONOTONIC_RAW
    PHP_PSXCLK_PRINTINFO_SUPPORTED(MONOTONIC_RAW);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(MONOTONIC_RAW);
  #endif

  #ifdef CLOCK_REALTIME_COARSE
    PHP_PSXCLK_PRINTINFO_SUPPORTED(REALTIME_COARSE);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(REALTIME_COARSE);
  #endif

  #ifdef CLOCK_MONOTONIC_COARSE
    PHP_PSXCLK_PRINTINFO_SUPPORTED(MONOTONIC_COARSE);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(MONOTONIC_COARSE);
  #endif

  #ifdef CLOCK_BOOTTIME
    PHP_PSXCLK_PRINTINFO_SUPPORTED(BOOTTIME);
  #else
    PHP_PSXCLK_PRINTINFO_UNSUPPORTED(BOOTTIME);
  #endif

  php_info_print_table_end();
}


PHP_FUNCTION(posix_clock_gettime)
{
  long clock_id     = CLOCK_REALTIME;
  long return_type  = PHP_PSXCLK_RETTYPE_TIMESPEC;
  struct timespec clock_val;

  if (ZEND_NUM_ARGS() > 2) {
    WRONG_PARAM_COUNT;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ll", &clock_id, &return_type) != SUCCESS) {
    return;
  }

  if (clock_gettime(clock_id, &clock_val) == -1) {
    if (errno == EINVAL) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clock_id);
      return;
    }
    else if (errno == EFAULT) {
      return;
    }
  }

  switch (return_type) {
    case PHP_PSXCLK_RETTYPE_TIMESPEC:
    {
      zval *obj;
      MAKE_STD_ZVAL(obj);
      object_init(obj);
      add_property_long(obj, "tv_sec", clockVal.tv_sec);
      add_property_long(obj, "tv_nsec", clockVal.tv_nsec);
      RETURN_ZVAL(obj, 1, 1);
      break;
    }
    case PHP_PSXCLK_RETTYPE_FLOAT:
      RETURN_DOUBLE(PHP_PSXCLK_TIMESPEC_TO_LDOUBLE(clock_val));
      break;
    case PHP_PSXCLK_RETTYPE_STRING:
      //
      break;
    default:
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "Return type must be one of: PSXCLK_CLOCK_RET_TIMESPEC, "
        "PSXCLK_CLOCK_RET_FLOAT, PSXCLK_CLOCK_RET_STRING");
      return;
  }
}


PHP_FUNCTION(posix_clock_getres)
{
  long clock_id = CLOCK_REALTIME;
  double result;
  struct timespec clock_res;

  if (ZEND_NUM_ARGS() > 1) {
    WRONG_PARAM_COUNT;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &clock_id) != SUCCESS) {
    return;
  }

  if (clock_getres(clock_id, &clock_res) == -1) {
    if (errno == EINVAL) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clock_id);
      return;
    }
    else if (errno == EFAULT) {
      return;
    }
  }

  RETURN_DOUBLE(PHP_PSXCLK_TIMESPEC_TO_LDOUBLE(clock_res));
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
  PHP_PSXCLK_VERSION,
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
