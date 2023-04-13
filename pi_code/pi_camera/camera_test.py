
import cv2 as cv
import numpy as np
import math
import os

# 读取摄像机
path = "rtsp://192.168.137.135/11"
cap = cv.VideoCapture(path)
i = 0
while True:
    ret, frame = cap.read()
    cv.imshow("frame", frame)
    i = i + 1
    print("帧数", i)
    c = cv.waitKey(1)
    if c == 27:  # ESC
        break
cv.waitKey(0)
cv.destroyAllWindows()


