#!/bin/sh
set -eu

# Locate Homebrew on Apple Silicon and Intel Macs, then run its LLDB DAP.
if [ -x /opt/homebrew/bin/brew ]; then
    brew_bin=/opt/homebrew/bin/brew
elif [ -x /usr/local/bin/brew ]; then
    brew_bin=/usr/local/bin/brew
elif command -v brew >/dev/null 2>&1; then
    brew_bin=$(command -v brew)
else
    echo "Homebrew is required for the macOS debugger. Install Homebrew, then run: brew install lldb" >&2
    exit 1
fi

lldb_prefix=$("$brew_bin" --prefix lldb)
lldb_dap="$lldb_prefix/bin/lldb-dap"
if [ ! -x "$lldb_dap" ]; then
    echo "Homebrew LLDB DAP not found at $lldb_dap. Run: brew install lldb" >&2
    exit 1
fi

# Resolve our launcher symlink in the DAP request before macOS starts the app.
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec python3 "$script_dir/lldb_dap_resolve_program.py" "$lldb_dap" "$@"
