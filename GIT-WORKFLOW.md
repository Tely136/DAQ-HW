# Git and generated files

Keep source code, testbenches, constraints, Vivado project/source files,
`hls_config.cfg`, component `vitis-comp.json`, application build configuration,
BSP/domain configuration and build scripts in Git.

The root `.gitignore` supplements the tool-generated Vitis ignore files. It
excludes HLS simulation/synthesis work, generated BSP libraries, compilation
databases and transient workspace state. Builds leave these files on disk.

## Hardware handoffs

Keep XSA files and the existing HLS `hls/impl/ip/` directories tracked for now.
The Vivado project searches the HLS workspace for exported IP, so those packages
are dependencies of the current workflow. Changes to exported IP and XSA files
after intentional hardware updates are expected and should be reviewed together
with their source changes. Do not globally ignore `*.xml`, `*.json`, `*.tcl`,
`*.xci`, or `*.bd`.

After cloning, rebuild the Vitis platforms/BSPs before building applications;
generated BSP headers and libraries are no longer stored in Git. Existing local
builds remain intact. HLS builds regenerate simulation and synthesis output.
See `hls/README-vscode.md` for the command-line HLS workflow. New IDE HLS
components with custom work directories need corresponding ignore entries.

## One-time cleanup

The cleanup stages removal of already tracked generated files from Git's index
only. The files remain on disk. The large deletion count is expected in this
one cleanup commit; subsequent builds should produce much smaller Git changes.
Review staged removals with `git diff --cached --stat`, and include the updated
`.gitignore` and this document in the same commit. Source changes unrelated to
the cleanup should be reviewed separately.
