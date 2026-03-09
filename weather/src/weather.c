//
// @package sublets-rs
//
// @file Weather plugin functions
// @copyright (c) 2026-present Christoph Kappel <christoph@unexist.dev>
// @version $Id$
//
// This program can be distributed under the terms of the GNU GPLv3.
// See the file LICENSE for details.
//

#define EXTISM_IMPLEMENTATION

#include "extism-pdk.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *get_config_key(const char *key, const char *fallback) {
    ExtismHandle value = extism_config_get(extism_alloc_buf_from_sz(key));

    if (0 == value) {
        value = extism_alloc_buf_from_sz(fallback);
    }

    const uint64_t len = extism_length(value);
    char *data = (char *)extism_malloc(value);

    if (NULL == data) {
        ExtismHandle err = extism_alloc_buf_from_sz("OOM");

        extism_error_set(err);

        return NULL;
    }

    extism_log_debug(value);

    extism_load_from_handle(value, 0, data, len);

    return data;
}

int32_t EXTISM_EXPORTED_FUNCTION(run) {
    char *latitude = get_config_key("latitude", "51.4566");
    char *longitude = get_config_key("longitude", "7.0123");

    if (NULL == latitude || NULL == longitude) {
        return -1;
    }

    char buf[255] = { 0 };

    sprintf(buf, "{\
        \"method\": \"GET\",\
        \"url\": \"https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&daily=temperature_2m_max&forecast_days=1\"\
    }", latitude, longitude);

    ExtismHandle req = extism_alloc_buf_from_sz(&buf);
    ExtismHandle res = extism_http_request(req, 0);

    if (200 != extism_http_status_code()) {
        return -1;
    }

    extism_output_set_from_handle(res, 0, extism_length(res));

    extism_free(latitude);
    extism_free(longitude);

    return 0;
}