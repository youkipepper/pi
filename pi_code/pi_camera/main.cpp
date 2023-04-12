#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // 读取图像
    Mat G3 = imread(argv[1]);

    // 将图像转换为二值化图像
    Mat G4;
    cvtColor(G3, G4, COLOR_BGR2GRAY);
    threshold(G4, G4, 0, 255, THRESH_BINARY);

    // 计算区域属性
    vector<vector<Point>> contours;
    findContours(G4, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<Moments> moments(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
        moments[i] = moments(contours[i], false);
    }

    // 提取质心坐标
    vector<Point2f> centroids(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
        centroids[i] = Point2f(static_cast<float>(moments[i].m10 / moments[i].m00),
                                static_cast<float>(moments[i].m01 / moments[i].m00));
    }

    // 将质心坐标存储到文件中
    ofstream outfile("centroids.txt");
    if (outfile.is_open()) {
        for (size_t i = 0; i < centroids.size(); i++) {
            outfile << centroids[i].x << " " << centroids[i].y << endl;
        }
        outfile.close();
        cout << "Centroids saved to file." << endl;
    } else {
        cout << "Failed to open file." << endl;
    }

    return 0;
}