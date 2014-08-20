/*
	Copyright 2011 Adrian Guenter	<adrianguenter@gmail.com>
	
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

#ifndef PHP_POSIXCLOCKS_H
#define PHP_POSIXCLOCKS_H

extern zend_module_entry posixclocks_module_entry;
#define phpext_posixclocks_ptr &posixclocks_module_entry

#ifdef PHP_WIN32
#	define PHP_POSIXCLOCKS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_POSIXCLOCKS_API __attribute__ ((visibility("default")))
#else
#	define PHP_POSIXCLOCKS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(posixclocks);
PHP_MSHUTDOWN_FUNCTION(posixclocks);
PHP_RINIT_FUNCTION(posixclocks);
PHP_RSHUTDOWN_FUNCTION(posixclocks);
PHP_MINFO_FUNCTION(posixclocks);

PHP_FUNCTION(posix_clock_gettime);
PHP_FUNCTION(posix_clock_getres);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(posixclocks)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(posixclocks)
*/

/* In every utility function you add that needs to use variables 
   in php_posixclocks_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as POSIXCLOCKS_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define POSIXCLOCKS_G(v) TSRMG(posixclocks_globals_id, zend_posixclocks_globals *, v)
#else
#define POSIXCLOCKS_G(v) (posixclocks_globals.v)
#endif

#endif	/* PHP_POSIXCLOCKS_H */

