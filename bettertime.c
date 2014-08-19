/*
	Copyright 2011 Adrian Guenter	<adrianguenter@gmail.com>
	
	This file is part of php_bettertime and licensed
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
#include "php_bettertime.h"

/* If you declare any globals in php_bettertime.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(bettertime)
*/

/* True global resources - no need for thread safety here */
static int le_bettertime;

/* {{{ bettertime_functions[]
 *
 * Every user visible function must have an entry in bettertime_functions[].
 */
const zend_function_entry bettertime_functions[] = {
	PHP_FE(posix_clock_gettime,	NULL)
	PHP_FE(posix_clock_getres,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in bettertime_functions[] */
};
/* }}} */

/* {{{ bettertime_module_entry
 */
zend_module_entry bettertime_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"bettertime",
	bettertime_functions,
	PHP_MINIT(bettertime),
	PHP_MSHUTDOWN(bettertime),
	NULL,		/* PHP_RINIT (request init) */
	NULL,	/* PHP_RSHUTDOWN (request shutdown) */
	PHP_MINFO(bettertime),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BETTERTIME
ZEND_GET_MODULE(bettertime)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("bettertime.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_bettertime_globals, bettertime_globals)
    STD_PHP_INI_ENTRY("bettertime.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_bettertime_globals, bettertime_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_bettertime_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_bettertime_init_globals(zend_bettertime_globals *bettertime_globals)
{
	bettertime_globals->global_value = 0;
	bettertime_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bettertime)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	#ifdef CLOCK_REALTIME
    REGISTER_LONG_CONSTANT("POSIX_CLOCK_REALTIME", CLOCK_REALTIME, CONST_CS | CONST_PERSISTENT);
  #endif

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
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bettertime)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bettertime)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "bettertime support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto double posix_clock_gettime()
   clk_id) Get the current time of a clock */
PHP_FUNCTION(posix_clock_gettime)
{
	long clkId;
	struct timespec currTime;
	double dResult;
	
	if (ZEND_NUM_ARGS() != 1) {
		WRONG_PARAM_COUNT;
	}
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &clkId) == FAILURE) {
		return;
	}
	
	if (clock_gettime(clkId, &currTime) == -1) {
		
		if (errno == EINVAL) {
			
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "the 'clk_id' %ld is not supported on this system", (long) clkId);
			
			return;
		}
		else if (errno == EFAULT) {
			
			return;
		}
	}
	
	dResult = currTime.tv_sec + (double) currTime.tv_nsec / 1000000000.0;
	
	RETURN_DOUBLE(dResult);
}
/* }}} */

/* {{{ proto double posix_clock_getres()
   clk_id) Get the resolution of a clock */
PHP_FUNCTION(posix_clock_getres)
{
	long clkId;
	struct timespec res;
	double dResult;
	
	if (ZEND_NUM_ARGS() != 1) {
		WRONG_PARAM_COUNT;
	}
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &clkId) == FAILURE) {
		return;
	}
	
	if (clock_getres(clkId, &res) == -1) {
		
		if (errno == EINVAL) {
			
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "the 'clk_id' %ld is not supported on this system", (long) clkId);
			
			return;
		}
		else if (errno == EFAULT) {
			
			return;
		}
	}
	
	dResult = res.tv_sec + (double) res.tv_nsec / 1000000000.0;
	
	RETURN_DOUBLE(dResult);
}
/* }}} */

// WinNT support via GetTickCount/GetTickCount64?

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
