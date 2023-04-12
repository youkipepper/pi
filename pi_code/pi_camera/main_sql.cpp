#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <mysql.h>

using namespace std;
using namespace cv;

#define HOST "localhost"
#define USER "root"
#define PWD "password"
#define DB_NAME "mydatabase"

int main(int argc, char** argv) {
    // 创建MySQL连接
    MYSQL* conn = mysql_init(0);
    conn = mysql_real_connect(conn, HOST, USER, PWD, DB_NAME, 0, NULL, 0);
    if (conn) {
        cout << "MySQL connection established." << endl;
    } else {
        cout << "Failed to connect to MySQL." << endl;
        return -1;
    }

    // 打开相机
    VideoCapture cap(0);
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

        // 将图像转换为二值图像
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        threshold(gray, gray, 0, 255, THRESH_BINARY);

        // 查找轮廓
        vector<vector<Point>> contours;
        findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        // 计算质心并将结果上传到MySQL
        for (size_t i = 0; i < contours.size(); i++) {
            Moments m = cv::moments(contours[i], false);
            Point2f centroid(m.m10 / m.m00, m.m01 / m.m00);
            string query = "INSERT INTO centroids (x, y) VALUES (" + to_string(centroid.x) + ", " + to_string(centroid.y) + ")";
            mysql_query(conn, query.c_str());
        }

        // 显示图像
        imshow("Frame", frame);

        // 按下ESC键退出循环
        if (waitKey(1) == 27) {
            break;
        }
    }

    // 关闭相机和MySQL连接
    cap.release();
    mysql_close(conn);
    return 0;
}
