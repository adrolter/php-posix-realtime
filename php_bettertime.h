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

#ifndef PHP_BETTERTIME_H
#define PHP_BETTERTIME_H

extern zend_module_entry bettertime_module_entry;
#define phpext_bettertime_ptr &bettertime_module_entry

#ifdef PHP_WIN32
#	define PHP_BETTERTIME_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_BETTERTIME_API __attribute__ ((visibility("default")))
#else
#	define PHP_BETTERTIME_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(bettertime);
PHP_MSHUTDOWN_FUNCTION(bettertime);
PHP_RINIT_FUNCTION(bettertime);
PHP_RSHUTDOWN_FUNCTION(bettertime);
PHP_MINFO_FUNCTION(bettertime);

PHP_FUNCTION(confirm_bettertime_compiled);	/* For testing, remove later. */
PHP_FUNCTION(clock_gettime);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(bettertime)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(bettertime)
*/

/* In every utility function you add that needs to use variables 
   in php_bettertime_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as BETTERTIME_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define BETTERTIME_G(v) TSRMG(bettertime_globals_id, zend_bettertime_globals *, v)
#else
#define BETTERTIME_G(v) (bettertime_globals.v)
#endif

#endif	/* PHP_BETTERTIME_H */

