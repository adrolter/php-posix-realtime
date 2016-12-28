/*
  Copyright 2014 Adrian Guenter  <adrianguenter@gmail.com>

  This file is part of php_posixrealtime and licensed
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

#include <stdint.h>

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>

#include "php_posixrealtime.h"
#include "posixrealtime_ver.h"


/*
 * Macros
 */

#define BILLION_D   1000000000.0
#define BILLION_LD  1000000000.0L
#define FLOOR_TO_CLOCKRES -1
#define CONSTFLAGS  CONST_CS | CONST_PERSISTENT

#define STR(str) #str

#define INTLEN(val) \
    (size_t) ((val >= 0 && val < 10) ? 1 : floor(log10(abs(val))) + (val < 0 ? 2 : 1))

#define TIMESPEC_TO_DOUBLE(ts) (ts.tv_sec + (ts.tv_nsec / BILLION_D))

#define TIMESPEC_TO_LDOUBLE(ts) (ts.tv_sec + (ts.tv_nsec / BILLION_LD))

#define DEFINE_CLOCK(id) \
    REGISTER_LONG_CONSTANT(STR(PSXRT_CLK_ ## id), CLOCK_ ## id, CONSTFLAGS)

typedef enum {
    TIMESPEC = 10,
    FLOAT = 20,
    STRING = 30
} ReturnType;


/*
 * Static globals
 */

static int le_posixrealtime;

static char * timespec_to_string(struct timespec const * ts_p)
{
#    ifdef ZTS
    TSRMLS_FETCH();
#    endif

    long decimal = ts_p->tv_nsec;
    size_t result_sz;
    char * result_p;

    /* Remove trailing zeros for decimal string representation */
    while (decimal > 0 && decimal % 10 == 0) {
        decimal /= 10;
    }

    result_sz = (sizeof (char) * (INTLEN(ts_p->tv_sec) + 1 + INTLEN(decimal))) + 1;

    result_p = emalloc(result_sz);
    if (!result_p) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to allocate memory [%s] (%s)", __func__, strerror(errno));
        return NULL;
    }

    snprintf(result_p, result_sz, "%jd.%ld", (intmax_t) ts_p->tv_sec, decimal);

    return result_p;
}

static zval * timespec_to_zval(struct timespec const * ts_p)
{
#    ifdef ZTS
    TSRMLS_FETCH();
#    endif
    
    long nsec = ts_p->tv_nsec;
    zval * obj_p;

    MAKE_STD_ZVAL(obj_p);
    object_init(obj_p);

    // TODO: Check value against LONG_MAX instead?
    if (sizeof (ts_p->tv_sec) <= SIZEOF_LONG) {
        add_property_long(obj_p, "tv_sec", ts_p->tv_sec);
    } else {
        char secstr[(size_t) (INTLEN(ts_p->tv_sec) + 1)];
        snprintf(secstr, sizeof (secstr), "%d", ts_p->tv_sec);
        add_property_string(obj_p, "tv_sec", secstr, 0);
    }

    add_property_long(obj_p, "tv_nsec", nsec);

    return obj_p;
}

/*
 * PHP callbacks
 */

PHP_MINIT_FUNCTION(posixrealtime)
{
    REGISTER_LONG_CONSTANT("PSXRT_AS_TIMESPEC", TIMESPEC, CONSTFLAGS);
    REGISTER_LONG_CONSTANT("PSXRT_AS_FLOAT", FLOAT, CONSTFLAGS);
    REGISTER_LONG_CONSTANT("PSXRT_AS_STRING", STRING, CONSTFLAGS);
    REGISTER_LONG_CONSTANT("PSXRT_FLOOR_TO_CLOCKRES", FLOOR_TO_CLOCKRES, CONSTFLAGS);

    DEFINE_CLOCK(REALTIME);

#    ifdef CLOCK_MONOTONIC
    DEFINE_CLOCK(MONOTONIC);
#    endif

#    ifdef CLOCK_PROCESS_CPUTIME_ID
    DEFINE_CLOCK(PROCESS_CPUTIME_ID);
#    endif

#    ifdef CLOCK_THREAD_CPUTIME_ID
    DEFINE_CLOCK(THREAD_CPUTIME_ID);
#    endif

#    ifdef CLOCK_MONOTONIC_RAW
    DEFINE_CLOCK(MONOTONIC_RAW);
#    endif

#    ifdef CLOCK_REALTIME_COARSE
    DEFINE_CLOCK(REALTIME_COARSE);
#    endif

#    ifdef CLOCK_MONOTONIC_COARSE
    DEFINE_CLOCK(MONOTONIC_COARSE);
#    endif

#    ifdef CLOCK_BOOTTIME
    DEFINE_CLOCK(BOOTTIME);
#    endif

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(posixrealtime)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(posixrealtime)
{
    char precision[50];
    struct timespec clock_res;

#    define PRINTINFO_SUPPORTED(clock_id)                              \
    clock_getres(CLOCK_ ## clock_id, &clock_res);                      \
    snprintf(precision, 50, "%.0Le", TIMESPEC_TO_LDOUBLE(clock_res));  \
    php_info_print_table_row(3, #clock_id, "Yes", precision)

#    define PRINTINFO_UNSUPPORTED(clock_id) \
    php_info_print_table_row(3, #clock_id, "No", "")

    php_info_print_table_start();
    php_info_print_table_row(2, "POSIX Realtime Support", "enabled");
    php_info_print_table_row(2, "Version", PHP_PSXRT_VERSION);
    php_info_print_table_end();

    php_info_print_table_start();
    php_info_print_table_header(3, "Clock", "Supported", "Precision");

    PRINTINFO_SUPPORTED(REALTIME);

#    ifdef CLOCK_MONOTONIC
    PRINTINFO_SUPPORTED(MONOTONIC);
#    else
    PRINTINFO_UNSUPPORTED(MONOTONIC);
#    endif

#    ifdef CLOCK_PROCESS_CPUTIME_ID
    PRINTINFO_SUPPORTED(PROCESS_CPUTIME_ID);
#    else
    PRINTINFO_UNSUPPORTED(PROCESS_CPUTIME_ID);
#    endif

#    ifdef CLOCK_THREAD_CPUTIME_ID
    PRINTINFO_SUPPORTED(THREAD_CPUTIME_ID);
#    else
    PRINTINFO_UNSUPPORTED(THREAD_CPUTIME_ID);
#    endif

#    ifdef CLOCK_MONOTONIC_RAW
    PRINTINFO_SUPPORTED(MONOTONIC_RAW);
#    else
    PRINTINFO_UNSUPPORTED(MONOTONIC_RAW);
#    endif

#    ifdef CLOCK_REALTIME_COARSE
    PRINTINFO_SUPPORTED(REALTIME_COARSE);
#    else
    PRINTINFO_UNSUPPORTED(REALTIME_COARSE);
#    endif

#    ifdef CLOCK_MONOTONIC_COARSE
    PRINTINFO_SUPPORTED(MONOTONIC_COARSE);
#    else
    PRINTINFO_UNSUPPORTED(MONOTONIC_COARSE);
#    endif

#    ifdef CLOCK_BOOTTIME
    PRINTINFO_SUPPORTED(BOOTTIME);
#    else
    PRINTINFO_UNSUPPORTED(BOOTTIME);
#    endif

    php_info_print_table_end();
}

PHP_FUNCTION(posix_clock_gettime)
{
    long clock_id = CLOCK_REALTIME;
    long clock_val_nsec_raw = -1;
    long return_type = STRING;
    long floor_to_ns = 0;
    struct timespec clock_val;

    if (ZEND_NUM_ARGS() > 3) {
        WRONG_PARAM_COUNT;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lll", &clock_id, &return_type, &floor_to_ns) != SUCCESS) {
        return;
    }

    if (clock_gettime(clock_id, &clock_val) == -1) {
        if (errno == EINVAL) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "The POSIX clock ID '%ld' is not supported on this system", clock_id);
        }
        return;
    }

    if (floor_to_ns) {
        if (floor_to_ns == FLOOR_TO_CLOCKRES) {
            struct timespec clock_res;
            if (clock_getres(clock_id, &clock_res) == -1) {
                return;
            }
            floor_to_ns = clock_res.tv_nsec;
        }
        clock_val_nsec_raw = clock_val.tv_nsec;
        clock_val.tv_nsec -= clock_val.tv_nsec % floor_to_ns;
    }

    switch (return_type) {
    case TIMESPEC:
    {
        zval * obj_p = timespec_to_zval(&clock_val);
        if (floor_to_ns) {
            add_property_long(obj_p, "floored_to_nsec", floor_to_ns);
            add_property_long(obj_p, "tv_nsec_raw", clock_val_nsec_raw);
        }
        RETURN_ZVAL(obj_p, 0, 1);
        break;
    }
    case FLOAT:
        RETURN_DOUBLE(TIMESPEC_TO_DOUBLE(clock_val));
        break;
    case STRING:
        RETURN_STRING(timespec_to_string(&clock_val), 0);
        break;
    default:
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Return type must be one of: PSXRT_AS_TIMESPEC, "
                "PSXRT_AS_FLOAT, PSXRT_AS_STRING");
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
        } else if (errno == EFAULT) {
            return;
        }
    }

    RETURN_LONG(clock_res.tv_nsec);
}

PHP_FUNCTION(posix_is_clock_supported)
{
    long clock_id;
	struct timespec tp;

    if (ZEND_NUM_ARGS() > 1) {
        WRONG_PARAM_COUNT;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &clock_id) != SUCCESS) {
        return;
    }

    if (clock_getres(clock_id, &tp) == -1) {
        if (errno == EINVAL) {
            RETURN_FALSE;
		}
    }

    RETURN_TRUE;
}


const zend_function_entry posixrealtime_functions[] = {
    PHP_FE(posix_clock_gettime, NULL)
    PHP_FE(posix_clock_getres, NULL)
    PHP_FE(posix_is_clock_supported, NULL)
    PHP_FE_END
};


zend_module_entry posixrealtime_module_entry = {
    STANDARD_MODULE_HEADER,
    "posixrealtime",
    posixrealtime_functions,
    PHP_MINIT(posixrealtime),
    PHP_MSHUTDOWN(posixrealtime),
    NULL, /* PHP_RINIT */
    NULL, /* PHP_RSHUTDOWN */
    PHP_MINFO(posixrealtime),
    PHP_PSXRT_VERSION,
    STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_POSIXREALTIME
ZEND_GET_MODULE(posixrealtime)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
