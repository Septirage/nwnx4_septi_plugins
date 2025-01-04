plugin_name = 'xp_aspectManager'+global_name_project_add

plugin_shared_lib = shared_library(plugin_name,
    [
        nwnx4_repo_path / 'src/plugins/plugin.cpp',
        nwnx4_repo_path / 'src/misc/log.cpp',
        nwnx4_repo_path / 'src/misc/Patch.cpp',
        '../../septutil/crypto.cpp',
        '../../septutil/srvadmin.cpp',
        '../../septutil/win_utils.cpp',
        'appListManagement.cpp',
        'aspectManager.cpp',
        'aspectManagerUtils.cpp',

        'manageArea.cpp',
        'manageCreature.cpp',
        'manageItem.cpp',
        'manageLight.cpp',
        'manageObject.cpp',

        'messageManagement.cpp',
        'refreshObject.cpp',
        'scanAndAnalyseFile.cpp',
    ],
    cpp_args: ['/D_WIN32_WINNT=0x0400'],
    link_args: ['/dynamicbase', '/nxcompat'],
    include_directories: include_dirs,
    dependencies: [
        cppcompiler.find_library('version'),
    	cppcompiler.find_library('detours', dirs: lib_dirs),
    	cppcompiler.find_library('NWN2_MemoryMgr_amdxp', dirs: lib_dirs)
    ],
    install: true,
    install_dir: 'plugins',
)


# Get version from git tags
version = run_command('git', 'describe', '--tags', '--match=am-*', check: true).stdout().substring(3, -1)

# Template version file
custom_target('compileVersion',
    input : [
        plugin_shared_lib,
        configure_file(
            input : meson.source_root() / 'src/Version.rc.in',
            output : 'Version.rc',
            configuration : {
                'PRODUCTVERSION' : version,
            },
        ),
    ],
    output : 'fake',
    command : [meson.source_root() / 'version.bat', meson.current_build_dir(), plugin_shared_lib.full_path()],
    build_by_default : true,
)

install_data('xp_aspectManager.ini', install_dir: 'config.example')
install_data('listeHakZip.ini', install_dir: 'config.example')
install_data('nwnx_aspectManager.nss', install_dir: 'nwscript')
