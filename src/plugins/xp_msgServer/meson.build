plugin_version = '"1.2.0"'
plugin_name = 'xp_msgServer'+global_name_project_add
dllName = plugin_name+'.dll'



shared_library_obj = shared_library(plugin_name,
    [
        '../plugin.cpp',
        '../../hook/scriptManagement.cpp',
        '../../misc/log.cpp',
        '../../misc/Patch.cpp',
        'Connection.cpp',
        'Anticheat.cpp',
        '../../septutil/crypto.cpp',
        '../../septutil/srvadmin.cpp',
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
          input : [shared_library_obj, configure_file(
    input : meson.source_root() / 'Version.rc.in',
    output : 'Version.rc',
    configuration : {
        'PRODUCTVERSION' : plugin_version.replace('"', ''),
    },
)],
          output : 'fake',
          command : [meson.source_root() / 'version.bat', meson.current_build_dir(), dllName],
          build_by_default : true)

install_data('xp_msgServer.ini', install_dir: 'config.example')
