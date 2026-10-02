#!/usr/bin/env bash
# Invoked by the WSL workspace tasks; may also be used from a terminal.
set -euo pipefail

action=${1:?Expected build, deploy, run, or run-root}
executable=${2:?Select and configure a CMake launch target first}
sdk=${CORA_SDK_ENV:-/home/tely1/yocto/sdk/cora-mcs/environment-setup-cortexa9t2hf-neon-amd-linux-gnueabi}
host=${CORA_HOST:-amd-edf@192.168.137.20}
jump=${CORA_JUMP_HOST-thomas@192.168.137.101}
remote_dir=${CORA_REMOTE_DIR:-/tmp/daq-hw-amd-edf}

case "$action" in
    build|deploy|run|run-root) ;;
    *) printf 'Unknown action: %s\n' "$action" >&2; exit 2 ;;
esac
[[ "$executable" = /* ]] || { echo 'Configure and select a CMake executable target first.' >&2; exit 2; }
name=$(basename "$executable")
# Remote commands are interpreted by SSH's remote shell; restrict path characters.
[[ "$name" =~ ^[a-zA-Z0-9_][a-zA-Z0-9_.-]*$ && "$remote_dir" =~ ^/[a-zA-Z0-9_/-]+$ ]] || {
    echo 'Use a simple executable name and an absolute remote directory without spaces.' >&2; exit 2;
}

# Executables may be in a subdirectory of the CMake build tree.
build_dir=$(dirname "$executable")
while [[ ! -f "$build_dir/CMakeCache.txt" && "$build_dir" != / ]]; do
    build_dir=$(dirname "$build_dir")
done
[[ -f "$build_dir/CMakeCache.txt" ]] || { echo 'No CMake cache found above the selected executable. Configure first.' >&2; exit 2; }
[[ -f "$sdk" ]] || { printf 'SDK setup script not found: %s\n' "$sdk" >&2; exit 2; }
# Generated SDK scripts may reference unset variables.
set +u
source "$sdk"
set -u
cmake --build "$build_dir" --parallel
[[ -f "$executable" ]] || { printf 'Executable was not built: %s\n' "$executable" >&2; exit 1; }
[[ "$action" != build ]] || exit 0

connection=()
[[ -z "$jump" ]] || connection=(-J "$jump")
remote="$remote_dir/$name"
ssh "${connection[@]}" "$host" "mkdir -p '$remote_dir'"
# Upload to a temporary file so failed copies leave the previous executable intact.
staged=$(ssh "${connection[@]}" "$host" "mktemp '$remote_dir/.upload.XXXXXX'")
[[ "$staged" == "$remote_dir"/.upload.* && "$staged" =~ ^/[a-zA-Z0-9_./-]+$ ]] || {
    echo 'Unexpected remote temporary filename.' >&2; exit 1;
}
cleanup() { ssh "${connection[@]}" "$host" "rm -f '$staged'" || true; }
trap cleanup EXIT
scp "${connection[@]}" "$executable" "$host:$staged"
ssh "${connection[@]}" "$host" "chmod 755 '$staged' && mv -f '$staged' '$remote'"
trap - EXIT
printf 'Deployed %s:%s\n' "$host" "$remote"
case "$action" in
    run) ssh -t "${connection[@]}" "$host" "cd '$remote_dir' && exec './$name'" ;;
    run-root) ssh -t "${connection[@]}" "$host" "cd '$remote_dir' && exec sudo -- '$remote'" ;;
esac
