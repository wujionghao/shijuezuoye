# VSCode + OpenCV 环境测试

对应课件《RGB 图像像素操作》：读图 → 逐像素把 BGR 三个通道都除以 2（整体变暗一半）→ `imshow` 显示。
用它来验证「VSCode + C++ + OpenCV」这条工具链是通的。

## 本机环境（已实测通过）

| 项目 | 值 |
| --- | --- |
| 编译器 | MSVC 14.40（Visual Studio Professional 2022，`D:\Visual Studio\2`） |
| OpenCV | 4.12.0 官方预编译版，`D:\opencv`（`build\include`、`build\x64\vc16\lib`） |
| 构建方式 | `build.bat`（内部调用 `vcvars64.bat` 后用 `cl` 编译链接） |
| 运行库 | 编译后自动把 `opencv_world4120.dll` 复制到 `build\` |

> 注意：本机装的 MinGW g++ 与这份 OpenCV 预编译库（MSVC 版 `.lib`）不兼容，所以这里走 MSVC。
> 另外本机没有单独安装 CMake，因此用 `build.bat` + `cl` 直接编译，不依赖 CMake。

## 文件说明

```
main.cpp                  课件代码（唯一改动：加了读图失败的提示，文件缺失时不会异常退出）
coins.png                 测试用图（脚本生成的 5 枚“硬币”，替代课件里的 D:\coins.png）
build.bat                 一键编译脚本
build/app.exe             编译产物
build/screenshot.png      实际运行时的窗口截图（验证 imshow 正常）
.vscode/tasks.json        Ctrl+Shift+B 编译任务
.vscode/launch.json       F5 调试（cppvsdbg），会先自动编译
.vscode/c_cpp_properties.json  IntelliSense 头文件/标准配置
```

## 怎么跑

**方式一：VSCode**
1. 用 VSCode 打开本文件夹 `E:\~大三\视觉作业`
2. 装扩展：C/C++（ms-vscode.cpptools）
3. 打开 `test/main.cpp`，选择 `Current project (MSVC x64)`；按 `F5` 编译并运行，或按 `Ctrl+F5` 不调试运行
4. 弹出 `test` 窗口后按任意键关闭

**方式二：命令行**
```bat
build.bat
build\app.exe
```
（必须在 `E:\~大三\视觉作业\test` 目录下运行，因为 `imread("coins.png")` 是相对当前工作目录。）

## 预期结果

窗口标题 `test`，深灰背景上有 3 枚可见的“硬币”（颜色比原图暗一半），按键盘任意键退出，返回码 0。
