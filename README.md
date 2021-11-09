# deceivmm
*Pre-alpha, not currently working. Open a PR!*

Lilu plugin to enable AssetCache (Content Caching) on macOS when running in a virtual machine.

## Build
1. Clone
2. Add debug Lilu.kext to the project folder
3. Open Xcode project, > Build

## Install
1. Add deceivmm.kext to OC kexts, alongside Lilu.kext
2. Add deceivmm.kext to OC config.plist, after Lilu.kext
