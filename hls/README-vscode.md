# HLS from VS Code on Windows

## Create a component

Run **Terminal > Run Task > HLS: Create component**. Enter the component name,
FPGA part, and clock period in nanoseconds. The task creates `hls/<name>/`
with `<name>.h`, `<name>.cpp`, `<name>_test.cpp`, and `hls_config.cfg`, then
registers it in the existing build selector. Save any edits to `tasks.json`
before running this task because it updates that file (which must be plain JSON).
Existing component directories are never overwritten.

The starter performs 32-bit addition with a self-checking testbench, including
overflow. Replace its logic and interfaces with your design. The defaults are
the Cora Z7 device and a 10 ns clock. Device availability is validated at build
time. The task does not build anything or create Vitis output metadata; the
first build creates `build/vscode-hls/`. Workspace IntelliSense applies automatically.

## Build a component

Open `DAQ-HW.code-workspace` or the `DAQ-HW` folder in native Windows VS Code.
Press **Ctrl+Shift+B** and select **HLS: run selected stage** if prompted.
Choose a stage and component. Output appears in the task terminal; ordinary
Clang/GCC diagnostics are also matched into Problems. AMD-specific messages
remain available in the terminal and generated logs.

- `csim`: compile and run the C/C++ testbench.
- `synth`: synthesize the configured top function.
- `cosim`: synthesize, then run C/RTL co-simulation.
- `package`: synthesize, then package IP using the component configuration.
- `all`: C simulation, synthesis, co-simulation, then packaging; stop on failure.

After successful synthesis, the VS Code task opens `hls/syn/report/csynth.rpt`
from the component's output directory in the existing VS Code window. This also
applies to the synthesis stage of `cosim`, `package`, and `all`. An already open,
unmodified report refreshes when the tool rewrites it; avoid editing generated
reports. Failed synthesis does not trigger opening an old summary.
The VS Code `code.cmd` launcher must be on PATH. If opening fails, the task
prints a warning and the report path, and any remaining build stages continue.
For the same behavior from PowerShell, add `-OpenReport` to the wrapper command.

The three existing configs select `package.output.format=ip_catalog`.
Sources, part, clock, top function, and testbench inputs are controlled by each
component's existing `hls_config.cfg`. Packaging does not add the IP to a Vivado
project automatically. Testbench success only validates what that testbench checks.

Outputs go into `<component>/build/vscode-hls/` (already ignored by Git).
Reports and packaged IP are under that directory; inspect `hls/syn/report/`
and `hls/impl/ip/` after the corresponding stages. Existing IDE output is separate.
Do not run simultaneous builds for the same component from other terminals.

The wrapper runs from the config directory to resolve relative sources/data.
AMD's `.bat` launchers set up their environment; no global PATH changes are needed.
The default installation is `C:\AMDDesignTools\2026.1\Vitis`.

From PowerShell at the repository root:

```powershell
.\scripts\hls.ps1 -Action csim -Config hls/pulse_gen/pulse_generator/hls_config.cfg
.\scripts\hls.ps1 -Action package -Config hls/counter/counter/hls_config.cfg
```

Use `-DryRun` to print the planned commands without building, or `-VitisRoot`
to select another installation. Add future components to the `hlsConfig` input
in `.vscode/tasks.json`; the wrapper also accepts any explicit config path.

These tasks use Windows tools. Run them in a native Windows VS Code window,
not a Remote WSL window. Embedded application workflows are not configured here.

## IntelliSense

The Microsoft C/C++ extension uses the MinGW GCC bundled with Vitis 2026.1
to discover the standard C/C++ headers and compiler definitions. The public
`C:/AMDDesignTools/2026.1/Vitis/include` directory supplies `ap_int.h`,
`ap_fixed.h`, `hls_stream.h`, and the other HLS simulation headers. Both design
sources and testbenches use these headers for editing; `__SYNTHESIS__` is not
defined in the editor. Vitis still controls the actual synthesis compilation.

Defaults in `DAQ-HW.code-workspace` apply to all its folders, including the
separately opened `counter` folder and the HLS example folders. The DAQ-HW
folder also has a matching `.vscode/c_cpp_properties.json` for use when opened
alone. Header files are treated as C++ in the multi-folder workspace.

The editor defaults to C++14. If a component specifies a different language
standard or extra includes/defines, give that folder its own C/C++ configuration.
The existing IDE-generated compilation database is not selected: the counter
database currently references the absent 2026.1.1 installation and contains
AMD-specific compiler flags. It has been left intact.

If old squiggles remain, run **C/C++: Reset IntelliSense Database**, then
**Developer: Reload Window**. **C/C++: Log Diagnostics** shows the active
compiler and include search paths. This setup supports navigation and C++
diagnostics; HLS pragma legality and synthesizability are checked by the HLS tasks.
