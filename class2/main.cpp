#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    const std::string input = argc > 1 ? argv[1] : "coins.jpg";
    cv::Mat original = cv::imread(input);
    if (original.empty()) {
        std::cerr << "Cannot read image: " << input << std::endl;
        return 1;
    }

    cv::Mat gray, binary, labels, stats, centroids;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9)));
    const int components = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8);

    cv::Mat result = original.clone();
    int count = 0;
    for (int i = 1; i < components; ++i) {
        if (stats.at<int>(i, cv::CC_STAT_AREA) < 300) continue;
        const cv::Rect box(stats.at<int>(i, cv::CC_STAT_LEFT),
                           stats.at<int>(i, cv::CC_STAT_TOP),
                           stats.at<int>(i, cv::CC_STAT_WIDTH),
                           stats.at<int>(i, cv::CC_STAT_HEIGHT));
        ++count;
        cv::rectangle(result, box, cv::Scalar(0, 200, 0), 2);
        cv::putText(result, std::to_string(count), cv::Point(box.x, std::max(20, box.y - 5)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 0, 255), 2);
    }
    cv::putText(result, "Count: " + std::to_string(count), cv::Point(10, 26),
                cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
    if (!cv::imwrite("binary.png", binary) || !cv::imwrite("result.png", result)) {
        std::cerr << "Cannot write output images in current directory." << std::endl;
        return 1;
    }
    std::cout << "Count: " << count << "\nSaved: binary.png, result.png" << std::endl;
    cv::imshow("Original", original);
    cv::imshow("Binary", binary);
    cv::imshow("Counting result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
