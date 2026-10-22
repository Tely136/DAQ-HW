# Cora MCS SDT handoff

Run `bash scripts/amd-edf-tools.sh --sdt` from WSL. The script locates the
repository from its own path, reads `hardware/counter_wrapper.xsa`, and generates
into a fresh temporary directory on the WSL filesystem. It checks the generated
handoff before synchronizing it to `hardware/cora-mcs/sdt` and the existing
`/home/tely1/yocto/hardware/cora-mcs/sdt` used by EDF and existing applications.
All existing phase flags and their defaults are unchanged.

The `sdt` directory is generated output: do not hand-edit it. Synchronization
removes stale generated files within that directory. Keep custom DTS overrides
in the Yocto layer, not in this snapshot. Reports, logs, and editor state are
excluded. Failed SDT generation leaves both published handoffs untouched;
publication itself uses rsync, so an interrupted copy can require rerunning.

Commit the handoff with the matching XSA and hardware-source changes. The
manifest records the configured Vitis release and input XSA SHA-256. The
bitstream is intentionally tracked as a normal Git binary so a checkout has
all handoff inputs; Git LFS can be adopted later if binary history grows large.

The generated C/H, CMake, Tcl, YAML, DTS and binding files are tracked. LF is
enforced through .gitattributes. Keep Yocto builds, caches and SDKs in WSL.
New applications can reference the repo's `sdt/drivers` directory through their
`IP_DRIVER_ROOT` CMake option. Existing WSL driver paths continue to work.
