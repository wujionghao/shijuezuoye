#include <opencv2/opencv.hpp>

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
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--no-window") continue;
        if (arg == "--input") {
            if (i + 1 < argc) return argv[i + 1];
            break;
        }
        if (!arg.empty() && arg[0] != '-') return arg;
    }
    return "images/solar_panel.png";
}

cv::Mat toBgr(const cv::Mat& gray)
{
    cv::Mat bgr;
    cv::cvtColor(gray, bgr, cv::COLOR_GRAY2BGR);
    return bgr;
}

cv::Mat makeTile(const cv::Mat& image, const std::string& title)
{
    const int tileWidth = 470;
    const int tileHeight = 264;
    const int labelHeight = 36;
    cv::Mat tile(tileHeight + labelHeight, tileWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    cv::Mat resized;
    cv::resize(image, resized, cv::Size(tileWidth, tileHeight), 0, 0, cv::INTER_AREA);
    resized.copyTo(tile(cv::Rect(0, labelHeight, tileWidth, tileHeight)));
    cv::putText(tile, title, cv::Point(14, 25), cv::FONT_HERSHEY_SIMPLEX,
                0.64, cv::Scalar(30, 34, 40), 1, cv::LINE_AA);
    return tile;
}

void saveImage(const std::string& path, const cv::Mat& image)
{
    if (!cv::imwrite(path, image)) {
        std::cerr << "Cannot write " << path << std::endl;
    } else {
        std::cout << "Saved: " << path << std::endl;
    }
}

}  // namespace

int main(int argc, char* argv[])
{
    const std::string input = inputPath(argc, argv);
    const bool showWindow = !hasFlag(argc, argv, "--no-window");

    cv::Mat src_color = cv::imread(input);
    if (src_color.empty()) {
        std::cerr << "Cannot read image: " << input << std::endl;
        return 1;
    }

    // OpenCV 读入彩色图后的通道顺序是 B、G、R。
    std::vector<cv::Mat> channels;
    cv::split(src_color, channels);
    cv::Mat B = channels.at(0);
    cv::Mat G = channels.at(1);
    cv::Mat R = channels.at(2);

    saveImage("output/q3_original.png", src_color);
    saveImage("output/q3_blue.png", B);
    saveImage("output/q3_green.png", G);
    saveImage("output/q3_red.png", R);

    cv::Mat top;
    cv::Mat bottom;
    cv::hconcat(std::vector<cv::Mat>{
        makeTile(src_color, "Original BGR image"),
        makeTile(toBgr(B), "Blue channel (B)")
    }, top);
    cv::hconcat(std::vector<cv::Mat>{
        makeTile(toBgr(G), "Green channel (G)"),
        makeTile(toBgr(R), "Red channel (R)")
    }, bottom);
    cv::Mat montage;
    cv::vconcat(top, bottom, montage);
    saveImage("output/q3_channels.png", montage);

    std::cout << "Image: " << input << " (" << src_color.cols << " x "
              << src_color.rows << ")\n";
    if (showWindow) {
        cv::imshow("Original Mat", src_color);
        cv::imshow("Blue channel", B);
        cv::imshow("Green channel", G);
        cv::imshow("Red channel", R);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return 0;
}
