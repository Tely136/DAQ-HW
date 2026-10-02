# Cora Linux workspace tasks

Open `DAQ-linux-sw.code-workspace` in VS Code connected to WSL.

For each application:

1. Add its source directory to the workspace using **File: Add Folder to Workspace**.
2. Select that project as the active folder in CMake Tools.
3. Select the Cora Yocto SDK kit and configure the project, using a separate
   build directory for each application.
4. Select its executable as the CMake launch/debug target.
5. Use **Tasks: Run Task** and select one of the shared Cora tasks.

Tasks:

- **Cora: Build** builds the selected application's configured build tree.
- **Cora: Build + Deploy** builds and copies the selected executable.
- **Cora: Build + Deploy + Run** also runs it as the SSH user.
- **Cora: Build + Deploy + Run as root** runs it through interactive sudo.

The tasks use CMake Tools' `cmake.getLaunchTargetPath` substitution. No application
name is hardcoded. `scripts/cora-app.sh` sources the SDK explicitly because a
generic process task does not inherit the selected CMake kit's environment.
Configuration remains the responsibility of CMake Tools and the SDK kit.

Connection and SDK values are in each task's `options.env` in the workspace file:

- `CORA_HOST`: `amd-edf@192.168.137.20`
- `CORA_JUMP_HOST`: `thomas@192.168.137.101` (set to an empty string for direct SSH)
- `CORA_REMOTE_DIR`: `/tmp/daq-hw-amd-edf`
- `CORA_SDK_ENV`: the installed SDK environment setup script

Transfers use a temporary remote file followed by a rename. Failed builds stop
before deployment. The run tasks allocate a remote terminal; enter any SSH or
sudo password in the task terminal. The account must already have sudo access.
No password or sudo policy is stored or changed by these tasks.

Each executable runs with the remote deployment directory as its working
directory. This workflow copies one executable; application data and extra
shared libraries need separate deployment. Remote arguments are not configured.
It assumes a normal single-configuration CMake build with executable outputs
inside the build tree. `/tmp` deployments disappear on reboot.

Validation: the script passed `bash -n` and its build-only action successfully
built `hello_world_linux` with the installed SDK. Remote transfer and execution
still need verification against the running Cora from the VS Code task terminal.
