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

#ifndef PHP_POSIXCLOCKS_H
#define PHP_POSIXCLOCKS_H

extern zend_module_entry posixclocks_module_entry;
#define phpext_posixclocks_ptr &posixclocks_module_entry


#ifdef PHP_WIN32
#  define PHP_POSIXCLOCKS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_POSIXCLOCKS_API __attribute__ ((visibility("default")))
#else
#  define PHP_POSIXCLOCKS_API
#endif


#ifdef ZTS
#include "TSRM.h"
#endif


#ifdef ZTS
#define POSIXCLOCKS_G(v) TSRMG(posixclocks_globals_id, zend_posixclocks_globals *, v)
#else
#define POSIXCLOCKS_G(v) (posixclocks_globals.v)
#endif

#endif

