#!/usr/bin/env bash

set -euo pipefail

# Paths and names to customize.
VITIS_SETTINGS="/opt/Xilinx/2026.1/Vitis/settings64.sh"
SDT_DIR="/home/tely1/yocto/hardware/cora-mcs/sdt/"
XSA_PATH="/mnt/c/DAQ-HW/hardware/counter_wrapper.xsa"
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
  --sdt       Generate the system device tree with SDTGen
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

if [[ "$RUN_SDT" == true && ! -d "$SDT_DIR" ]]; then
	printf 'SDT directory not found: %s\n' "$SDT_DIR" >&2
	exit 1
fi

if [[ "$RUN_SDT" == true && ! -f "$SDT_DIR/system-top.dts" ]]; then
	printf 'system-top.dts not found in SDT directory: %s\n' "$SDT_DIR" >&2
	exit 1
fi

# Generate the SDT in a Vitis environment isolated from EDF.
if [[ "$RUN_SDT" == true ]]; then
(
	set +u
    source "$VITIS_SETTINGS"
	set -u

    sdtgen <<SDTGEN_COMMANDS
set_dt_param -dir "$SDT_DIR"
set_dt_param -xsa "$XSA_PATH"
generate_sdt
SDTGEN_COMMANDS
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