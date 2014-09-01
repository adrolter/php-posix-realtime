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


/*
 * Includes
 */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_posixclocks.h"
#include "posixclocks_ver.h"


/*
 * Macros
 */

#define BILLION_LD  1000000000.0L

#define CONSTFLAGS       CONST_CS | CONST_PERSISTENT
#define RETTYPE_TIMESPEC 0
#define RETTYPE_FLOAT    1
#define RETTYPE_STRING   2

#define INTLEN(val) \
  (size_t) ((val >= 0 && val < 10) ? 1 : floor(log10(abs(val))) + (val < 0 ? 2 : 1))

#define TIMESPEC_TO_LDOUBLE(ts) \
  (ts.tv_sec + (ts.tv_nsec / BILLION_LD))

#define STRINGIFY(str) #str

#define DEFINE_CLOCK(clock_id) \
  REGISTER_LONG_CONSTANT(STRINGIFY(PSXCLK_CLOCK_ ## clock_id), CLOCK_ ## clock_id, CONSTFLAGS)


/*
 * Static globals
 */

static int le_posixclocks;

static char *timespec_to_string(struct timespec const *p_ts)
{
  // (char size * (digits in seconds + 1 for decimal point + 9 for nanoseconds)) + 1 for \0
  size_t const result_sz = (sizeof(char) * (INTLEN(p_ts->tv_sec) + 1 + 9)) + 1;
  char *p_result = malloc(result_sz);

  if (!p_result) {
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to allocate memory [%s] (%s)", __func__, strerror(errno));
    return NULL;
  }

  snprintf(p_result, result_sz, "%jd.%.9ld", (intmax_t) p_ts->tv_sec, p_ts->tv_nsec);

  return p_result;
}

static zval *timespec_to_zval(struct timespec const *p_ts)
{
  zval *p_obj;

  MAKE_STD_ZVAL(p_obj);
  object_init(p_obj);

  if (sizeof(p_ts->tv_sec) <= SIZEOF_LONG) {
    add_property_long(p_obj, "tv_sec", p_ts->tv_sec);
  }
  else {
    char secstr[(size_t) (INTLEN(p_ts->tv_sec) + 1)];
    snprintf(secstr, sizeof(secstr), "%d", p_ts->tv_sec);
    add_property_string(p_obj, "tv_sec", secstr, 1);
  }

  add_property_long(p_obj, "tv_nsec", p_ts->tv_nsec);

  return p_obj;
}


/*
 * PHP callbacks
 */

PHP_MINIT_FUNCTION(posixclocks)
{
  REGISTER_LONG_CONSTANT("PSXCLK_AS_TIMESPEC", RETTYPE_TIMESPEC, CONSTFLAGS);
  REGISTER_LONG_CONSTANT("PSXCLK_AS_FLOAT", RETTYPE_FLOAT, CONSTFLAGS);
  REGISTER_LONG_CONSTANT("PSXCLK_AS_STRING", RETTYPE_STRING, CONSTFLAGS);

  DEFINE_CLOCK(REALTIME);

  #ifdef CLOCK_MONOTONIC
    DEFINE_CLOCK(MONOTONIC);
  #endif

  #ifdef CLOCK_PROCESS_CPUTIME_ID
    DEFINE_CLOCK(PROCESS_CPUTIME_ID);
  #endif

  #ifdef CLOCK_THREAD_CPUTIME_ID
    DEFINE_CLOCK(THREAD_CPUTIME_ID);
  #endif

  #ifdef CLOCK_MONOTONIC_RAW
    DEFINE_CLOCK(MONOTONIC_RAW);
  #endif

  #ifdef CLOCK_REALTIME_COARSE
    DEFINE_CLOCK(REALTIME_COARSE);
  #endif

  #ifdef CLOCK_MONOTONIC_COARSE
    DEFINE_CLOCK(MONOTONIC_COARSE);
  #endif

  #ifdef CLOCK_BOOTTIME
    DEFINE_CLOCK(BOOTTIME);
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

  #define PRINTINFO_SUPPORTED(clock_id)                                \
    clock_getres(CLOCK_ ## clock_id, &clock_res);                      \
    snprintf(precision, 50, "%.0le", TIMESPEC_TO_LDOUBLE(clock_res));  \
    php_info_print_table_row(3, #clock_id, "Yes", precision)

  #define PRINTINFO_UNSUPPORTED(clock_id) \
    php_info_print_table_row(3, #clock_id, "No", "")

  php_info_print_table_start();
  php_info_print_table_row(2, "POSIX Clocks Support", "enabled");
  php_info_print_table_row(2, "Version", PHP_PSXCLK_VERSION);
  php_info_print_table_end();

  php_info_print_table_start();
  php_info_print_table_header(3, "Clock", "Supported", "Precision");

  PRINTINFO_SUPPORTED(REALTIME);

  #ifdef CLOCK_MONOTONIC
    PRINTINFO_SUPPORTED(MONOTONIC);
  #else
    PRINTINFO_UNSUPPORTED(MONOTONIC);
  #endif

  #ifdef CLOCK_PROCESS_CPUTIME_ID
    PRINTINFO_SUPPORTED(PROCESS_CPUTIME_ID);
  #else
    PRINTINFO_UNSUPPORTED(PROCESS_CPUTIME_ID);
  #endif

  #ifdef CLOCK_THREAD_CPUTIME_ID
    PRINTINFO_SUPPORTED(THREAD_CPUTIME_ID);
  #else
    PRINTINFO_UNSUPPORTED(THREAD_CPUTIME_ID);
  #endif

  #ifdef CLOCK_MONOTONIC_RAW
    PRINTINFO_SUPPORTED(MONOTONIC_RAW);
  #else
    PRINTINFO_UNSUPPORTED(MONOTONIC_RAW);
  #endif

  #ifdef CLOCK_REALTIME_COARSE
    PRINTINFO_SUPPORTED(REALTIME_COARSE);
  #else
    PRINTINFO_UNSUPPORTED(REALTIME_COARSE);
  #endif

  #ifdef CLOCK_MONOTONIC_COARSE
    PRINTINFO_SUPPORTED(MONOTONIC_COARSE);
  #else
    PRINTINFO_UNSUPPORTED(MONOTONIC_COARSE);
  #endif

  #ifdef CLOCK_BOOTTIME
    PRINTINFO_SUPPORTED(BOOTTIME);
  #else
    PRINTINFO_UNSUPPORTED(BOOTTIME);
  #endif

  php_info_print_table_end();
}


PHP_FUNCTION(posix_clock_gettime)
{
  long clock_id     = CLOCK_REALTIME;
  long return_type  = RETTYPE_TIMESPEC;
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
    case RETTYPE_TIMESPEC:
      RETURN_ZVAL(timespec_to_zval(&clock_val), 1, 1);
      break;
    case RETTYPE_FLOAT:
      RETURN_DOUBLE(TIMESPEC_TO_LDOUBLE(clock_val));
      break;
    case RETTYPE_STRING:
      RETURN_STRING(timespec_to_string(&clock_val), 1);
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

  RETURN_DOUBLE(TIMESPEC_TO_LDOUBLE(clock_res));
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
