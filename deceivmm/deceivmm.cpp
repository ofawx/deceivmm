//
//  deceivmm.cpp
//  deceivmm
//
//  Created by ofawx on 8/11/21.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_user.hpp>


// MARK: - Kern Boot Args

static const char *bootargOff[] =
{
    "-dvmmoff"
};

static const char *bootargDebug[] =
{
    "-dvmmdbg"
};

static const char *bootargBeta[] =
{
    "-dvmmbeta"
};


// MARK: - Patch Defs

static const uint32_t SectionActive = 1;

static const char* acPath = "/usr/libexec/AssetCache/AssetCache";
static const char* acmsPath = "/System/Library/PrivateFrameworks/AssetCacheServicesExtensions.framework/Versions/A/XPCServices/AssetCacheManagerService.xpc/Contents/MacOS/AssetCacheManagerService";

static const uint8_t find_lower[] = "vmm";
static const uint8_t repl_lower[] = "xxx";

static const uint8_t find_upper[] = "VMM";
static const uint8_t repl_upper[] = "XXX";

static UserPatcher::BinaryModPatch patches[] = {
    { CPU_TYPE_X86_64, 0, find_lower, repl_lower, 3, 0, 2, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_upper, repl_upper, 3, 0, 8, UserPatcher::FileSegment::SegmentTextCstring, SectionActive }
};

static UserPatcher::BinaryModInfo binaryMod[] = {
    { acPath, patches, 2 },
    { acmsPath, patches, 2 }
};

// find / -perm +111 -type f -exec sh -c 'jtool2 -L "{}" 2>/dev/null | grep AssetCacheServicesExtensions && echo "ACSE matched at {}"' \;
// plus some others??
static UserPatcher::ProcInfo procInfo[] = {
    { "launchd", 7, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "AssetCache", 10, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "AssetCacheManagerService", 24, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "dyld", 4, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "CoreFoundation", 14, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "LaunchServices", 14, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "OSServices", 10, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "AssetCacheServicesExtensions", 7, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchAny},
    { "/System/Library/PreferencePanes/SharingPref.prefPane/Contents/MacOS/SharingPref", 79, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/ConfigurationProfiles.framework/XPCServices/AssetCacheProfilePlugin.xpc/Contents/MacOS/AssetCacheProfilePlugin", 144, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/AssetCacheServicesExtensions.framework/Versions/A/XPCServices/AssetCacheTetheratorService.xpc/Contents/MacOS/AssetCacheTetheratorService", 170, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Applications/Utilities/Activity Monitor.app/Contents/MacOS/Activity Monitor", 83, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/bin/AssetCacheManagerUtil", 30, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/bin/AssetCacheTetheratorUtil", 33, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/bin/AssetCacheLocatorUtil", 30, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/libexec/AssetCache/AssetCache", 34, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/libexec/AssetCacheAgent/AssetCacheAgent", 44, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact }
};


static bool registerPatches() {
    DBGLOG("deceivmm", "Registering patches...");
    LiluAPI::Error retval = lilu.onProcLoad(procInfo, 16, nullptr, nullptr, binaryMod, 2);
    return retval == LiluAPI::Error::NoError;
}


static void start()
{
    DBGLOG("deceivmm", "Starting...");

    if (registerPatches()) {
        DBGLOG("deceivmm", "Success!");
    }
    else {
        DBGLOG("deceivmm", "Failed!");
    }
}

//
// MARK: - Conf
//

PluginConfiguration ADDPR(config) =
{
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::HighSierra,
    KernelVersion::Monterey,
    start
};
