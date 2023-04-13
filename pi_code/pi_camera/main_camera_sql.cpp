#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <mysql.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    // 连接MySQL数据库
    MYSQL* conn = mysql_init(NULL);
    if (!conn) {
        cout << "Failed to initialize MySQL." << endl;
        return -1;
    }
    string db_host = "YOUR_HOST";
    string db_user = "YOUR_USERNAME";
    string db_password = "YOUR_PASSWORD";
    string db_name = "YOUR_DATABASE_NAME";
    if (!mysql_real_connect(conn, db_host.c_str(), db_user.c_str(), db_password.c_str(), db_name.c_str(), 0, NULL, 0)) {
        cout << "Failed to connect MySQL." << endl;
        return -1;
    }

    // 打开网络相机
    string url = "YOUR_CAMERA_URL"; // 替换为您的相机URL
    VideoCapture cap(url);
    if (!cap.isOpened()) {
        cout << "Failed to open camera." << endl;
        return -1;
    }

    // 循环读取每一帧图像并计算质心
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

        // 提取质心坐标
        vector<Point2f> centroids(contours.size());
        for (size_t i = 0; i < contours.size(); i++) {
            centroids[i] = Point2f(static_cast<float>(moments[i].m10 / moments[i].m00),
                                    static_cast<float>(moments[i].m01 / moments[i].m00));
        }

        // 将质心坐标上传到MySQL数据库中
        MYSQL_RES* res;
        MYSQL_FIELD* field;
        MYSQL_ROW row;
        string query = "INSERT INTO centroids(x, y) VALUES ";
        for (size_t i = 0; i < centroids.size(); i++) {
            query += "(" + to_string(centroids[i].x) + "," + to_string(centroids[i].y) + ")";
            if (i != centroids.size() - 1) {
                query += ",";
            }
        }
        if (mysql_query(conn, query.c_str())) {
            cout << "Failed to insert data into database." << endl;
        } else {
            cout << "Data inserted into database." << endl;
        }

        // 在图像上显示质心
        for (size_t i = 0; i < centroids.size(); i++) {
            circle(frame, centroids[i], 5, Scalar(0, 0, 255), -1);
        }

        // 显示图像
        imshow("Frame", frame);

        // 按下ESC键退出循环
        if (waitKey(1) == 27) {
            break;
        }
    }

    // 关闭相机和MySQL数据库连接
    cap.release();
    mysql_close(conn);
    return 0;
}
