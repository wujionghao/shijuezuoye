# 回形针自动计数

使用 C++ 和 OpenCV：灰度化、高斯滤波、Otsu 反向二值化、形态学闭运算，再通过 8 邻域连通域统计并过滤小面积噪声。结果用矩形框和编号标记。

在本文件夹打开终端，执行：

```bat
build.bat
build\class1.exe
```

默认读取 `paperclips.jpg`，也可以指定其他图片，例如 `build\class1.exe other.jpg`。结果保存为当前目录下的 `binary.png` 和 `result.png`，重复运行会覆盖；控制台输出计数。

构建依赖本机 `D:\Visual Studio\2` 的 MSVC 和 `D:\opencv` 的 OpenCV 4.12.0。面积阈值 300 像素、5×5 闭运算和边框处理针对示例图设置；其他分辨率、光照或相互接触的物体可能需要调整参数，粘连物体不能保证分别计数。
