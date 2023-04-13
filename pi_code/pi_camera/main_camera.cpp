#include <iostream>
#include <vector>
#include <fstream>
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

    // 创建用于存储质心坐标的文件
    ofstream outfile("centroids.txt", ios::app);
    if (!outfile.is_open()) {
        cout << "Failed to open file." << endl;
        return -1;
    }

    // 循环读取每一帧图像、计算质心并将其保存到文件中
    while (true) {
        // 获取下一帧
        Mat frame;
        if (!cap.read(frame)) {
            cout << "Failed to read frame." << endl;
            break;
        }

        // 将图像转换为二值化图像
        Mat binary;
        cvtColor(frame, binary, COLOR_BGR2GRAY);
        threshold(binary, binary, 0, 255, THRESH_BINARY);

        // 计算区域属性
        vector<vector<Point>> contours;
        findContours(binary, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        vector<Moments> moments(contours.size());
        for (size_t i = 0; i < contours.size(); i++) {
            moments[i] = cv::moments(contours[i], false);
        }

        // 提取质心坐标并将其保存到文件中
        for (size_t i = 0; i < contours.size(); i++) {
            Point2f centroid(static_cast<float>(moments[i].m10 / moments[i].m00),
                              static_cast<float>(moments[i].m01 / moments[i].m00));
            outfile << centroid.x << " " << centroid.y << endl;
        }
    }

    // 关闭相机和文件
    cap.release();
    outfile.close();

    return 0;
}
