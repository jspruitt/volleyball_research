from imutils.video import VideoStream
import argparse
import datetime
import imutils
import time
import cv2
import numpy as np

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--video", help="path to the video file")
    ap.add_argument("-a", "--min-area", type=int, default=500, help="minimum area size")
    args = vars(ap.parse_args())

    if args.get("video", None) is None:
        vs = VideoStream(src=0).start()
        time.sleep(2.0)

    else:
        vs = cv2.VideoCapture(args["video"])

    firstFrame = None
    while True:
        frame = vs.read()
        frame = frame if args.get("video", None) is None else frame[1]
        #end of video
        if frame is None:
            break
        frame = imutils.resize(frame, width=500)
        gray = white_only(frame)
        gray = cv2.cvtColor(gray, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (21, 21), 0)
        if firstFrame is None:
            firstFrame = gray
            continue
        frameDelta = cv2.absdiff(firstFrame, gray)
        thresh = cv2.threshold(frameDelta, 25, 255, cv2.THRESH_BINARY)[1]
        frame = size(thresh, frame)

        cv2.imshow("Thresh", frame)
        key = cv2.waitKey(1) & 0xFF

        # if the `q` key is pressed, break from the lop
        if key == ord("q"):
            break

    # cleanup the camera and close any open windows
    vs.stop() if args.get("video", None) is None else vs.release()
    cv2.destroyAllWindows()

def size(thresh, frame):
    minVolleyball = 75
    maxVolleyball = 150
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if minVolleyball < area < maxVolleyball:
            ellipse = cv2.fitEllipse(cnt)
            center, axes, startAngle = ellipse
            if 0.5 < axes[0]/axes[1]< 2:
                cv2.ellipse(frame, ellipse, (0, 255, 0), 2)
    return frame

def white_only(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2YCrCb)
    lower_white = np.array([150,0,0], dtype=np.uint8)
    upper_white = np.array([255,255,255], dtype=np.uint8)
    mask = cv2.inRange(hsv, lower_white, upper_white)
    res = cv2.bitwise_and(frame,frame, mask= mask)
    return res

def houghCircles(frame):
    circles = cv2.HoughCircles(frame, cv2.HOUGH_GRADIENT, 1, 1)
                               #param1=200, param2=100, minRadius=0, maxRadius=0)
    if circles is not None:
        for i in circles[0, :]:
            cv2.circle(frame, (i[0], i[1]), i[2], (0, 255, 0), 2)
            # draw the center of the circle
            cv2.circle(frame, (i[0], i[1]), 2, (0, 0, 255), 3)
    return frame
main()