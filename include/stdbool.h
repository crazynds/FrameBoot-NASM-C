/*===---- stdbool.h - Standard header for booleans -------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */
 
#ifndef __STDBOOL_H
#define __STDBOOL_H
 
#define __bool_true_false_are_defined 1

#ifndef bool
#define bool char
#endif

#define _Bool bool
#define true 1
#define false 0
 
#endif /* __STDBOOL_H */