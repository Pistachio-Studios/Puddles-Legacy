Package: box2d:x64-linux@2.3.1-374664b-2

**Host Environment**

- Host: x64-linux
- Compiler: GNU 13.2.1
-    vcpkg-tool version: 2024-02-07-8a83681f921b10d86ae626fd833c253f4f8c355b
    vcpkg-scripts version: 215a25355 2024-03-01 (3 days ago)

**To Reproduce**

`vcpkg install `
**Failure logs**

```
CMake Warning at scripts/cmake/vcpkg_common_functions.cmake:3 (message):
  vcpkg_common_functions has been removed and all values are automatically
  provided in all portfile.cmake invocations.  Please remove
  `include(vcpkg_common_functions)`.
Call Stack (most recent call first):
  buildtrees/versioning_/versions/box2d/042a70aea5f44f5fe8cc7c5dfc36229badcf264b/portfile.cmake:1 (include)
  scripts/ports.cmake:172 (include)


-- Downloading https://github.com/erincatto/Box2D/archive/374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz -> erincatto-Box2D-374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz...
[DEBUG] To include the environment variables in debug output, pass --debug-env
[DEBUG] Trying to load bundleconfig from /home/hugo/vcpkg/vcpkg-bundle.json
[DEBUG] Failed to open: /home/hugo/vcpkg/vcpkg-bundle.json
[DEBUG] Bundle config: readonly=false, usegitregistry=false, embeddedsha=nullopt, deployment=Git, vsversion=nullopt
[DEBUG] Metrics enabled.
[DEBUG] Feature flag 'binarycaching' unset
[DEBUG] Feature flag 'compilertracking' unset
[DEBUG] Feature flag 'registries' unset
[DEBUG] Feature flag 'versions' unset
[DEBUG] Feature flag 'dependencygraph' unset
[DEBUG] 1000: execute_process(curl --fail -L https://github.com/erincatto/Box2D/archive/374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz --create-dirs --output /var/cache/vcpkg/erincatto-Box2D-374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz.153022.part)
[DEBUG] 1000: cmd_execute_and_stream_data() returned 0 after   859908 us
[DEBUG] Trying to hash /var/cache/vcpkg/erincatto-Box2D-374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz.153022.part
[DEBUG] /var/cache/vcpkg/erincatto-Box2D-374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz.153022.part has hash 89a46287a7dcb937d58454237d8ca0d3f5788bd544ccd045a282c38c7d197380fa5c5264c5dcfa9e19f250b3c9887dbe2252b188fc7ecc40b74ed241d7634650
error: Failed to download from mirror set
error: File does not have the expected hash:
url: https://github.com/erincatto/Box2D/archive/374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz
File: /var/cache/vcpkg/erincatto-Box2D-374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc.tar.gz.153022.part
Expected hash: 39074bab01b36104aa685bfe39b40eb903d9dfb54cc3ba8098125db5291f55a8a9e578fc59563b2e8743abbbb26f419be7ae1524e235e7bd759257f99ff96bda
Actual hash: 89a46287a7dcb937d58454237d8ca0d3f5788bd544ccd045a282c38c7d197380fa5c5264c5dcfa9e19f250b3c9887dbe2252b188fc7ecc40b74ed241d7634650
[DEBUG] /mnt/vss/_work/1/s/src/vcpkg/base/downloads.cpp(1030): 
[DEBUG] Time in subprocesses: 859908us
[DEBUG] Time in parsing JSON: 7us
[DEBUG] Time in JSON reader: 0us
[DEBUG] Time in filesystem: 147us
[DEBUG] Time in loading ports: 0us
[DEBUG] Exiting after 890 ms (889547us)

CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:32 (message):
      
      Failed to download file with error: 1
      If you are using a proxy, please check your proxy setting. Possible causes are:
      
      1. You are actually using an HTTP proxy, but setting HTTPS_PROXY variable
         to `https://address:port`. This is not correct, because `https://` prefix
         claims the proxy is an HTTPS proxy, while your proxy (v2ray, shadowsocksr
         , etc..) is an HTTP proxy. Try setting `http://address:port` to both
         HTTP_PROXY and HTTPS_PROXY instead.
      
      2. If you are using Windows, vcpkg will automatically use your Windows IE Proxy Settings
         set by your proxy software. See https://github.com/microsoft/vcpkg-tool/pull/77
         The value set by your proxy might be wrong, or have same `https://` prefix issue.
      
      3. Your proxy's remote server is out of service.
      
      If you've tried directly download the link, and believe this is not a temporary
      download server failure, please submit an issue at https://github.com/Microsoft/vcpkg/issues
      to report this upstream download server failure.
      

Call Stack (most recent call first):
  scripts/cmake/vcpkg_download_distfile.cmake:270 (z_vcpkg_download_distfile_show_proxy_and_fail)
  scripts/cmake/vcpkg_from_github.cmake:106 (vcpkg_download_distfile)
  buildtrees/versioning_/versions/box2d/042a70aea5f44f5fe8cc7c5dfc36229badcf264b/portfile.cmake:5 (vcpkg_from_github)
  scripts/ports.cmake:172 (include)



```
**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "proyecto-2",
  "version": "0.1.0",
  "dependencies": [
    "sdl2",
    "sdl2-image",
    "sdl2-mixer",
    "sdl2-ttf",
    "pugixml",
    "box2d"
  ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc",
  "overrides": [
    {
      "name": "box2d",
      "version": "2.3.1-374664b-2"
    }
  ]
}

```
</details>
