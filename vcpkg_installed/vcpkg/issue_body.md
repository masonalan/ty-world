Package: glfw3:x64-osx@3.4

**Host Environment**

- Host: x64-osx
- Compiler: AppleClang 15.0.0.15000309
-    vcpkg-tool version: 2024-08-01-2024.08.01
    vcpkg-scripts version: 85392b146 2024-08-30 (23 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached glfw-glfw-3.4.tar.gz.
-- Cleaning sources at /Users/jamespickering/dev/vendor/vcpkg/buildtrees/glfw3/src/3.4-2448ff4533.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /Users/jamespickering/dev/vendor/vcpkg/downloads/glfw-glfw-3.4.tar.gz
-- Using source at /Users/jamespickering/dev/vendor/vcpkg/buildtrees/glfw3/src/3.4-2448ff4533.clean
GLFW3 currently requires the following libraries from the system package manager:
    xinerama
    xcursor
    xorg
    libglu1-mesa
    pkg-config

These can be installed via brew install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config
-- Found external ninja('1.12.1').
-- Configuring x64-osx
-- Building x64-osx-dbg
-- Building x64-osx-rel
-- Fixing pkgconfig file: /Users/jamespickering/dev/vendor/vcpkg/packages/glfw3_x64-osx/lib/pkgconfig/glfw3.pc
CMake Error at scripts/cmake/vcpkg_find_acquire_program.cmake:166 (message):
  Could not find pkg-config.  Please install it via your package manager:

      brew install pkg-config
Call Stack (most recent call first):
  scripts/cmake/vcpkg_fixup_pkgconfig.cmake:193 (vcpkg_find_acquire_program)
  /Users/jamespickering/.cache/vcpkg/registries/git-trees/fcbaa3b4073da7a24e20e043164075512f027d2d/portfile.cmake:53 (vcpkg_fixup_pkgconfig)
  scripts/ports.cmake:192 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "glfw3"
  ]
}

```
</details>
