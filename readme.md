# HAL9000

## Introduction

The initial version of HAL9000's source code was managed as a Visual Studio project (not VS Code), which meant it could only be compiled on the Windows operating system (OS). In the current version, the build process has been migrated to CMake and Clang, making the project buildable on any platform. Keep in mind, however, that the target platform on which HAL9000 runs was and still is x86-64.

Previously, the compiled HAL9000 OS required a full virtual machine (VM) to run. Now, it can be executed directly inside QEMU, in emulator mode.

With these changes, the project can be compiled and run on your normal "daily driver" OS (i.e. the one installed on your computer) (Linux, MacOS, Windows) without a virtualization solution (i.e. VMWare Workstation, VirtualBox).

Despite the improvements described above and the increased flexibility of the build environment, we strongly recommend working on the project inside a Linux VM, since this is the configuration we thoroughly tested. In particular, we suggest using Ubuntu 24.04 LTS, as the project has been validated on this version.

Running HAL9000 in a VM provides an isolated environment, avoiding version conflicts between its dependencies and the software installed on your host system. Moreover, a VM makes cleanup trivial—once you are done: you can simply delete the VM to "uninstall" everything at once.

## Recommended Setup

1. Download and install a virtualization solution such as [VMWare Workstation](https://www.vmware.com/products/desktop-hypervisor/workstation-and-fusion) or [Oracle Virtualbox](https://www.virtualbox.org/wiki/Downloads). Alternatively, you may enable the built-in virtualization solution of your OS (e.g., Hyper-V on Windows, Apple Virtualization Framework on macOS, KVM on Linux).
2. Download [Ubuntu 24.04 LTS](https://ubuntu.com/download/desktop) and install it in a virtual machine (VM). Configure the VM with at least 50 GB of storage, 4 GB of RAM and 4 processors.
3. Start Ubuntu VM, log in and open a terminal.
4. Install the `git` tool, by running:

    ```bash
    sudo apt install git
    ```

5. Clone the HAL9000 repository in your VM, by running:

    ```bash
    git clone URL
    ```

6. Change into the cloned directory and run the setup script:

    ```bash
    python3 HAL9000.py setup
    ```

### Test the Setup

1. Build the project, by running:

    ```bash
    python3 HAL9000.py build --all
    ```

2. Run the HAL9000 in a virtualized environment created and managed by QEMU, by running:

    ```bash
    python3 HAL9000.py run
    ```

### Configure git

To configure git, run:

```bash
python3 HAL9000.py setup git <your_repo>
```

where `<your_repo>` should look like `git@github.com:PSO-OSD-UTCN/hal9000-lab-<youruser>.git`.

## Install VSCode

Download VSCode as a [deb package](https://code.visualstudio.com/sha/download?build=stable&os=linux-deb-x64). Be aware that **installing VSCode as a snap package (i.e. from the store) will NOT work!**

After the file is downloaded, open a terminal in the Downloads folder and install VSCode, by running:

```bash
sudo apt install "code*.deb"
```

Then, in the HAL9000 folder, run the following command to automatically install the required extensions:

```bash
python3 HAL9000.py setup vscode
```

### Test the VSCode Setup

1. Open a terminal, change the directory to the cloned HAL9000 repository and run HAL9000, by running:

    ```bash
    python3 HAL9000.py run -d
    ```

2. Add a breakpoint in the code:
   1. Find the `SystemInit()` function in HAL9000 using "*Go -> Go to Symbol in Workspace ...*".
   2. Add a breakpoint after the line with calling `EfiRuntimeGetTime()`.

3. Start the debugger:
   1. Go to the "*Run and Debug*" tab in VSCode.
   2. Choose the "*Debug HAL9000*" target.
   3. Click "*Run*".

4. The debugger should break once it's connected to QEMU. Click "*Continue*".

5. Once the breakpoint in `SystemInit()` is hit, inspect the value in the `Time` variable in the debugging panel.

6. Test the debug console:
   1. Open the Debug Console "*View -> Debug Console*".
   2. Type `expression Time`. It should print the `Time` variable.
   3. Type `list_processes` to see a list of currently running processes.
   4. Type `list_threads` to see a list of currently running threads.
