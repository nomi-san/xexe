# xexe
Protect your application from antivirus on Windows

# about
- [BoxedAppSDK](https://www.boxedapp.com/boxedappsdk/) for create virtual file
- [QuickLZ](http://www.quicklz.com/) for compress/decompress data

# usage

- Compress a file to **.bin**
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
