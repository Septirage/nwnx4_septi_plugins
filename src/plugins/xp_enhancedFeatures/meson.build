plugin_name = 'xp_enhancedFeatures'+global_name_project_add

plugin_shared_lib = shared_library(plugin_name,
    [
        nwnx4_repo_path / 'src/plugins/plugin.cpp',
        nwnx4_repo_path / 'src/misc/log.cpp',
        nwnx4_repo_path / 'src/hook/scriptManagement.cpp',
        nwnx4_repo_path / 'src/misc/Patch.cpp',
        'EnhancedFeatures.cpp',
        'MagicFunctions.cpp',
        'SmallPatchFunctions.cpp',
        'SmallHookFunctions.cpp',
        'SkillHooks.cpp',
        'SpeedFeats.cpp',
        'RuleCreatureFct.cpp',
        'ReduceSpeedHook.cpp',
        'WeaponFinesseHook.cpp',
        'RuleParser.cpp',
        'BonusParser.cpp',
        'StoreRetrievePatch.cpp',
        '../../septutil/intern2daMgt.cpp',
        '../../septutil/crypto.cpp',
        '../../septutil/srvadmin.cpp',
        '../../septutil/win_utils.cpp',
    ],
    include_directories: include_dirs,
    dependencies: [
        cppcompiler.find_library('version'),
        cppcompiler.find_library('detours', dirs: lib_dirs),
        cppcompiler.find_library('NWN2_MemoryMgr_amdxp', dirs: lib_dirs),
    ],
    install: true,
    install_dir: 'plugins',
)


# Get version from git tags
version = run_command('git', 'describe', '--tags', '--match=eff-*', check: true).stdout().substring(4, -1)

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

install_data('xp_enhancedFeatures.ini', install_dir: 'config.example')
install_data('SpeedFeatsEFF.ini', install_dir: 'config.example')
install_data('SkillFeatsEFF.ini', install_dir: 'config.example')
install_data('WeaponFinesseEFF.ini', install_dir: 'config.example')
