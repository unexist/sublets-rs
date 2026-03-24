//
// @package sublets-rs
//
// @file CPU plugin functions
// @copyright (c) 2026-present Christoph Kappel <christoph@unexist.dev>
// @version $Id$
//
// This program can be distributed under the terms of the GNU GPLv3.
// See the file LICENSE for details.
//

#include <stdint.h>

#define IMPORT(a, b) __attribute__((import_module(a), import_name(b)))

IMPORT("extism:host/user", "get_cpu")
uint32_t get_cpu_impl(void);

uint32_t get_cpu(void) {
  return get_cpu_impl();
}