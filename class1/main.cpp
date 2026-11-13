#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    const std::string input = argc > 1 ? argv[1] : "paperclips.jpg";
    cv::Mat image = cv::imread(input);
    if (image.empty()) {
        std::cerr << "Cannot read image: " << input << std::endl;
        return 1;
    }

    cv::Mat original = image.clone();
    cv::Mat gray, binary, labels, stats, centroids;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    // Remove the dark frame before joining nearby parts of each paperclip.
    const int border = std::min(5, std::min(binary.rows, binary.cols) / 2);
    if (border > 0) {
        binary.rowRange(0, border).setTo(0);
        binary.rowRange(binary.rows - border, binary.rows).setTo(0);
        binary.colRange(0, border).setTo(0);
        binary.colRange(binary.cols - border, binary.cols).setTo(0);
    }
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    const int components = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8);

    int count = 0;
    const int minArea = 300;
    for (int i = 1; i < components; ++i) {
        if (stats.at<int>(i, cv::CC_STAT_AREA) < minArea) continue;
        const cv::Rect box(stats.at<int>(i, cv::CC_STAT_LEFT),
                           stats.at<int>(i, cv::CC_STAT_TOP),
                           stats.at<int>(i, cv::CC_STAT_WIDTH),
                           stats.at<int>(i, cv::CC_STAT_HEIGHT));
        ++count;
        cv::rectangle(image, box, cv::Scalar(0, 180, 0), 2);
        cv::putText(image, std::to_string(count), cv::Point(box.x, std::max(20, box.y - 5)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 0, 255), 2);
    }
    cv::putText(image, "Count: " + std::to_string(count), cv::Point(15, 28),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    if (!cv::imwrite("binary.png", binary) || !cv::imwrite("result.png", image)) {
        std::cerr << "Cannot write output images in current directory." << std::endl;
        return 1;
    }
    std::cout << "Count: " << count << "\nSaved: binary.png, result.png" << std::endl;
    cv::imshow("Original", original);
    cv::imshow("Binary", binary);
    cv::imshow("Counting result", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
