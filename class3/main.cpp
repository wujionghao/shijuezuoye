#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    const std::string input = argc > 1 ? argv[1] : "part.jpg";
    cv::Mat original = cv::imread(input);
    if (original.empty()) {
        std::cerr << "Cannot read image: " << input << std::endl;
        return 1;
    }

    cv::Mat gray, binary, labels, stats, centroids;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::threshold(gray, binary, 95, 255, cv::THRESH_BINARY_INV);
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    int components = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8);

    cv::Mat result = original.clone();
    cv::Mat selected = cv::Mat::zeros(binary.size(), CV_8UC1);
    int count = 0;
    for (int i = 1; i < components; ++i) {
        int x = stats.at<int>(i, cv::CC_STAT_LEFT);
        int y = stats.at<int>(i, cv::CC_STAT_TOP);
        int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
        int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (x == 0 || y == 0 || x + w >= binary.cols || y + h >= binary.rows) continue;
        if (area < 400 || area > 12000) continue;
        double aspect = static_cast<double>(w) / h;
        double fill = static_cast<double>(area) / (w * h);
        if (aspect < 0.7 || aspect > 1.3 || fill < 0.5) continue;

        selected.setTo(255, labels == i);
        ++count;
        cv::rectangle(result, cv::Rect(x, y, w, h), cv::Scalar(0, 255, 0), 2);
        cv::putText(result, std::to_string(count), cv::Point(x, std::max(20, y - 5)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
    }
    cv::putText(result, "Count: " + std::to_string(count), cv::Point(10, 28),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    if (!cv::imwrite("binary.png", binary) ||
        !cv::imwrite("selected.png", selected) ||
        !cv::imwrite("result.png", result)) {
        std::cerr << "Cannot write output images in current directory." << std::endl;
        return 1;
    }
    std::cout << "Count: " << count << "\nSaved: binary.png, selected.png, result.png" << std::endl;
    cv::imshow("Original", original);
    cv::imshow("Binary", binary);
    cv::imshow("Selected holes", selected);
    cv::imshow("Detection result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
