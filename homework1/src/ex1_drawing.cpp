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

cv::Mat makeCanvas(const std::string& title)
{
    cv::Mat canvas(500, 500, CV_8UC3, cv::Scalar(24, 25, 28));
    cv::putText(canvas, title, cv::Point(28, 40), cv::FONT_HERSHEY_SIMPLEX,
                0.68, cv::Scalar(235, 238, 242), 1, cv::LINE_AA);
    return canvas;
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

void drawLines()
{
    cv::Mat canvas = makeCanvas("line(): three line styles");

    // 红色，宽度为 3，4 邻接
    cv::line(canvas, cv::Point(70, 125), cv::Point(430, 130),
             cv::Scalar(0, 0, 220), 3, cv::LINE_4);
    // 绿色，宽度为 5，8 邻接
    cv::line(canvas, cv::Point(70, 230), cv::Point(430, 235),
             cv::Scalar(0, 210, 70), 5, cv::LINE_8);
    // 蓝色，宽度为 10，抗锯齿
    cv::line(canvas, cv::Point(70, 345), cv::Point(430, 350),
             cv::Scalar(230, 120, 20), 10, cv::LINE_AA);

    cv::putText(canvas, "thickness 3, LINE_4", cv::Point(70, 105),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);
    cv::putText(canvas, "thickness 5, LINE_8", cv::Point(70, 210),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);
    cv::putText(canvas, "thickness 10, LINE_AA", cv::Point(70, 325),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);

    saveImage("output/q1_lines.png", canvas);
    cv::imshow("Exercise 1 - lines", canvas);
}

void drawRectangles()
{
    cv::Mat canvas = makeCanvas("rectangle(): outline and filled");

    // 两个顶点方式：红色边框，宽度 3
    cv::rectangle(canvas, cv::Point(55, 85), cv::Point(215, 190),
                  cv::Scalar(0, 0, 220), 3, cv::LINE_4);
    // 两个顶点方式：绿色边框，宽度 5
    cv::rectangle(canvas, cv::Point(285, 85), cv::Point(445, 190),
                  cv::Scalar(0, 210, 70), 5, cv::LINE_8);

    // cv::Rect 方式：蓝色实心矩形
    cv::Rect rect(70, 260, 130, 95);
    cv::rectangle(canvas, rect, cv::Scalar(230, 120, 20), -1, cv::LINE_AA);

    // 用 Rect 指定位置、宽度和高度，绘制黄色边框
    cv::Rect outline(270, 255, 160, 110);
    cv::rectangle(canvas, outline, cv::Scalar(40, 210, 235), 4, cv::LINE_AA);

    cv::putText(canvas, "two corner points", cv::Point(55, 220),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);
    cv::putText(canvas, "Rect, thickness = -1", cv::Point(70, 390),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);
    cv::putText(canvas, "Rect, thickness = 4", cv::Point(270, 390),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);

    saveImage("output/q1_rectangles.png", canvas);
    cv::imshow("Exercise 1 - rectangles", canvas);
}

void drawCircles()
{
    cv::Mat canvas = makeCanvas("circle(): outline and filled");

    // 描边圆：半径、颜色、线宽
    cv::circle(canvas, cv::Point(145, 175), 66, cv::Scalar(0, 0, 220), 3, cv::LINE_4);
    cv::circle(canvas, cv::Point(355, 175), 66, cv::Scalar(0, 210, 70), 6, cv::LINE_AA);

    // 实心圆，thickness = -1
    cv::circle(canvas, cv::Point(145, 355), 58, cv::Scalar(230, 120, 20), -1, cv::LINE_AA);
    cv::circle(canvas, cv::Point(355, 355), 58, cv::Scalar(40, 210, 235), 10, cv::LINE_AA);

    // 圆心的位置用一个白色小圆点标出
    cv::circle(canvas, cv::Point(145, 175), 3, cv::Scalar(255, 255, 255), -1, cv::LINE_AA);
    cv::circle(canvas, cv::Point(355, 175), 3, cv::Scalar(255, 255, 255), -1, cv::LINE_AA);

    cv::putText(canvas, "outline: thickness 3 / 6", cv::Point(55, 275),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);
    cv::putText(canvas, "filled: thickness = -1", cv::Point(55, 455),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(190, 195, 202), 1, cv::LINE_AA);

    saveImage("output/q1_circles.png", canvas);
    cv::imshow("Exercise 1 - circles", canvas);
}

int main(int argc, char* argv[])
{
    const bool showWindow = !hasFlag(argc, argv, "--no-window");

    drawLines();
    drawRectangles();
    drawCircles();

    cv::Mat lines = cv::imread("output/q1_lines.png");
    cv::Mat rectangles = cv::imread("output/q1_rectangles.png");
    cv::Mat circles = cv::imread("output/q1_circles.png");
    if (!lines.empty() && !rectangles.empty() && !circles.empty()) {
        cv::Mat montage;
        cv::hconcat(std::vector<cv::Mat>{lines, rectangles, circles}, montage);
        saveImage("output/q1_drawing.png", montage);
    }

    if (showWindow) {
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return 0;
}
