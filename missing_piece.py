
import numpy as np
import sympy as sp
import cv2
from sympy import *
lowerRange = 160
t = 10
differenceRange = 20


def main():
    img = cv2.imread("missing_piece2.jpg")
    cannyedge(img)
    blackbackground(img)

def cannyedge(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (3, 3), 0)
    edges = cv2.Canny(blurred, 50, 150, apertureSize=3)
    cv2.imshow("gray.png", edges)
    cv2.waitKey(0)


def blackbackground(img):
    tmp = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, alpha = cv2.threshold(tmp, 0, 255, cv2.THRESH_BINARY)
    b, g, r = cv2.split(img)
    rgba = [b, g, r, alpha]
    dst = cv2.merge(rgba, 4)
    cv2.imshow("gray.png", dst)
    cv2.waitKey(0)
main()