//
// @package sublets-rs
//
// @file Weather plugin functions
// @copyright (c) 2025-present Christoph Kappel <christoph@unexist.dev>
// @version $Id$
//
// This program can be distributed under the terms of the GNU GPLv3.
// See the file LICENSE for details.
//

#define EXTISM_IMPLEMENTATION
#include "extism-pdk.h"
#include <stdint.h>

int32_t EXTISM_EXPORTED_FUNCTION(run) {
  const char *reqStr = "{\
    \"method\": \"GET\",\
    \"url\": \"https://api.open-meteo.com/v1/forecast?latitude=51.4566&longitude=7.0123&daily=temperature_2m_max&forecast_days=1\"\
  }";

  ExtismHandle req = extism_alloc_buf_from_sz(reqStr);
  ExtismHandle res = extism_http_request(req, 0);

  if (200 != extism_http_status_code()) {
    return -1;
  }

  extism_output_set_from_handle(res, 0, extism_length(res));

  return 0;
}