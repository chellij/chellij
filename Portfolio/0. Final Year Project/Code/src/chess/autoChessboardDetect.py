# This Python file uses the following encoding: utf-8
import cv2
import numpy as np
import math
from src.chess.variables import camera

webcam = False

"""
This code performs chessboard corner detection on an input image. 
It can either read the input image from a webcam or from a file. 
The code converts the input image to grayscale and then uses the cv2.findChessboardCorners function to find the corners of a chessboard pattern in the image. 
If the corners are found, the code reshapes the array of corner coordinates and loops over the rows and columns of corners. 
For each corner, it gets the x and y coordinates and draws a red circle at that location on the input image. 
The code then shows the input image with the corners marked. 
It waits for a key press and exits if the 'q' key is pressed.
"""

def chessBoardCorners(webcam=True, image_str=None):
    # Set up video capture
    vid = cv2.VideoCapture(camera, cv2.CAP_DSHOW)

    # Set number of rows and columns in chessboard
    rows = 7
    cols = 7
    
    while True:
        # Read image from webcam or file
        if webcam:
            _, img = vid.read() 
        else:
            img = cv2.imread(image_str)
        
        # Convert image to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        
        # Find chessboard corners in image
        ret, corners = cv2.findChessboardCorners(gray, (rows, cols), None)
        
        # If corners were found
        if ret:
            # Reshape corners array
            corners = corners.reshape(rows, cols, 2)
            
            # Loop over rows and columns of corners
            for row in range(rows):
                for col in range(cols):
                    # Get corner coordinates
                    x = int(corners[row][col][0])
                    y = int(corners[row][col][1])
                    coord = [x, y]
                    
                    # Draw red circle at corner coordinates
                    cv2.circle(img, coord, 4, (0, 0, 255), -1)

        # Show image with corners marked
        cv2.imshow("im", img)
        
        # Wait for key press
        if webcam:
            key = cv2.waitKey(1)
            if key == ord('q'):
                break
        else:
            key = cv2.waitKey(0)
            if key == ord('q'):
                break
            
    # Close all windows
    cv2.destroyAllWindows()

"""
This code performs corner detection on an input image. 
It reads the input image and resizes it to a fixed size. 
Then it converts the image to grayscale and performs edge detection using the Canny algorithm.
The code then finds contours in the edge-detected image and approximates them as polygons. 
It checks if the polygons are rectangular and convex and adds them to a list of polygons. 
The code then finds the polygon with the maximum area and creates a mask by filling this polygon with white color on a black background. 
Finally, it applies this mask to the original image and shows the masked image.
"""

def cornerDetect(input):
    image_str = f"./MachineLearning/data/images/blank_chessboard.jpg"
    if input == "webcam":
        vid = cv2.VideoCapture(camera, cv2.CAP_DSHOW)
        while(True):
            _, img = vid.read() 
            cv2.imshow("Webcam", img)
            key = cv2.waitKey(1)
            if key == ord(" "):
                cv2.destroyAllWindows()
                break
    elif input == "image": 
        # Read input image
        img = cv2.imread(image_str)

    # Resize image to 640x640
    img = cv2.resize(img, (640, 640))
    
    # Convert image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Perform edge detection
    edges = cv2.Canny(gray, 150, 200)
    # Show edges
    cv2.imshow("edges", edges)
    
    # Find contours in the image
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    
    polygons = [] 
    for cnt in contours: 
        # Set approximation accuracy
        epsilon = 0.01 * cv2.arcLength(cnt, True)
        # Approximate polygon
        poly = cv2.approxPolyDP(cnt, epsilon, True)
        # Calculate area of polygon
        area = cv2.contourArea(poly)
        # Check if polygon is rectangular and convex
        if cv2.isContourConvex(poly) and area > 300:
            polygons.append(poly)

    max_area = 0 # maximum distance from origin 
    max_poly = None # maximum polygon 
    for poly in polygons: 
        area = cv2.contourArea(poly)
        # Update maximum distance and polygon
        if area > max_area:
            max_area = area 
            max_poly = poly 
            
    # Create a black image with same shape as input image
    corner_img = np.zeros_like(img)
    # Draw red contour for each corner
    cv2.fillPoly(corner_img, [max_poly], (255, 255, 255))

    mask = cv2.bitwise_and(img, corner_img)
    cv2.imshow("m", mask)
    image_str = f"./MachineLearning/results/masked_original.jpg"
    cv2.imwrite(image_str, mask)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return max_poly, mask



