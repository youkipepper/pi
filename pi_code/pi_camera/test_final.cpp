#include <iostream>
#include <mysql/mysql.h>
#include <opencv2/opencv.hpp>

int main() {
  // MySQL connection
  MYSQL *mysql_conn;
  mysql_conn = mysql_init(NULL);
  if (mysql_conn == NULL) {
    std::cout << "MySQL initialization failed." << std::endl;
    return 1;
  }

  mysql_conn = mysql_real_connect(mysql_conn, "123.60.191.124", "root", "Hzk2022@", "bridge", 0, NULL, 0);
  if (mysql_conn == NULL) {
    std::cout << "Could not connect to MySQL server. " << mysql_error(mysql_conn) << std::endl;
    return 1;
  }
  
  std::cout << "Connected to the MySQL server successfully." << std::endl;

  // OpenCV 
  cv::Mat image = cv::imread("/home/youki/Desktop/pi/pi_code/pi_camera/test.jpg");
  if (image.empty()) {
    std::cout << "Could not read the image file." << std::endl;
    return 1;
  }
  std::cout << "Image loaded successfully." << std::endl;
  
  return 0;
}

