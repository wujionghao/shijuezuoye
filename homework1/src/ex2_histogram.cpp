#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool hasFlag(int argc, char* argv[], const std::string& flag)
{
    for (int i = 1; i < argc; ++i) {
        if (flag == argv[i]) return true;
    }
    return false;
}

std::string inputPath(int argc, char* argv[])
{
    for (int i = 1; i + 1 < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--input") return argv[i + 1];
        if (arg == "--no-window") continue;
    }
    return "images/solar_panel.png";
}

}  // namespace

int main(int argc, char* argv[])
{
    const std::string input = inputPath(argc, argv);
    const bool showWindow = !hasFlag(argc, argv, "--no-window");

    cv::Mat gray = cv::imread(input, cv::IMREAD_GRAYSCALE);
    if (gray.empty()) {
        std::cerr << "Cannot read image: " << input << std::endl;
        return 1;
    }

    // 题目要求：容量为 256 的 float 型数组。
    float histogram[256] = {0.0f};

    // 逐像素遍历，把每个灰度值出现的次数累加到对应下标。
    for (int y = 0; y < gray.rows; ++y) {
        const unsigned char* row = gray.ptr<unsigned char>(y);
        for (int x = 0; x < gray.cols; ++x) {
            histogram[row[x]] += 1.0f;
        }
    }

    float maxCount = 0.0f;
    int maxBin = 0;
    double sum = 0.0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxCount) {
            maxCount = histogram[i];
            maxBin = i;
        }
        sum += static_cast<double>(histogram[i]) * i;
    }
    const double totalPixels = static_cast<double>(gray.rows) * gray.cols;
    const double mean = sum / totalPixels;

    // 绘制直方图：白色画布、坐标轴、浅灰网格、蓝色柱线，并用圆点标出峰值。
    cv::Mat plot(460, 820, CV_8UC3, cv::Scalar(250, 250, 250));
    const int left = 72;
    const int right = 28;
    const int top = 70;
    const int bottom = 62;
    const int plotWidth = plot.cols - left - right;
    const int plotHeight = plot.rows - top - bottom;
    const int baseline = plot.rows - bottom;

    cv::putText(plot, "Gray-level histogram", cv::Point(left, 36),
                cv::FONT_HERSHEY_SIMPLEX, 0.82, cv::Scalar(38, 42, 48), 2, cv::LINE_AA);
    cv::putText(plot, cv::format("pixels: %.0f    mean: %.2f    peak: bin %d (%.0f pixels)",
                                 totalPixels, mean, maxBin, maxCount),
                cv::Point(left, 58), cv::FONT_HERSHEY_SIMPLEX, 0.43,
                cv::Scalar(90, 96, 104), 1, cv::LINE_AA);

    // 网格线：4 条水平线。
    for (int k = 0; k <= 4; ++k) {
        const int y = baseline - plotHeight * k / 4;
        cv::line(plot, cv::Point(left, y), cv::Point(left + plotWidth, y),
                 cv::Scalar(224, 226, 230), 1, cv::LINE_AA);
    }

    // 256 根柱线全部用 line 绘制。
    const double binWidth = static_cast<double>(plotWidth) / 256.0;
    for (int i = 0; i < 256; ++i) {
        const int x = left + static_cast<int>(i * binWidth + binWidth * 0.5);
        const int h = static_cast<int>(std::round(histogram[i] / maxCount * plotHeight));
        const cv::Scalar color(220.0 - 0.35 * i, 105.0 + 0.35 * i, 30.0 + 0.25 * i);
        cv::line(plot, cv::Point(x, baseline), cv::Point(x, baseline - h),
                 color, 2, cv::LINE_AA);
    }

    // 直方图包络线，并在每 16 个灰度级处画圆点。
    std::vector<cv::Point> curve;
    curve.reserve(256);
    for (int i = 0; i < 256; ++i) {
        const int x = left + static_cast<int>(i * binWidth + binWidth * 0.5);
        const int y = baseline - static_cast<int>(std::round(histogram[i] / maxCount * plotHeight));
        curve.emplace_back(x, y);
    }
    cv::polylines(plot, curve, false, cv::Scalar(60, 85, 210), 1, cv::LINE_AA);
    for (int i = 0; i < 256; i += 16) {
        cv::circle(plot, curve[i], 3, cv::Scalar(30, 50, 170), -1, cv::LINE_AA);
    }
    cv::circle(plot, curve[maxBin], 5, cv::Scalar(0, 0, 220), -1, cv::LINE_AA);
    cv::putText(plot, "peak", curve[maxBin] + cv::Point(-16, -12),
                cv::FONT_HERSHEY_SIMPLEX, 0.42, cv::Scalar(0, 0, 190), 1, cv::LINE_AA);

    // 坐标轴与刻度。
    cv::line(plot, cv::Point(left, top), cv::Point(left, baseline),
             cv::Scalar(45, 50, 56), 2, cv::LINE_AA);
    cv::line(plot, cv::Point(left, baseline), cv::Point(left + plotWidth, baseline),
             cv::Scalar(45, 50, 56), 2, cv::LINE_AA);
    for (int value = 0; value <= 255; value += 64) {
        const int x = left + static_cast<int>(value * binWidth);
        cv::line(plot, cv::Point(x, baseline), cv::Point(x, baseline + 6),
                 cv::Scalar(45, 50, 56), 1, cv::LINE_AA);
        cv::putText(plot, std::to_string(value), cv::Point(x - 12, baseline + 24),
                    cv::FONT_HERSHEY_SIMPLEX, 0.42, cv::Scalar(70, 76, 84), 1, cv::LINE_AA);
    }
    cv::putText(plot, "gray level", cv::Point(left + plotWidth / 2 - 44, plot.rows - 16),
                cv::FONT_HERSHEY_SIMPLEX, 0.46, cv::Scalar(70, 76, 84), 1, cv::LINE_AA);
    cv::putText(plot, "count", cv::Point(8, top + 6),
                cv::FONT_HERSHEY_SIMPLEX, 0.46, cv::Scalar(70, 76, 84), 1, cv::LINE_AA);

    if (!cv::imwrite("output/q2_gray.png", gray) ||
        !cv::imwrite("output/q2_histogram.png", plot)) {
        std::cerr << "Cannot write output images." << std::endl;
        return 1;
    }

    std::cout << "Total pixels: " << totalPixels << "\n"
              << "Mean gray level: " << mean << "\n"
              << "Peak bin: " << maxBin << " (" << maxCount << " pixels)\n"
              << "Saved: output/q2_gray.png, output/q2_histogram.png" << std::endl;

    if (showWindow) {
        cv::imshow("Exercise 2 - gray image", gray);
        cv::imshow("Exercise 2 - histogram", plot);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return 0;
}
