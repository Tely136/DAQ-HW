#!/usr/bin/env bash

set -euo pipefail

# Resolve the checkout through the script, not the caller's working directory.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd -P)"
REPO_SDT_DIR="$REPO_ROOT/hardware/cora-mcs/sdt"

# Paths and names to customize.
VITIS_RELEASE="2026.1"
VITIS_SETTINGS="/opt/Xilinx/2026.1/Vitis/settings64.sh"
SDT_DIR="/home/tely1/yocto/hardware/cora-mcs/sdt/"
XSA_PATH="$REPO_ROOT/hardware/counter_wrapper.xsa"
MACHINE_NAME="cora-mcs-generated"
IMAGE_MACHINE_NAME="amd-cortexa9thf-neon-common"
REMOTE_HOST="amd-edf@192.168.137.20"
REMOTE_PATH="/tmp/boot.bin"
# JUMP_HOST="thomas@pi5"
JUMP_HOST="thomas@192.168.137.101"

EDF_ROOT="/home/tely1/yocto/edf"
EDF_BUILD_DIR="$EDF_ROOT/build"
CUSTOM_LAYER="/home/tely1/yocto/custom-layers/meta-cora"
BOOTBIN_PATH="/home/tely1/yocto/edf/build/tmp/deploy/images/cora-mcs-generated/boot.bin"
WIC_IMAGE="/home/tely1/yocto/edf/build/tmp/deploy/images/amd-cortexa9thf-neon-common/edf-linux-disk-image-amd-cortexa9thf-neon-common.rootfs.wic"
IMAGE_RECIPE="edf-linux-disk-image"
SDK_RECIPE="meta-edf-app-sdk"
SDK_INSTALL_DIR="/home/tely1/yocto/sdk/cora-mcs"

RUN_SDT=false
RUN_BOOTBIN=false
RUN_IMAGE=false
RUN_WIC=false
RUN_DEPLOY=false
RUN_SDK=false

usage() {
	cat <<'USAGE'
Usage: amd-edf-tools.sh [phase ...]

Phases:
  --sdt       Generate SDT in WSL; update WSL and repo handoffs
  --bootbin   Generate the machine configuration and BOOT.BIN
  --image     Build edf-linux-disk-image
  --wic       Copy BOOT.BIN into WIC partition 1
  --deploy    SCP BOOT.BIN to the configured remote path
  --sdk       Build meta-edf-app-sdk and install into SDK_INSTALL_DIR
  --all       Run SDT, BOOT.BIN, image, WIC, and deploy phases
              With no arguments: SDT, BOOT.BIN, image, and WIC only
              SDK installation is opt-in; combine --all with --sdk
  -h, --help  Show this help

Examples:
  ./amd-edf-tools.sh --sdt --bootbin
  ./amd-edf-tools.sh --deploy
  ./amd-edf-tools.sh --image --wic
  ./amd-edf-tools.sh --sdk
USAGE
}

if [[ $# -eq 0 ]]; then
	RUN_SDT=true
	RUN_BOOTBIN=true
	RUN_IMAGE=true
	RUN_WIC=true
else
	while [[ $# -gt 0 ]]; do
		case "$1" in
			--sdt) RUN_SDT=true ;;
			--bootbin) RUN_BOOTBIN=true ;;
			--image) RUN_IMAGE=true ;;
			--wic) RUN_WIC=true ;;
			--deploy) RUN_DEPLOY=true ;;
			--sdk) RUN_SDK=true ;;
			--all)
				RUN_SDT=true
				RUN_BOOTBIN=true
				RUN_IMAGE=true
				RUN_WIC=true
				RUN_DEPLOY=true
				;;
			-h|--help) usage; exit 0 ;;
			*) printf 'Unknown phase: %s\n\n' "$1" >&2; usage >&2; exit 2 ;;
		esac
		shift
	done
fi

if [[ "$RUN_SDT" == true && ! -f "$VITIS_SETTINGS" ]]; then
	printf 'Vitis settings file not found: %s\n' "$VITIS_SETTINGS" >&2
	exit 1
fi

