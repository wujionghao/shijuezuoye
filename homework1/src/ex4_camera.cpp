#include <opencv2/opencv.hpp>

#include <filesystem>
#include <iostream>
#include <string>

namespace {

struct Options {
    int cameraIndex = 0;
    std::string videoPath;
    std::string snapshotPath = "output/q4_snapshot.png";
    int frames = 0;
    bool showWindow = true;
    bool takeSnapshot = false;
};

Options parseOptions(int argc, char* argv[])
{
    Options options;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--no-window") {
            options.showWindow = false;
        } else if (arg == "--video" && i + 1 < argc) {
            options.videoPath = argv[++i];
        } else if (arg == "--index" && i + 1 < argc) {
            options.cameraIndex = std::stoi(argv[++i]);
        } else if (arg == "--frames" && i + 1 < argc) {
            options.frames = std::stoi(argv[++i]);
        } else if (arg == "--snapshot" && i + 1 < argc) {
            options.snapshotPath = argv[++i];
            options.takeSnapshot = true;
        }
    }
    return options;
}

void ensureParentDirectory(const std::string& path)
{
    const std::filesystem::path parent = std::filesystem::path(path).parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }
}

}  // namespace

int main(int argc, char* argv[])
{
    const Options options = parseOptions(argc, argv);

    cv::VideoCapture cap;
    if (!options.videoPath.empty()) {
        cap.open(options.videoPath);
    } else {
        // 优先使用 DirectShow；若失败，再尝试 OpenCV 默认后端。
        cap.open(options.cameraIndex, cv::CAP_DSHOW);
        if (!cap.isOpened()) {
            cap.open(options.cameraIndex);
        }
    }

    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件或摄像头 " << options.cameraIndex << std::endl;
        return 2;
    }

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    std::cout << "Opened " << (options.videoPath.empty() ? "camera " : "video ")
              << (options.videoPath.empty() ? std::to_string(options.cameraIndex) : options.videoPath)
              << "\nfps: " << fps << "\nresolution: " << width << " x " << height << std::endl;

    int frameCount = 0;
    cv::Mat frame;
    while (cap.read(frame)) {
        if (frame.empty()) {
            std::cerr << "不能从视频中读取帧" << std::endl;
            break;
        }

        if (options.takeSnapshot && frameCount == 0) {
            ensureParentDirectory(options.snapshotPath);
            if (cv::imwrite(options.snapshotPath, frame)) {
                std::cout << "Saved snapshot: " << options.snapshotPath << std::endl;
            } else {
                std::cerr << "Cannot write snapshot: " << options.snapshotPath << std::endl;
            }
        }

        if (options.showWindow) {
            cv::imshow("Camera", frame);
            const int key = cv::waitKey(30);
            if (key == 27) break;
            if (key == 's' || key == 'S') {
                ensureParentDirectory(options.snapshotPath);
                if (cv::imwrite(options.snapshotPath, frame)) {
                    std::cout << "Saved snapshot: " << options.snapshotPath << std::endl;
                }
            }
        } else {
            cv::waitKey(1);
        }

        ++frameCount;
        if (options.frames > 0 && frameCount >= options.frames) break;
    }

    cap.release();
    if (options.showWindow) {
        cv::destroyAllWindows();
    }
    return 0;
}
