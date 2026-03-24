#
# @package sublets-rs
#
# @file Uptime plugin functions
# @copyright (c) 2026-present Christoph Kappel <christoph@unexist.dev>
# @version $Id$
#
# This program can be distributed under the terms of the GNU GPLv3.
# See the file LICENSE for details.
#

import extism

@extism.import_fn("app", "get_uptime")
def get_uptime(): ...

@extism.plugin_fn
def run():
    data = get_uptime()
