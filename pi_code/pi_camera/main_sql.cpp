#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // 连接到MySQL数据库pkg
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* conn;
    sql::Statement* stmt;
    driver = sql::mysql::get_mysql_driver_instance();
    conn = driver->connect("tcp://127.0.0.1:3306", "root", "password");
    conn->setSchema("database_name");

    // 读取图像
    Mat G3 = imread("/home/youki/Desktop/pi/pi_code/pi_camera/test.jpg");

    // 将图像转换为二值化图像
    Mat G4;
    cvtColor(G3, G4, COLOR_BGR2GRAY);
    threshold(G4, G4, 0, 255, THRESH_BINARY);

    // 计算区域属性
    vector<vector<Point>> contours;
    findContours(G4, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
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

    // 将质心坐标插入到MySQL数据库中
    stmt = conn->createStatement();
    for (size_t i = 0; i < centroids.size(); i++) {
        float x = centroids[i].x;
        float y = centroids[i].y;
        stringstream ss;
        ss << "INSERT INTO centroid_data(x, y) VALUES ('" << x << "','" << y << "')";
        stmt->execute(ss.str());
    }
    delete stmt;
    delete conn;

    return 0;
}
