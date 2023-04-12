#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
  Mat img = imread("/Users/youkipepper/Desktop/pi/cv_test/test.jpg");
  imshow("test", img);

  waitKey(0);

  return 0;
}
