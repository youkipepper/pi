#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    string path = "rstp://192.168.1.154:8554/stream1";
    VideoCapture cap(path);

    if (!cap.isOpened()) {
        cout << "Failed to open camera." << endl;
        return -1;
    }

    int i = 0;
    while (true) {
        Mat frame;
        if (!cap.read(frame)) {
            cout << "Failed to read frame." << endl;
            break;
        }
        imshow("frame", frame);
        i++;

        char c = waitKey(1);
        if (c == 27) { // ESC
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
