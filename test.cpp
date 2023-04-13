#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    // 打开网络相机
    string url = "YOUR_CAMERA_URL"; // 替换为您的相机URL
    VideoCapture cap(url);
    if (!cap.isOpened()) {
        cout << "Failed to open camera." << endl;
        return -1;
    }

    while (true) {
        // 获取下一帧
        Mat frame;
        if (!cap.read(frame)) {
            cout << "Failed to read frame." << endl;
            break;
        }

        // 保存图像
        string filename = "image.jpg";
        imwrite(filename, frame);

        // 显示图像
        imshow("Frame", frame);

        // 按下ESC键退出循环
        if (waitKey(1) == 27) {
            break;
        }
    }

    // 关闭相机
    cap.release();
    return 0;
}