# Generate into a fresh Linux staging directory. Failed generation never
# publishes into either the working EDF handoff or the versioned snapshot.
if [[ "$RUN_SDT" == true ]]; then
(
    [[ -f "$XSA_PATH" ]] || { printf 'XSA not found: %s\n' "$XSA_PATH" >&2; exit 1; }
    command -v rsync >/dev/null || { echo 'Install rsync in WSL first.' >&2; exit 1; }
    mkdir -p "$(dirname -- "${SDT_DIR%/}")"
    STAGE_ROOT="$(mktemp -d "$(dirname -- "${SDT_DIR%/}")/.sdt-stage.XXXXXX")"
    trap 'rm -rf -- "$STAGE_ROOT"' EXIT
    export SDT_STAGE_OUTPUT="$STAGE_ROOT/sdt"
    export SDT_INPUT_XSA="$XSA_PATH"
    XSA_SHA256="$(sha256sum "$XSA_PATH" | cut -d ' ' -f 1)"
    (
        cd "$STAGE_ROOT"
        set +u
        source "$VITIS_SETTINGS"
        set -u
        # Tcl environment references also handle spaces in the repo path.
        sdtgen <<'SDTGEN_COMMANDS'
if {[catch {
    set_dt_param -dir $::env(SDT_STAGE_OUTPUT)
    set_dt_param -xsa $::env(SDT_INPUT_XSA)
    generate_sdt
} message]} {
    puts stderr $message
    exit 1
}
exit 0
SDTGEN_COMMANDS
    )
    [[ "$XSA_SHA256" == "$(sha256sum "$XSA_PATH" | cut -d ' ' -f 1)" ]] || {
        echo 'XSA changed during generation; handoff not published.' >&2; exit 1;
    }
    for required in system-top.dts pl.dtsi pcw.dtsi ps7_init.c ps7_init.h counter_wrapper.bit; do
        [[ -s "$SDT_STAGE_OUTPUT/$required" ]] || {
            printf 'Generated handoff missing %s; not published.\n' "$required" >&2; exit 1;
        }
    done
    [[ -n "$(find "$SDT_STAGE_OUTPUT/drivers" -name '*_hw.h' -print -quit)" ]] || {
        echo 'Generated HLS driver headers missing; not published.' >&2; exit 1;
    }
    # Record stable provenance; avoid a timestamp-only diff every generation.
    printf 'vitis_release=%s\nxsa=hardware/counter_wrapper.xsa\nxsa_sha256=%s\n' \
        "$VITIS_RELEASE" "$XSA_SHA256" > "$SDT_STAGE_OUTPUT/handoff-manifest.txt"

    # Only the designated generated directories may be synchronized/deleted.
    [[ "$(realpath -m "$REPO_SDT_DIR")" == "$REPO_ROOT/hardware/cora-mcs/sdt" ]] || exit 1
    [[ "$(realpath -m "$SDT_DIR")" == /home/tely1/yocto/hardware/cora-mcs/sdt ]] || {
        echo 'Unexpected EDF SDT destination; review the publication guard.' >&2; exit 1;
    }
    for destination in "$REPO_SDT_DIR" "${SDT_DIR%/}"; do
        [[ ! -L "$destination" ]] || { echo 'SDT destination cannot be a symlink.' >&2; exit 1; }
        mkdir -p "$destination"
        rsync -rlt --delete --checksum \
            --exclude='.git/' --exclude='.vscode/' --exclude='.Xil/' \
            --exclude='*.log' --exclude='*.jou' --exclude='ps7_init.html' \
            "$SDT_STAGE_OUTPUT/" "$destination/"
    done
    printf 'Published SDT and drivers to:\n  %s\n  %s\n' "$SDT_DIR" "$REPO_SDT_DIR"
)
fi

# Generate the machine configuration and build BOOT.BIN in an EDF environment.

if [[ "$RUN_BOOTBIN" == true || "$RUN_IMAGE" == true || "$RUN_WIC" == true || "$RUN_SDK" == true ]]; then
(
	set +u
	source "$EDF_ROOT/edf-init-build-env" "$EDF_BUILD_DIR"
	set -u

	if [[ "$RUN_BOOTBIN" == true ]]; then
		gen-machine-conf \
			--hw-description "$SDT_DIR" \
			--machine-name "$MACHINE_NAME" \
			--config-dir "$CUSTOM_LAYER/conf" \
			parse-sdt

		MACHINE="$MACHINE_NAME" bitbake xilinx-bootbin
	fi

	if [[ "$RUN_IMAGE" == true ]]; then
		MACHINE="$IMAGE_MACHINE_NAME" bitbake "$IMAGE_RECIPE"
	fi

	if [[ "$RUN_SDK" == true ]]; then
		MACHINE="$IMAGE_MACHINE_NAME" bitbake "$SDK_RECIPE"
		SDK_DEPLOY_DIR="$(MACHINE="$IMAGE_MACHINE_NAME" bitbake-getvar -r "$SDK_RECIPE" --value SDK_DEPLOY)"
		SDK_OUTPUT_NAME="$(MACHINE="$IMAGE_MACHINE_NAME" bitbake-getvar -r "$SDK_RECIPE" --value TOOLCHAIN_OUTPUTNAME)"
		if [[ -z "$SDK_DEPLOY_DIR" || -z "$SDK_OUTPUT_NAME" ]]; then
			printf 'Could not determine SDK installer path from BitBake metadata.\n' >&2
			exit 1
		fi
		SDK_INSTALLER="$SDK_DEPLOY_DIR/$SDK_OUTPUT_NAME.sh"
		if [[ ! -f "$SDK_INSTALLER" ]]; then
			printf 'SDK installer not found: %s\n' "$SDK_INSTALLER" >&2
			exit 1
		fi
		printf 'Installing SDK from %s into %s\n' "$SDK_INSTALLER" "$SDK_INSTALL_DIR"
		bash "$SDK_INSTALLER" -y -d "$SDK_INSTALL_DIR"
	fi

	if [[ "$RUN_WIC" == true && ! -f "$BOOTBIN_PATH" ]]; then
		printf 'BOOT.BIN not found: %s\n' "$BOOTBIN_PATH" >&2
		exit 1
	fi

	if [[ "$RUN_WIC" == true && ! -f "$WIC_IMAGE" ]]; then
		printf 'WIC image not found: %s\n' "$WIC_IMAGE" >&2
		exit 1
	fi

	if [[ "$RUN_WIC" == true ]]; then
		wic cp "$BOOTBIN_PATH" "$WIC_IMAGE:1"
	fi
)
fi

if [[ "$RUN_DEPLOY" == true ]]; then
	if [[ ! -f "$BOOTBIN_PATH" ]]; then
		printf 'BOOT.BIN not found: %s\n' "$BOOTBIN_PATH" >&2
		exit 1
	fi

	scp -o "ProxyJump=$JUMP_HOST" "$BOOTBIN_PATH" "$REMOTE_HOST:$REMOTE_PATH"
fi