# Homework 1 OpenCV 编程练习

本目录包含四个相互独立的 OpenCV C++ 练习，对应作业中的四道题：

| 目录/文件 | 对应题目 | 主要内容 |
| --- | --- | --- |
| `src/ex1_drawing.cpp` | 练习 1 | 画圆、画直线、画矩形 |
| `src/ex2_histogram.cpp` | 练习 2 | `float histogram[256]` 统计灰度直方图并绘图 |
| `src/ex3_channels.cpp` | 练习 3 | 分离并显示 B、G、R 三个通道 |
| `src/ex4_camera.cpp` | 练习 4 | 打开本机摄像头并实时显示 |

## 编译

在本目录打开终端，执行：

```bat
build.bat
```

脚本使用本机的 MSVC 和 `D:\opencv` 中的 OpenCV 4.12.0，生成：

```text
build\ex1_drawing.exe
build\ex2_histogram.exe
build\ex3_channels.exe
build\ex4_camera.exe
```

## 运行

```bat
build\ex1_drawing.exe
build\ex2_histogram.exe
build\ex3_channels.exe
build\ex4_camera.exe
```

图片类程序会把结果保存到 `output` 目录，同时弹出窗口；在窗口内按任意键退出。

练习 3 默认读取 `images\solar_panel.png`，也可以指定其他图片：

```bat
build\ex3_channels.exe path\to\太阳能板.png
```

如果使用老师提供的原图，把图片命名为 `solar_panel.png` 放到 `images` 目录即可覆盖当前测试图。

练习 4 默认打开 0 号摄像头，按 `Esc` 退出，按 `S` 保存当前帧到 `output\q4_snapshot.png`。也可以通过本地视频测试：

```bat
build\ex4_camera.exe --video images\solar_panel_demo.mp4
```

无窗口批处理模式（用于自动截图）：

```bat
build\ex1_drawing.exe --no-window
build\ex2_histogram.exe --no-window
build\ex3_channels.exe --no-window
build\ex4_camera.exe --video images\solar_panel_demo.mp4 --no-window --frames 5 --snapshot output\q4_snapshot.png
```

## 说明

题目中的 `太阳能板.png` 附件没有随作业网页一起提供，仓库内使用程序生成的太阳能板测试图。第 3 题的通道分离代码不依赖具体图片，换成老师提供的原图后可直接运行。
