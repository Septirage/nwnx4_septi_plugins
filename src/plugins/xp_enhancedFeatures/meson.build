plugin_version = '"1.0.2"'
plugin_name = 'xp_enhancedFeatures'+global_name_project_add
dllName = plugin_name+'.dll'


shared_library_obj = shared_library(plugin_name,
    [
        '../plugin.cpp',
        '../../misc/log.cpp',
        '../../hook/scriptManagement.cpp',
        '../../misc/Patch.cpp',
        'EnhancedFeatures.cpp',
        'MagicFunctions.cpp',
        'SmallPatchFunctions.cpp',
        'SmallHookFunctions.cpp',
        'SkillHooks.cpp',
        'SpeedFeats.cpp',
        'WeaponFinesseHook.cpp',
        'RuleParser.cpp',
        'BonusParser.cpp',
        'StoreRetrievePatch.cpp',
        '../../septutil/crypto.cpp',
        '../../septutil/srvadmin.cpp',
    ],
    cpp_args: ['-DPLUGIN_VERSION='+plugin_version],
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

          
install_data('xp_enhancedFeatures.ini', install_dir: 'config.example')
install_data('SpeedFeatsEFF.ini', install_dir: 'config.example')
install_data('SkillFeatsEFF.ini', install_dir: 'config.example')
install_data('WeaponFinesseEFF.ini', install_dir: 'config.example')
