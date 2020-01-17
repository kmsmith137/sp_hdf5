#!/usr/bin/env python

import build_helpers

# If called recursively in superbuild, a global persistent HeavyHandedUninstaller will be returned.
u = build_helpers.get_global_heavy_handed_uninstaller()

u.uninstall_headers('sp_hdf5*.hpp')

# If called recursively in superbuild, run() will not be called here.
if __name__ == '__main__':
    lu.run()