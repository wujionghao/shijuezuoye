#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;

int main()
{
    Mat srcMat = imread("coins.png");

    if (srcMat.empty())
    {
        std::cout << "imread failed: coins.png not found in current directory: "
                  << std::filesystem::current_path().string() << std::endl;
        return -1;
    }

    int height = srcMat.rows;
    int width = srcMat.cols;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            srcMat.at<Vec3b>(j, i)[0] = srcMat.at<Vec3b>(j, i)[0] / 2;
            srcMat.at<Vec3b>(j, i)[1] = srcMat.at<Vec3b>(j, i)[1] / 2;
            srcMat.at<Vec3b>(j, i)[2] = srcMat.at<Vec3b>(j, i)[2] / 2;
        }
    }

    imshow("test", srcMat);
    waitKey(0);
    return 0;
}
