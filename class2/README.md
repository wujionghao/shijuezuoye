# 硬币连通域计数

使用灰度化、高斯滤波、Otsu 二值化和形态学闭运算，再用 8 邻域连通域统计并过滤小面积噪声。示例图中深色硬币的亮边与内部也会被作为同一连通域。

在 VS Code 中打开本工程的 `main.cpp`，选择 `Current project (MSVC x64)` 并按 F5。也可以在本目录的终端运行：

```bat
build.bat
build\app.exe
```

默认读取 `coins.jpg`；也可运行 `build\app.exe other.jpg` 指定图片。程序保存 `binary.png` 和 `result.png`，并显示原图、二值化图、计数结果三个窗口，在图片窗口按任意键退出。

构建需要 `D:\Visual Studio\2` 中的 MSVC 和 `D:\opencv` 中的 OpenCV 4.12.0。参数针对示例图设置，光照、分辨率不同或硬币相互接触时可能需要调整，粘连物体不能保证分别计数。
