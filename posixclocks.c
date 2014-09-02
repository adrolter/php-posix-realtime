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

#define BILLION_LD       1000000000.0L
#define CONSTFLAGS       CONST_CS | CONST_PERSISTENT
#define RETTYPE_TIMESPEC 0
#define RETTYPE_FLOAT    1
#define RETTYPE_STRING   2

#define STR(str) #str

#define INTLEN(val) \
  (size_t) ((val >= 0 && val < 10) ? 1 : floor(log10(abs(val))) + (val < 0 ? 2 : 1))

#define TIMESPEC_TO_LDOUBLE(ts, res_nsec) \
  (ts.tv_sec + ( \
    (res_nsec > 1 ? (ts.tv_nsec - (ts.tv_nsec % res_nsec)) : ts.tv_nsec) / BILLION_LD \
  ))

#define DEFINE_CLOCK(clock_id) \
  REGISTER_LONG_CONSTANT(STR(PSXCLK_CLOCK_ ## clock_id), CLOCK_ ## clock_id, CONSTFLAGS)


/*
 * Static globals
 */

static int le_posixclocks;

static char * timespec_to_string(struct timespec const * ts_p, long const res_nsec)
{
  // (char size * (digits in seconds + 1 for decimal point + 9 for nanoseconds)) + 1 for \0
  size_t const result_sz = (sizeof(char) * (INTLEN(ts_p->tv_sec) + 1 + 9)) + 1;
  long decimal = ts_p->tv_nsec;
  char * result_p;

  result_p = emalloc(result_sz);
  if (!result_p) {
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to allocate memory [%s] (%s)", __func__, strerror(errno));
    return NULL;
  }

  // Apply resolution if provided
  // Only apply if > 1 here as result would be unchanged for MOD(1)
  if (res_nsec > 1) {
    decimal -= decimal % res_nsec;
  }
  
  // Remove trailing zeros for decimal string representation
  while (decimal % 10 == 0) {
    decimal /= 10;
  }

  snprintf(result_p, result_sz, "%jd.%ld", (intmax_t) ts_p->tv_sec, decimal);

  return result_p;
}

static zval * timespec_to_zval(struct timespec const * ts_p, long const res_nsec)
{
  long nsec = ts_p->tv_nsec;
  zval * obj_p;

  MAKE_STD_ZVAL(obj_p);
  object_init(obj_p);

  // Apply resolution if provided (res_nsec is greater than zero nanoseconds)
  // Do even if resolution is 1ns so that extra properties are added to object
  if (res_nsec > 0) {
    if (res_nsec != 1) {
      nsec -= nsec % res_nsec;
    }
    add_property_long(obj_p, "res_nsec", res_nsec);
    add_property_long(obj_p, "tv_nsec_raw", ts_p->tv_nsec);
  }

  if (sizeof(ts_p->tv_sec) <= SIZEOF_LONG) {
    add_property_long(obj_p, "tv_sec", ts_p->tv_sec);
  }
  else {
    char secstr[(size_t) (INTLEN(ts_p->tv_sec) + 1)];
    snprintf(secstr, sizeof(secstr), "%d", ts_p->tv_sec);
    add_property_string(obj_p, "tv_sec", secstr, 0);
  }

  add_property_long(obj_p, "tv_nsec", nsec);

  return obj_p;
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
    snprintf(precision, 50, "%.0le", TIMESPEC_TO_LDOUBLE(clock_res, 0));  \
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
  long clock_id              = CLOCK_REALTIME;
  long return_type           = RETTYPE_TIMESPEC;
  long resolution            = 0;
  zend_bool apply_resolution = 0;
  struct timespec clock_val;

  if (ZEND_NUM_ARGS() > 3) {
    WRONG_PARAM_COUNT;
  }

  // TODO: Add boolean to decide whether or not to apply resolution to nanoseconds before returning
  //  If so then do clock_getres and pass appropriately to conversion functions
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|llb", &clock_id, &return_type, &apply_resolution) != SUCCESS) {
    return;
  }

  if (clock_gettime(clock_id, &clock_val) == -1) {
    if (errno == EINVAL) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clock_id);
    }
    return;
  }
  
  if (apply_resolution) {  
    struct timespec clock_res;
    if (clock_getres(clock_id, &clock_res) == -1) {
      return;
    }
    resolution = clock_res.tv_nsec;
  }

  switch (return_type) {
    case RETTYPE_TIMESPEC:
      RETURN_ZVAL(timespec_to_zval(&clock_val, resolution), 0, 1);
      break;
    case RETTYPE_FLOAT:
      RETURN_DOUBLE(TIMESPEC_TO_LDOUBLE(clock_val, resolution));
      break;
    case RETTYPE_STRING:
      RETURN_STRING(timespec_to_string(&clock_val, resolution), 0);
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

  RETURN_DOUBLE(TIMESPEC_TO_LDOUBLE(clock_res, 0));
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
