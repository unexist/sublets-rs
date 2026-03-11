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
#include <string.h>

static const char *convert_to_char(ExtismHandle handle) {
    const uint64_t len = extism_length(handle);
    char *data = (char *)malloc(len + 1);

    if (NULL == data) {
        ExtismHandle err = extism_alloc_buf_from_sz("Out of memory");

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

static const char *REQUEST_URL = "{\
    \"method\": \"GET\",\
    \"url\": \"https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&daily=temperature_2m_max&forecast_days=1&temperature_unit=%s\"\
}";

int32_t EXTISM_EXPORTED_FUNCTION(run) {
    /* Assemble request */
    const char *latitude = get_config_key("latitude", "51.4566");
    const char *longitude = get_config_key("longitude", "7.0123");
    const char *unit = get_config_key("unit", "C");

    if (NULL == latitude || NULL == longitude || NULL == unit) {
        ExtismHandle err = extism_alloc_buf_from_sz("Out of memory");

        extism_error_set(err);

        if (latitude) free((void *)latitude);
        if (longitude) free((void *)longitude);
        if (unit) free((void *)unit);

        return -1;
    }

    const bool use_fahrenheit = (0 == strncmp(unit, "F", 1));

    /* Prepare and run HTTP request */
    char buf[255] = { 0 };

    sprintf(buf, REQUEST_URL, latitude, longitude,
        use_fahrenheit ? "fahrenheit" : "celsius");

    free((void *)latitude);
    free((void *)longitude);

    ExtismHandle req = extism_alloc_buf_from_sz(buf);
    ExtismHandle res = extism_http_request(req, 0);

    extism_free(req);

    if (200 != extism_http_status_code()) {
        ExtismHandle err = extism_alloc_buf_from_sz("HTTP call failed");

        extism_error_set(err);

        return -1;
    }

    /* Parse reply */
    const char *json = convert_to_char(res);

    if (NULL != json) {
        uint8_t i = 0;
        char buf[10] = { 0 };

        for (char *c = strrchr(json, '['); *++c != ']' && *c != '\0';) {
            buf[i++] = *c;
        }

        buf[i++] = use_fahrenheit ? 'F' : 'C';

        ExtismHandle out = extism_alloc_buf_from_sz(buf);

        extism_output_set_from_handle(out, 0, extism_length(out));

        free((void *)json);
    } else {
        ExtismHandle err = extism_alloc_buf_from_sz("Temperature not found");

        extism_error_set(err);

        return -1;
    }

    return 0;
}