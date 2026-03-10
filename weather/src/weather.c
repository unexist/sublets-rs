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
#include <stdlib.h>

static const char *convert_to_char(ExtismHandle handle) {
    const uint64_t len = extism_length(handle);
    char *data = (char *)malloc(len + 1);

    if (NULL == data) {
        ExtismHandle err = extism_alloc_buf_from_sz("OOM");

        extism_error_set(err);

        return NULL;
    }

    extism_load_from_handle(handle, 0, data, len);

    data[len] = '\0';

    return data;
}

static const char *get_config_key(const char *key, const char *fallback) {
    ExtismHandle value = extism_config_get(extism_alloc_buf_from_sz(key));

    if (0 == value) {
        value = extism_alloc_buf_from_sz(fallback);
    }

    return convert_to_char(value);
}

int32_t EXTISM_EXPORTED_FUNCTION(run) {
    /* Assemble request */
    const char *latitude = get_config_key("latitude", "51.4566");
    const char *longitude = get_config_key("longitude", "7.0123");

    if (NULL == latitude || NULL == longitude) {
        ExtismHandle err = extism_alloc_buf_from_sz("Latitude or longitude missing");

        extism_error_set(err);

        return -1;
    }

    char buf[255] = { 0 };

    sprintf(buf, "{\
        \"method\": \"GET\",\
        \"url\": \"https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&daily=temperature_2m_max&forecast_days=1\"\
    }", latitude, longitude);

    /* Run HTTP request */
    ExtismHandle req = extism_alloc_buf_from_sz(buf);
    ExtismHandle res = extism_http_request(req, 0);

    if (200 != extism_http_status_code()) {
        ExtismHandle err = extism_alloc_buf_from_sz("HTTP call failed");

        extism_error_set(err);

        return -1;
    }

    /* Parse reply */


    extism_output_set_from_handle(res, 0, extism_length(res));

    free((void *)latitude);
    free((void *)longitude);

    return 0;
}