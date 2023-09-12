import os.path
import numpy as np
import cv2
from src.chess.clickShape import *
from src.chess.variables import camera

def drawDots(image, show = False):
    global warped_image, new_points, InvM
    
    # CREATE ALL DOTS
    blankImage = np.zeros(warped_image.shape, np.uint8)                              # create blank image
    
    h, w, _ = blankImage.shape
    rows2 = np.linspace(0, w-1, 9)
    cols2 = np.linspace(0, h-1, 9)

    # draw corners using the grid size 
    cornerPosOnWarpedImage = np.zeros(warped_image.shape, np.uint8)
    cornerPosOnWarpedImage = cv2.cvtColor(cornerPosOnWarpedImage, cv2.COLOR_BGR2GRAY)
    for x in rows2:
        for y in cols2:
            cv2.circle(cornerPosOnWarpedImage, (int(x), int(y)), 2, 255, -1)

    cornerPosOnWarpedImage = np.uint8(cornerPosOnWarpedImage)
    ret, labels, stats, centroids = cv2.connectedComponentsWithStats(cornerPosOnWarpedImage)       # find centre of each dot 
    crit = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 0.001) 
    cornerPosOnWarped = cv2.cornerSubPix(cornerPosOnWarpedImage, np.float32(centroids),(5,5),(-1,-1), crit)  # detecting the corner location in subpixels
    
    # draw all corners on the original image again
    cornersPosOnOriginal = calculate_newPoints(cornerPosOnWarped, InvM)
    if show:
        cornersPosOnOriginalImage = image
        r = 255
        b = 10
        for i in range(len(cornersPosOnOriginal)):
            r = r - 2
            b = b + 2
            cv2.circle(cornersPosOnOriginalImage, tuple(cornersPosOnOriginal[i].astype('int64')), radius=0, color=(b, 0, r), thickness=5)

        cv2.imshow("dots on original", cornersPosOnOriginalImage)
        cv2.imwrite("./MachineLearning/results/identified_corners.png", cornersPosOnOriginalImage)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    return cornersPosOnOriginal

def runWarp(pts, img, show = False):

    global warped_image, new_points, InvM

    # import an image of chessboard and pieces
    unwarped_image = img

    # draw a few points to test accuracy
    points = np.array([[250, 250], [400, 300], [500, 200]])
    for i in range(len(points)):
        cv2.circle(unwarped_image, tuple(points[i].astype('int64')), radius=0, color=(255, 0, 0), thickness=10)

    # get top left, top right, bottom right, bottom left from chessboard mask
    cornerPoints = np.array([pts[0], pts[1], pts[2], pts[3]], dtype='float32')

    # find base and height of the rectangle given by the corners
    base = np.linalg.norm(cornerPoints[1] - cornerPoints[0] )
    height = base/12*11

    # define new corner points from base and height of the rectangle
    new_cornerPoints = np.array([[0, 0], [int(base), 0], [int(base), int(height)], [0, int(height)]], dtype='float32')

    # calculate matrix to transform the perspective of the image
    M = cv2.getPerspectiveTransform(cornerPoints, new_cornerPoints)
    InvM = cv2.getPerspectiveTransform(new_cornerPoints, cornerPoints)
    warped_image = cv2.warpPerspective(unwarped_image, M, (int(base), int(height)))

    new_points = calculate_newPoints(points, M)

    if show:
        # paint points in red (should appear over the blue) to test
        for i in range(len(new_points)):
            cv2.circle(warped_image, tuple(new_points[i].astype('int64')), radius=0, color=(0, 0, 255), thickness=5)

        cv2.imwrite('./MachineLearning/results/new_distorted.png', warped_image)
        cv2.imwrite('./MachineLearning/results/undistorted_withPoints.png', unwarped_image)
        cv2.imshow("warp", warped_image)
        cv2.imshow("unwarp", unwarped_image)

        print("Images saved.\n")

#Function to get data points in the new perspective from points in the image
def calculate_newPoints(points, M):
        new_points = np.einsum('kl, ...l->...k', M,  np.concatenate([points, np.broadcast_to(1, (*points.shape[:-1], 1)) ], axis = -1) )
        return new_points[...,:2] / new_points[...,2][...,None]

def take_image(path_to_im):
    vid = cv2.VideoCapture(camera, cv2.CAP_DSHOW)

    if not (vid.isOpened()):
        print("Could not open video device")

    else:
        while True:
            # Capture the video frame by frame
            ret, frame = vid.read()

            # Display the resulting frame
            cv2.imshow('frame', frame)

            # take image by pressing 'q'
            if cv2.waitKey(1) == ord('q'):
                cv2.imwrite(path_to_im, frame)
                if os.path.exists(path_to_im):
                    print("Image written to {}", path_to_im)
                    break
                else:
                    print("Image not saving")

        # After the loop release the cap object
        vid.release()
        cv2.destroyAllWindows()
        return frame

def create_mask(path):
    if not os.path.exists(path):
        print("Chessboard Image not found, taking picture")
        take_image(path)
    else:
        print("Chessboard Image found")
        while(1):
            value = input("Retake Chessboard Image? Y/N\n")
            if value == "Y" or value == "y":
                take_image(path)
                break
            elif value == "N" or value == "n":
                print("Using displayed image")
                break
            else:
                print("Try again.")
    
    chessboardIm = cv2.imread(path)
    chessboardIm = cv2.resize(chessboardIm, (640, 480))
    polygon, pts = get_outside_corners(path)

    d = 30
    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2*d+1, 2*d+1), (d, d))
    mask = cv2.dilate(polygon, element)

    mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
    masked_original = cv2.bitwise_and(chessboardIm, mask)

    cv2.waitKey(0)
    cv2.imwrite("./MachineLearning/results/mask.jpg", mask)
    cv2.imwrite("./MachineLearning/results/masked_original.jpg", masked_original)
    cv2.destroyAllWindows()
    
    return mask, masked_original, pts

def get_outside_corners(path):
    pd = PolygonDrawer("Polygon")
    image = pd.run(path)
    cv2.imwrite("./MachineLearning/results/polygon.png", image)
    print("Polygon = %s" % pd.points)
    return image, pd.points


def remove_background(mask, img_name = ""):
    image = cv2.imread(img_name)
    masked = cv2.bitwise_and(image, mask)
    return masked

def remove_background(mask, img = np.array):
    masked = cv2.bitwise_and(img, mask)
    return masked

def draw_grid(img, grid_shape, color=(0, 255, 0), thickness=1):
    h, w, _ = img.shape
    rows, cols = grid_shape
    dy, dx = h / rows, w / cols

    # draw vertical lines
    for x in np.linspace(start=0, stop=w-1, num=9):
        x = int(round(x))
        cv2.line(img, (x, 0), (x, h), color=color, thickness=thickness)

    # draw horizontal lines
    for y in np.linspace(start= 0, stop=h-1, num=9):
        y = int(round(y))
        cv2.line(img, (0, y), (w, y), color=color, thickness=thickness)

    return img, dx, dy