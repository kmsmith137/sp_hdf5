#!/usr/bin/env python

import build_helpers

# If called recursively in superbuild, a global persistent LegacyUninstaller will be returned.
lu = build_helpers.get_global_legacy_uninstaller()

lu.uninstall_headers('sp_hdf5*.hpp')

# If called recursively in superbuild, run() will not be called here.
if __name__ == '__main__':
    lu.run()
