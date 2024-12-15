# NWNX4 SEPTI Plugins

This repository contains a collection of plugins for NWNX4 and Neverwinter Nights 2 (NWN2).

These plugins enhance server functionality by providing a variety of features and tools that can be easily integrated into your NWN2 experience. Each plugin is designed to be flexible and adaptable, allowing developers to extend NWN2's capabilities through custom scripts and functionalities.

For more information about the SEPTI Project and its objectives, please visit the [project website](https://septirage.com/nwn2/).


# Usage

For detailed integration instructions, please refer to the official website and the project documentation, which includes extensive integration guidelines for compiled plugin versions.


# Development

## Download and setup NWNX4

```bash
# Fetch NWNX4 submodule
git submodule init
git submodule update

# Move to NWNX4 folder
cd nwnx4

# The followig instructions are based on 
# https://github.com/nwn2dev/nwnx4/blob/main/README.md#initialize-your-environment

# Initialize git submodules (if you did not clone this repo with `--recurse`)
git submodule init
git submodule update

# Bootstrap vcpkg (prepare dependencies)
vcpkg\bootstrap-vcpkg.bat

# Compile and install dependencies
vcpkg\vcpkg.exe install --triplet=x86-windows-static-md --clean-after-build
```

Alternatively, if you already have a nwnx4 dev environment, you can use `meson setup builddir -D nwnx4_dir=C:\xxx\nwnx4` to use your own environment. Be aware that a newer or older version of nwnx4 may not be compatible for building septi plugins.

## Build septi plugins

```bash
meson setup builddir
cd builddir
meson compile

meson install --destdir=%cd%\..\septi-plugins
```

# Contributing

SEPTI Project is an open-source project, and contributions are welcome. 
If you would like to contribute to the project, you can do so by:

- Reporting bugs or issues in the [GitHub issue tracker](https://github.com/Septirage/nwnx4_septi_plugins/issues).
- Submitting merge requests for new features or improvements.
- Reporting bugs, providing feedback, or making requests on our [website](https://septirage.com/nwn2/) or discord server