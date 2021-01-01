
import numpy as np
import sympy as sp
import cv2
from sympy import *
lowerRange = 160
t = 10
differenceRange = 20


def main():
    img = cv2.imread("volleyball_example.jpg")
    white = find_white(img)
    cv2.imshow("white.png", white)
    cv2.waitKey(0)
    hough = houghtransform(white)
    cv2.imshow("white.png", hough)
    cv2.waitKey(0)

def houghtransform(img):
    #hough transform
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (3, 3), 0)
    edges = cv2.Canny(blurred, 50, 150, apertureSize=3)
    cv2.imshow("gray.png", edges)
    cv2.waitKey(0)

    minVotesThreshold =  150
    lines = cv2.HoughLines(edges, 1, np.pi / 180, minVotesThreshold)
    #refine and remove duplicates



    for l in lines:
        for rho, theta in l:
            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a * rho
            y0 = b * rho
            x1 = int(x0 + 1000 * (-b))
            y1 = int(y0 + 1000 * (a))
            x2 = int(x0 - 1000 * (-b))
            y2 = int(y0 - 1000 * (a))

            cv2.line(img, (x1, y1), (x2, y2), (0, 0, 255), 1)
    return img
def getClosePointsMatrix(line, binaryImage, rgbImage):
    M =
     = Mat::zeros(0, 2, CV_32F);

    for x in range(binaryImage.cols):
        for y in range(binaryImage.rows):
            if binaryImage.at < uchar > (y, x) == GlobalParameters().fgValue:
                distance = line.getDistance(Point2f(x, y))
                if  distance < parameters.distanceThreshold:
                    Mat point = Mat::zeros(1, 2, CV_32F)
                    point.at < float > (0, 0) = x
                    point.at < float > (0, 1) = y
                    M.push_back(point)
    return M
def getLine(line, binaryImage, rgbImage):
    A = getClosePointsMatrix(line, binaryImage, rgbImage);
    X = Mat::zeros(1, 4, CV_32F);
    fitLine(A, X, DIST_L2, 0, 0.01, 0.01);
    Point2f v(X.at < float > (0, 0), X.at < float > (0, 1));
    Point2f p(X.at < float > (0, 2), X.at < float > (0, 3));
    return Line(p, v);
def refineLineParameters(lines, binaryImage, rgbImage):
    for line in lines:
        line= getLine(line, binaryImage, rgbImage)

def find_white(img):
    baw = cv2.cvtColor(img, cv2.COLOR_RGB2YCrCb)
    baw2 = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

    equation = a = np.ndarray((len(baw),len(baw[0])), dtype=int)
    for i in range(len(equation)):
        for j in range(len(equation[i])):
            equation[i][j] = baw[i][j][0]
    mask = np.zeros_like(equation)
    blockSize = 3#2*b+1
    ksize = 3
    eigenvalues = cv2.cornerEigenValsAndVecs(baw2, blockSize, ksize, borderType=cv2.BORDER_DEFAULT)
    for i in range(t, len(mask) - t, 1):
        for j in range(t, len(mask[i]) - t, 1):
            e1, e2 = eigenvalues[i][j][0], eigenvalues[i][j][1]
            if equation[i][j] >= lowerRange and equation[i][j] - equation[i][j - t] > differenceRange and equation[i][j] - equation[i][j + t]: #turn white
                mask[i][j] = 255
            if equation[i][j] >= lowerRange and equation[i][j] - equation[i - t][j] > differenceRange and equation[i][j] - equation[i + t][j] : #turn white
                mask[i][j] = 255
            else:
                mask[i][j] = 0
    imask = mask > 0  # for every part of mask that is greater than 0
    white = np.zeros_like(img, np.uint8)  # copy format of img
    white[imask] = img[imask]  # make white resemble the same color
    return white


def eigenvalue():
    x = symbols("x")
    A = np.array([[3, 4, -1], [-1, -2, 1], [3, 9, 0]])
    I = np.identity(len(A))
    H = A - I * x
    M = sp.Matrix(H)
    equation = M.det()
    final=solve(equation, x)
    return math.abs(final[0]) > 4 * math.abs(final[1])
main()