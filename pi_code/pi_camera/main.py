import cv2
import numpy as np

# 读取图像
G3 = cv2.imread("/home/youki/Desktop/pi/pi_code/pi_camera/test.jpg")

# 将图像转换为二值化图像
G4 = cv2.cvtColor(G3, cv2.COLOR_BGR2GRAY)
_, G4 = cv2.threshold(G4, 0, 255, cv2.THRESH_BINARY)

# 计算区域属性
contours, _ = cv2.findContours(G4, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
moments = [cv2.moments(contour, False) for contour in contours]

# 提取质心坐标
centroids = [np.array([moment["m10"]/moment["m00"], moment["m01"]/moment["m00"]]) for moment in moments]

# 将质心坐标存储到文件中
with open("centroids.txt", "w") as outfile:
    for centroid in centroids:
        outfile.write(str(centroid[0]) + " " + str(centroid[1]) + "\n")
    print("Centroids saved to file.")
