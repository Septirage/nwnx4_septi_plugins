plugin_version = '"1.3.4"'
plugin_name = 'xp_msgServer'+global_name_project_add
dllName = plugin_name+'.dll'


plugin_shared_lib = shared_library(plugin_name, 
    [
        nwnx4_repo_path / 'src/plugins/plugin.cpp',
        nwnx4_repo_path / 'src/hook/scriptManagement.cpp',
        nwnx4_repo_path / 'src/misc/log.cpp',
        nwnx4_repo_path / 'src/misc/Patch.cpp',
        'playerIDManager.cpp',
        'Connection.cpp',
        'Anticheat.cpp',
        '../../septutil/crypto.cpp',
        '../../septutil/srvadmin.cpp',
        '../../septutil/intern2daMgt.cpp',
    ],
    cpp_args: ['-DPLUGIN_VERSION='+plugin_version],
    link_args: ['/dynamicbase', '/nxcompat'],
    include_directories: include_dirs,
    dependencies: [
    	cppcompiler.find_library('detours', dirs: lib_dirs),
    	cppcompiler.find_library('NWN2_MemoryMgr_amdxp', dirs: lib_dirs),
    ],
    install: true,
    install_dir: 'plugins',
)

versionrc_obj = configure_file(
    input : meson.source_root() / 'Version.rc.in',
    output : 'Version.rc',
    configuration : {
        'PRODUCTVERSION' : plugin_version.replace('"', ''),
        'FILEVERSION' : plugin_version.replace('"', ''),
    },
)

custom_target('compileVersion',
          input : 
          [
                plugin_shared_lib, configure_file(
                    input : meson.source_root() / 'Version.rc.in',
                    output : 'Version.rc',
                    configuration : {
                        'PRODUCTVERSION' : plugin_version.replace('"', ''),
                    },
                )
          ],
          output : 'fake',
          command : [meson.source_root() / 'version.bat', meson.current_build_dir(), dllName],
          build_by_default : true
)

install_data('xp_msgServer.ini', install_dir: 'config.example')
