# windows下单dll编译说明：

要用这个来装mapnik的static版

```shell
.\vcpkg\vcpkg install mapnik[input-csv,input-gdal,input-geobuf,input-geojson,input-ogr,input-pgraster,input-postgis,input-raster,input-shape,input-sqlite,input-topojson] --triplet=x64-windows-static-release
```

用`CMakeLists_single_dll.txt`来配置

`cmake.configureArgs`也要加上`-DVCPKG_TARGET_TRIPLET=x64-windows-static-release -DMAPNIK_JNI_MSVC_USE_STATIC_LINKING=ON`

`.vscode/settings.json`:

```json
{
    "cmake.configureArgs": [
        "-DBoost_NO_WARN_NEW_VERSIONS=ON",
        "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "-DVCPKG_TARGET_TRIPLET=x64-windows-static-release",
        "-DMAPNIK_JNI_MSVC_USE_STATIC_LINKING=ON",
    ]
}
```

# 1. 编译步骤

## 1.1 安装vcpkg

向环境变量中添加：

```
VCPKG_DEFAULT_TRIPLET=x64-windows
```

将vcpkg克隆到希望的安装路径中，并下载`vcpkg.exe`

```shell
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

# 1.2 编译mapnik

使用如下指令下载并编译mapnik和它的依赖

```shell
.\vcpkg\vcpkg install mapnik[input-csv,input-gdal,input-geobuf,input-geojson,input-ogr,input-pgraster,input-postgis,input-raster,input-shape,input-sqlite,input-topojson]
```

将编译完成后的路径添加到环境变量`PATH`中：

```
C:\vcpkg\installed\x64-windows\bin
```

其中`C:\vcpkg`为vcpkg的安装路径

## 1.3 编译

### 1.3.1 克隆项目到本地

```shell
git clone https://github.com/FredBill1/mapnik-jni-win.git
cd mapnik-jni-win
```

### 1.3.2 编译mapnik-jin.dll

使用如下指令配置CMake：

```shell
cmake -DBoost_NO_WARN_NEW_VERSIONS=ON -Bbuild -G "Visual Studio 17 2022" -T host=x64 -A x64 --toolchain "C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

其中`Visual Studio 17 2022`为Visual Studio安装的版本，`C:\vcpkg`为vcpkg的安装路径。

配置完成后，使用如下指令编译本项目：

```shell
cmake --build build --config Release --target ALL_BUILD -j 18 --
```

如果编译成功，`mapnik-jni.dll`将会出现在`build\Release`目录下。

将`mapnik-jni.dll`拷贝到`C:\vcpkg\installed\x64-windows\bin`目录下。

### 1.3.3 编译mapnik-jni.jar

使用如下指令编译`mapnik-jni.jar`(需要事先安装Apache Ant)

```shell
ant jar
```

如果编译成功，`mapnik-jni.jar`将会出现在`build\dist`目录下。

将`mapnik-jni.jar`拷贝到Java工程中即可。

# 2. Demo

将编译好的`mapnik-jni.jar`拷贝到`demo\lib`下，然后使用如下指令运行demo：

```shell
cd demo
java -cp lib/mapnik-jni.jar src/App.java
```

如果运行成功，`demo`文件夹下将出现`result.png`。