"""
This code loads an image and applies edge detection to it using the Canny edge detector. 
It then applies the Hough transform to detect lines in the edge-detected image. 
The code then groups similar lines together based on their rho and theta values. 
Two lines are considered similar if their rho values differ by less than rho_threshold and their theta values differ by less than theta_threshold. 
The average rho and theta values are computed for each group of similar lines. 
These average lines are drawn on another copy of the original image using green color.

The code then finds the minimum and maximum rho and theta values among all average lines. 
It filters out any average lines whose rho or theta value is equal to either the minimum or maximum value. 
The remaining filtered lines are drawn on another copy of the original image using blue color.

Finally, the code computes the intersections of the filtered lines by solving a system of linear equations for each pair of lines. 
The resulting intersections are drawn on another copy of the original image using red color.
"""

def lineDetect(img):
    # Load the image and apply edge detection
    img = cv2.resize(img, (640, 640))
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 130, 180)

    # Apply Hough transform to detect lines in the image
    lines = cv2.HoughLines(edges, 1, np.pi/180, 120)
    
    # Define a threshold for grouping similar lines
    rho_threshold = 15
    theta_threshold = np.pi/150
    # Group similar lines and compute their average
    groups = []
    for line in lines:
        rho, theta = line[0]
        found_group = False
        for group in groups:
            avg_rho, avg_theta = group[0]
            if abs(rho - avg_rho) < rho_threshold and abs(theta - avg_theta) < theta_threshold:
                group.append((rho, theta))
                found_group = True
                break
        if not found_group:
            groups.append([(rho, theta)])

    avg_lines = []
    for group in groups:
        sum_rho = sum(rho for rho, _ in group)
        sum_theta = sum(theta for _, theta in group)
        count = len(group)
        avg_rho = sum_rho / count
        avg_theta = sum_theta / count
        avg_lines.append((avg_rho, avg_theta))

    # Initialize variables to store the lines with the highest and lowest y-intercepts
    max_y_intercept_pos = float('-inf')
    min_y_intercept_pos = float('inf')
    max_y_intercept_neg = float('-inf')
    min_y_intercept_neg = float('inf')
    line_max_y_intercept_pos = None
    line_min_y_intercept_pos = None
    line_max_y_intercept_neg = None
    line_min_y_intercept_neg = None
    
    filtered_lines = []
    if len(avg_lines) == 18:
        filtered_lines = avg_lines
    elif len(avg_lines) == 22:
        # Find the lines with the highest and lowest y-intercepts in each group
        for line in avg_lines:
            rho, theta = line
            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a * rho
            y0 = b * rho
            x1 = int(x0 + 1000 * (-b))
            y1 = int(y0 + 1000 * (a))
            x2 = int(x0 - 1000 * (-b))
            y2 = int(y0 - 1000 * (a))
            
            # Calculate the gradient of the line
            if x2 - x1 == 0:
                continue
            m = (y2 - y1) / (x2 - x1)
            
            # Calculate the y-intercept of the line
            c = y1 - m * x1
            
            # Update the lines with the highest and lowest y-intercepts in each group
            if m > 0:
                if c > max_y_intercept_pos:
                    max_y_intercept_pos = c
                    line_max_y_intercept_pos = line
                if c < min_y_intercept_pos:
                    min_y_intercept_pos = c
                    line_min_y_intercept_pos = line
            else:
                if c > max_y_intercept_neg:
                    max_y_intercept_neg = c
                    line_max_y_intercept_neg = line
                if c < min_y_intercept_neg:
                    min_y_intercept_neg = c
                    line_min_y_intercept_neg = line
            
        for line in avg_lines:
            if not (line == line_max_y_intercept_pos or line == line_min_y_intercept_pos or line == line_max_y_intercept_neg or line == line_min_y_intercept_neg):
                filtered_lines.append(line)

    # Find the intersections between any lines
    intersections = []
    filtered_lines_im = img.copy()
    height, width = img.shape[:2]
    for i in range(len(filtered_lines)):
        drawLine(filtered_lines[i], filtered_lines_im, (0, 0, 255))
        for j in range(i+1, len(filtered_lines)):
            rho1, theta1 = filtered_lines[i]
            rho2, theta2 = filtered_lines[j]
            A = np.array([[np.cos(theta1), np.sin(theta1)], [np.cos(theta2), np.sin(theta2)]])
            b = np.array([rho1, rho2])
            if abs(np.linalg.det(A)) > 1e-6:
                x0, y0 = np.linalg.solve(A, b)
                if 0 <= x0 < width and 0 <= y0 < height:
                    intersections.append((x0, y0))
    cv2.imshow("filtered lines", filtered_lines_im)
    
    # Calculate the outermost coners of these intersections
    edge_corners = findOutermostCorners(intersections, img.shape)
    # Draw the intersections on a black image
    corner_img = img.copy()
    for x, y in intersections:
        if (int(x), int(y)) == edge_corners[0] or (int(x), int(y)) == edge_corners[1] or (int(x), int(y)) == edge_corners[2] or (int(x), int(y)) == edge_corners[3]:
            cv2.circle(corner_img, (int(x), int(y)), 3, (0, 255, 0), -1)
        else:
            cv2.circle(corner_img, (int(x), int(y)), 3, (0, 0, 255), -1)

    # All the changes made in the input image are finally written on a new image houghlines.jpg
    cv2.imshow("corners", corner_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return edge_corners


"""
This code defines a function drawLine that takes as input a line, an image, and a color.
The line is represented by its polar coordinates rho and theta. 
The code calculates the cosine and sine of theta and uses them to calculate the x and y coordinates of a point on the line. 
It then calculates the coordinates of two points on the line by adding and subtracting multiples of the sine and cosine values from the x and y coordinates of the point on the line. 
Finally, it uses the cv2.line function to draw a line between these two points on the input image using the specified color.
"""
def drawLine(line, img, colour):
    # Unpack line parameters
    rho, theta = line
    # Calculate cosine and sine of theta
    a = np.cos(theta)
    b = np.sin(theta)
    # Calculate x and y coordinates of point on line
    x0 = a * rho
    y0 = b * rho
    # Calculate coordinates of two points on line
    x1 = int(x0 + 1000 * (-b))
    y1 = int(y0 + 1000 * a)
    x2 = int(x0 - 1000 * (-b))
    y2 = int(y0 - 1000 * a)
    # Draw line on image
    cv2.line(img, (x1, y1), (x2, y2), colour, 1)
   
def findOutermostCorners(points, img_shape):
    # Initialize edge variables
    top = None
    bottom = None
    left = None
    right = None
    
    # Get image dimensions
    height, width = img_shape[:2]
    
    # Loop over intersection coordinates
    for x, y in points:
        if 0 <= x < width and 0 <= y < height:
            # Calculate distances to edges of image
            dist_to_top = y
            dist_to_bottom = height - y
            dist_to_left = x
            dist_to_right = width - x
            
            # Update edge variables
            if top is None or dist_to_top < top[1]:
                top = (int(x), int(y))
            if bottom is None or dist_to_bottom < height - bottom[1]:
                bottom =(int(x), int(y))
            if left is None or dist_to_left < left[0]:
                left = (int(x), int(y))
            if right is None or dist_to_right < width - right[0]:
                right = (int(x), int(y))
    
    # Return closest intersection points
    fourPts = sort_points([top, bottom, left, right])
    return fourPts

"""
This function sorts the input list of 4 points (x,y) first based on their y-coordinates to split them into top and bottom halves. 
Then it sorts the top and bottom halves separately based on their x-coordinates. 
Finally, it rearranges the sorted points to match the desired order of top-left, top-right, bottom-right, and bottom-left.
"""

def sort_points(points):
    # Calculate the center of the points
    center_x = sum(p[0] for p in points) / len(points)
    center_y = sum(p[1] for p in points) / len(points)
    
    # Calculate the angle formed by the line connecting the center of the points and each point
    angles = [math.atan2(p[1] - center_y, p[0] - center_x) for p in points]
    
    # Sort the points based on their angles
    sorted_points = [point for _, point in sorted(zip(angles, points))]
    
    # Rearrange the sorted points to match the desired order
    top_left, top_right, bottom_right, bottom_left = sorted_points
    
    sorted_point = [top_left, top_right, bottom_right, bottom_left]
    return sorted_points

"""
This function takes as input a Hough line and the shape of an image. 
It unpacks the rho and theta parameters of the line and calculates its endpoints. 
It then uses the cv2.clipLine() function to clip the line to the image rectangle. 
The first argument to this function is a tuple specifying the size of the image rectangle, and the second and third arguments are the coordinates of the line endpoints. 
The cv2.clipLine() function returns a boolean value indicating whether the line intersects the image rectangle, as well as the coordinates of the clipped line. 
The function returns this boolean value to indicate whether or not the input line passes through the image space.
"""

def line_intersects_image(line, img_shape):
    # Unpack line parameters
    rho, theta = line[0]
    
    # Calculate line endpoints
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a * rho
    y0 = b * rho
    pt1 = (int(x0 + 1000 * (-b)), int(y0 + 1000 * a))
    pt2 = (int(x0 - 1000 * (-b)), int(y0 - 1000 * a))
    
    # Clip line to image rectangle
    ret, pt1, pt2 = cv2.clipLine((0, 0, img_shape[1], img_shape[0]), pt1, pt2)
    
    # Return whether line intersects image
    return ret

if __name__ == "__main__":
    _, mask = cornerDetect("webcam")
    pts = lineDetect(mask)
    chessBoardCorners()