#!/usr/bin/env python3

import os
import re
import subprocess
import sys

# Ensure copyright headers match this format and are followed by a blank line:
# /*
#  * Copyright (c) YYYY(-YYYY), Whatever
#  * ... more of these ...
#  *
#  * SPDX-License-Identifier: BSD-2-Clause
#  */
GOOD_LICENSE_HEADER_PATTERN = re.compile(
    '^/\\*\n' +
    '( \\* Copyright \\(c\\) [0-9]{4}(-[0-9]{4})?, .*\n)+' +
    ' \\*\n' +
    ' \\* SPDX-License-Identifier: BSD-2-Clause\n' +
    ' \\*/\n' +
    '\n')
LICENSE_HEADER_CHECK_EXCLUDES = {
    'AK/Checked.h',
    'AK/Function.h',
    'Userland/Libraries/LibJS/SafeFunction.h',
    'Userland/Libraries/LibC/elf.h',
    'Userland/Libraries/LibCodeComprehension/Cpp/Tests/',
    'Userland/Libraries/LibCpp/Tests/parser/',
    'Userland/Libraries/LibCpp/Tests/preprocessor/'
}

# We check that "#pragma once" is present
PRAGMA_ONCE_STRING = '#pragma once'
PRAGMA_ONCE_CHECK_EXCLUDES = {
    'Userland/Libraries/LibC/assert.h',
}

# We make sure that there's a blank line before and after pragma once
GOOD_PRAGMA_ONCE_PATTERN = re.compile('(^|\\S\n\n)#pragma once(\n\n\\S.|$)')


def should_check_file(filename):
    if not filename.endswith('.cpp') and not filename.endswith('.h'):
        return False
    if filename.startswith('Base/'):
        return False
    if filename == 'Kernel/FileSystem/Ext2FS/Definitions.h':
        return False
    return True


def find_files_here_or_argv():
    if len(sys.argv) > 1:
        raw_list = sys.argv[1:]
    else:
        process = subprocess.run(["git", "ls-files"], check=True, capture_output=True)
        raw_list = process.stdout.decode().strip('\n').split('\n')

    return filter(should_check_file, raw_list)


def run():
    errors_license = []
    errors_pragma_once_bad = []
    errors_pragma_once_missing = []

    for filename in find_files_here_or_argv():
        with open(filename, "r") as f:
            file_content = f.read()
        if not any(filename.startswith(forbidden_prefix) for forbidden_prefix in LICENSE_HEADER_CHECK_EXCLUDES):
            if not GOOD_LICENSE_HEADER_PATTERN.search(file_content):
                errors_license.append(filename)
        if filename.endswith('.h'):
            if any(filename.startswith(forbidden_prefix) for forbidden_prefix in PRAGMA_ONCE_CHECK_EXCLUDES):
                # File was excluded
                pass
            elif GOOD_PRAGMA_ONCE_PATTERN.search(file_content):
                # Excellent, the formatting is correct.
                pass
            elif PRAGMA_ONCE_STRING in file_content:
                # Bad, the '#pragma once' is present but it's formatted wrong.
                errors_pragma_once_bad.append(filename)
            else:
                # Bad, the '#pragma once' is missing completely.
                errors_pragma_once_missing.append(filename)

    if errors_license:
        print("Files with bad licenses:", " ".join(errors_license))
    if errors_pragma_once_missing:
        print("Files without #pragma once:", " ".join(errors_pragma_once_missing))
    if errors_pragma_once_bad:
        print("Files with a bad #pragma once:", " ".join(errors_pragma_once_bad))

    if errors_license or errors_pragma_once_missing or errors_pragma_once_bad:
        sys.exit(1)


if __name__ == '__main__':
    os.chdir(os.path.dirname(__file__) + "/..")
    run()
