# xexe
Protect your application from antivirus on Windows

### about
- [BoxedAppSDK](https://www.boxedapp.com/boxedappsdk/) for create virtual file
- [QuickLZ](http://www.quicklz.com/) for compress/decompress data

```
[C/C++ application] +-----> [bxsdk] => execute => exit code
  > resources                  ^
    | icon                     | [decompress] :: built-in
    | rcdata: bin +------------+
               ^
               | [compress] :: ./tools/tobin.exe
[Another .exe] +
```

### usage

- Compress an executable file to **.bin**
```
$> tools/tobin.exe file.input output.bin
```

- Copy this **.bin** to **src/res/** and rename to **app.bin**
- Change icon by replacing file **src/res/icon.ico**
- Edit version info in **src/res.rc**
- Open **xexe.sln** (with Visual Studio 2010+) and build on **Release** mode
  - **consle** for console subsystem (with console application)
  - **windows** for windows subsystem (with GUI application)

You can use compiled binaries (in **bin/..**) and change its resources by using [Resource Hacker](http://www.angusj.com/resourcehacker/).
- RCData: .bin
- Icons: .ico

### compare

**AutoIt program**
```au3
#include <array.au3>

MsgBox(0, 'Test', 'Hello, world!')
_ArrayDisplay($CmdLine, 'Commandline array')
```

**VirusTotal**
- [app.exe - 9/72](https://www.virustotal.com/gui/file/8a391d804a5c61c4db02f03a3e2a9ed0cd5d0d6a07b16d7a784ceb2817d0cb66/detection)
- [console.exe - 1/72](https://www.virustotal.com/gui/file/dc7557204430504decd2e97c49c1317b27f24a3e7faebeb620c508f9425735bc/detection)
- [console_x64.exe - 1/71](https://www.virustotal.com/gui/file/76fdef0f74728b6de8ec9510b9c7eb605261e91b1e1bb7cc9e82734475d2102b/detection)
