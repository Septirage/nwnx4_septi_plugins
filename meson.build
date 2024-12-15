project(
    'SeptiPlugins',
    ['cpp', 'c'],
    default_options : [
        'cpp_std=c++20',
        'bindir=.',
        'libdir=.',
        'datadir=.',
    ],
)

fs = import('fs')


# Global compiler arguments
cppcompiler = meson.get_compiler('cpp')
add_project_arguments('/DWIN32', '/D_WINDOWS', '/D_USRDLL', '/DTEST_EXPORTS', '/D_UNICODE', '/DUNICODE', language: ['cpp', 'c'])

if get_option('buildtype') == 'release'
    add_project_arguments('/DNDEBUG', language: ['cpp', 'c'])
    add_global_link_arguments('/DEBUG', language: ['cpp', 'c'])
    global_name_project_add=''
else
    add_project_arguments('/D_DEBUG', language: ['cpp', 'c'])
    global_name_project_add='D'
endif


nwnx4_repo_path = get_option('nwnx4_dir')
if not fs.is_absolute(nwnx4_repo_path)
    nwnx4_repo_path = meson.global_source_root() / nwnx4_repo_path
endif

# Include & lib paths
include_dirs = include_directories([
    get_option('nwnx4_dir') / 'lib/NWN2/include',
    get_option('nwnx4_dir') / 'vcpkg_installed/x86-windows-static-md/include',
    get_option('nwnx4_dir') / 'src',
])
lib_dirs = [
    nwnx4_repo_path / 'lib/NWN2'
]

if get_option('buildtype') == 'release'
    lib_dirs += nwnx4_repo_path / 'vcpkg_installed/x86-windows-static-md/lib'
else
    lib_dirs += nwnx4_repo_path / 'vcpkg_installed/x86-windows-static-md/debug/lib'
endif


# # Target list
subdir('src/plugins/xp_aspectManager')
subdir('src/plugins/xp_enhancedFeatures')
subdir('src/plugins/xp_msgServer')

install_data('README.md', install_dir: '.')
