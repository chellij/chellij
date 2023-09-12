import cv2
from roboflow import Roboflow

import os.path

from src.chess.displayFunctions import *
from src.chess.variables import *
from src.chess.userChessboardDetect import *
import src.chess.setupChessBoard as setupChess

import cv2
import numpy as np

from ultralytics import YOLO


def exampleVideo():
    model = loadDataset()
    for i in range(30):
        img = cv2.imread(f"./MachineLearning/tests/0/{i}.jpg")
        img = cv2.resize(img, (640, 480))
        img2 = cv2.imread(f"./MachineLearning/src/yolov5/runs/detect/exp11/{i}.jpg")
        img2 = cv2.resize(img2, (640, 480))
        #pred = model.predict(img, confidence=50).json()
        #draw_predictions(img2, pred)
        cv2.imshow("i", np.hstack([img, img2]))
        cv2.waitKey(300)
    cv2.destroyAllWindows()


###########################################################
"""
retrieveSquareLocation takes as input a set of test coordinates and a dictionary of chess squares. 
The code loops over the rows and columns of the chessboard and over the items in the chessSquares dictionary. 
For each item, it checks if the value is a dictionary. 
If it is, the code loops over the items in this inner dictionary and checks if the key is 'coordinates'. 
If it is, the code checks if the coordinates match the testCoordinates input. 
If they do, the code gets the position of the square from the 'pos' key of the inner dictionary and returns success along with this position. 
If no matching square is found, the code returns failure along with a default position.
"""
def retrieveSquareLocation(testCoordinates, chessSquares):
    # Loop over rows and columns of chessboard
    for x in range(1, 9):
        for y in range(1, 9):
            # Loop over items in chessSquares dictionary
            for key, val in chessSquares.items():
                # Check if value is a dictionary
                if type(val) is dict:
                    # Loop over items in inner dictionary
                    for k, v in val.items():
                        # Check if key is 'coordinates'
                        if k == 'coordinates':
                            # Check if coordinates match testCoordinates
                            if np.array_equal(v, testCoordinates):
                                # Get position of square
                                [x, y] = val['pos']
                                # Return success and position
                                return True, [y-1, x-1]

    # Return failure and default position
    return False, [8, 8]

#########################################################

"""
This code defines a function isOverlapping that takes as input a testBox array and two corner points c1 and c2.
The code reshapes the testBox array and creates a blank image of size CANVAS_SIZE. 
It then uses the cv2.fillPoly function to fill a polygon defined by the reshaped testBox array with white color on the blank image. 
The code then creates another blank image and uses the cv2.rectangle function to draw a white rectangle with corners c1 and c2 on this image. 
It performs a bitwise AND operation between the two images to find the overlapping region between the polygon and the rectangle. 
Finally, it returns the sum of white pixels in the overlap image.
"""

def isOverlapping(testBox, c1, c2, im):
    # Reshape testBox array
    pts = testBox.reshape(-1, 1, 2)
    
    # Create blank image
    blankImage = np.zeros(CANVAS_SIZE, np.uint8)
    # Fill polygon defined by pts with white color
    sq = cv2.fillPoly(blankImage, [pts], 255)
    
    # Create another blank image
    blankImage2 = np.zeros(CANVAS_SIZE, np.uint8)
    # Draw white rectangle with corners c1 and c2
    bb = cv2.rectangle(blankImage2, c1, c2, 255, -1)
    
    # Perform bitwise AND between two images
    overlap = cv2.bitwise_and(bb, sq)

    # Return sum of white pixels in overlap image
    return np.sum(overlap == 255)

#########################################################

def compareBoard(board1, board2, type):
    changes = ""
    errNo = 0
    if not board1 == board2:
        for j in range(8):
            for i in range(8):
                if not board1[i][j] == board2[i][j]:
                    thisSquare = findKey(j+1, i+1, chessSquares)
                    if type == "compare":
                        if not board2[i][j] == " ": 
                            if not board1[i][j] == " ":
                                    changes = changes + (f"{board2[i][j]}x{thisSquare}\n")
                            else:
                                changes = changes + (f"{board2[i][j]}{thisSquare}\n")
                    elif type == "err":
                        changes = changes + (f"{thisSquare}: is {board1[i][j]} should be {board2[i][j]}\n")
                        errNo = errNo + 1

    if type == "compare":
        return changes
    elif type == "err":
        return changes, errNo

def compareBoard6(board1, board2):
    changes = ""
    errNo = 0
    board2 = np.char.upper(board2)
    if not (board1 == board2).all():
        for j in range(8):
            for i in range(8):
                if not board1[i][j] == board2[i][j]:
                    thisSquare = findKey(j+1, i+1, chessSquares)
                    changes = changes + (f"{thisSquare}: is {board1[i][j]} should be {board2[i][j]}\n")
                    errNo = errNo + 1

    return changes, errNo

    
def findKey(x, y, chessSquares): 
    for key, val in chessSquares.items(): 
        if type(val) is dict:
            for k, v in val.items():
                if k == 'pos' and v == [x, y]:
                        return key

def loadDataset(V = VERSION_NO):
    rf = Roboflow(api_key=API_KEY)
    project = rf.workspace(WORKSPACE_NAME).project(PROJECT_NAME)
    model = project.version(VERSION_NO).model
    print("Model found from Roboflow")
    return model

def defineSquares(corners, chessSquares):
    squares = np.empty([8, 8, 4, 2], np.int32)

    # load dictionary 
    thisSquare = ''   
    while(1):
        if corners.shape[0] == 82:
            for horiz in range(0, 8):
                for vert in range(0, 8):
                    thisSquare = findKey(horiz+1, vert+1, chessSquares)
                    a = [corners[chessBoard[vert][horiz][0]][0].astype('int32'), corners[chessBoard[vert][horiz][0]][1].astype('int32')]
                    b = [corners[chessBoard[vert][horiz][1]][0].astype('int32'), corners[chessBoard[vert][horiz][1]][1].astype('int32')]
                    c = [corners[chessBoard[vert][horiz][2]][0].astype('int32'), corners[chessBoard[vert][horiz][2]][1].astype('int32')]
                    d = [corners[chessBoard[vert][horiz][3]][0].astype('int32'), corners[chessBoard[vert][horiz][3]][1].astype('int32')]
        
                    pts = np.array([[a[0], a[1]], [b[0], b[1]], [d[0], d[1]], [c[0], c[1]]], np.int32)

                    chessSquares[thisSquare]['coordinates'] = pts
                    squares[vert][horiz][:] = pts[:]
            break
        else:
            print("Redraw the squares", corners.shape[0])
            
            runWarp(pts)
            corners = drawDots()
        
    return squares, chessSquares

def draw_predictions(im, predictions):
    pieces = 0
    for bounding_box in predictions['predictions']:
        pieces = pieces + 1
        x0 = bounding_box['x'] - bounding_box['width'] / 2
        x1 = bounding_box['x'] + bounding_box['width'] / 2
        y0 = bounding_box['y'] - bounding_box['height'] / 2
        y1 = bounding_box['y'] + bounding_box['height'] / 2

        start_point = (int(x0), int(y0))
        end_point = (int(x1), int(y1))

        if bounding_box['class'] == 'p' or bounding_box['class'] == 'P':
            colour = (0, 255, 0)
        elif bounding_box['class'] == 'q' or bounding_box['class'] == 'Q':
            colour = (0, 0, 255)
        elif bounding_box['class'] == 'k' or bounding_box['class'] == 'K':
            colour = (255, 0, 0)
        elif bounding_box['class'] == 'r' or bounding_box['class'] == 'R':
            colour = (255, 255, 0)
        elif bounding_box['class'] == 'b' or bounding_box['class'] == 'B':
            colour = (0, 255, 255)
        elif bounding_box['class'] == 'n' or bounding_box['class'] == 'N':
            colour = (255, 0, 255)

        w = im.shape[0]
        h = im.shape[1]
        scale = 7e-4
        fs = min(w, h) * scale

        cv2.putText(im, bounding_box["class"],(int(x0), int(y0) - 10), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=fs, color=(255, 255, 255), thickness=1)
        cv2.rectangle(im, start_point, end_point, color=colour, thickness=2)

def runMainTask(model, corners, im):
    pred = model.predict("./MachineLearning/results/test.jpg", confidence=50).json()
    
    global chessSquares
    chessSquares = setupChess.chessDict

    chessboardLayout, chessSquares = defineSquares(corners, chessSquares)
    
    board = [[' ' for i in range(8)] for i in range(8)]
    pieces = 0

    im = cv2.imread("./MachineLearning/results/test.jpg")
    for bounding_box in pred['predictions']:
        pieces = pieces + 1
        x0 = int(bounding_box['x'] - bounding_box['width'] / 2)
        x1 = int(bounding_box['x'] + bounding_box['width'] / 2)
        y0 = int((bounding_box['y'] - bounding_box['height'] / 2) + bounding_box['height']/3*2)
        y1 = int(bounding_box['y'] + bounding_box['height'] / 2)

        largestOverlap = 0
        foundSquare = []
        for chessRows in chessboardLayout:
            for chessSquare in chessRows:
                overlap = isOverlapping(chessSquare, (x0, y0), (x1, y1), im)
                if(overlap > largestOverlap):
                    largestOverlap = overlap
                    foundSquare = chessSquare

        found, [x, y] = retrieveSquareLocation(foundSquare, chessSquares)
        
        y0 = int(bounding_box['y'] - bounding_box['height'] / 2)
        start_point = (int(x0), y0)
        end_point = (int(x1), int(y1))
        
        if (bounding_box['class']).isupper():
            colour = white
        else:
            colour = black

        cv2.rectangle(im, start_point, end_point, colour, 2)

        if not (largestOverlap == 0) and found:
            board[x][y] = bounding_box['class']

        w = im.shape[0]
        h = im.shape[1]
        scale = 1e-3
        fs = min(w, h) * scale

        if bounding_box['class'] == 'p' or bounding_box['class'] == 'P': colour = red
        elif bounding_box['class'] == 'q' or bounding_box['class'] == 'Q': colour = green
        elif bounding_box['class'] == 'k' or bounding_box['class'] == 'K': colour = blue
        elif bounding_box['class'] == 'r' or bounding_box['class'] == 'R': colour = cyan
        elif bounding_box['class'] == 'b' or bounding_box['class'] == 'B': colour = magenta
        elif bounding_box['class'] == 'n' or bounding_box['class'] == 'N': colour = yellow

        cv2.putText(im, bounding_box["class"], (int(start_point[0]-10), int(start_point[1])), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=fs, color=colour, thickness=1)
        
    return board, im

# ===============================

def runTest(M = 0):
    M = int(M)
    print(f"starting at model {M}")
    value = input("Show Images Throughout Test? (Y/N)")
    if value == "Y" or value == "y":
        show = True
    elif value == "N" or value == "n":
        show = False

    value = input("Print Errors (Y/N)")
    if value == "Y" or value == "y":
        prnt = True
    elif value == "N" or value == "n":
        prnt = False
    global chessSquares
    chessSquares = setupChess.chessDict

    for m in (1, 3, 8, 12, 15):
        if M > m:
            print(f"Model {m} skipped")
        else:     
            model = YOLO(f'./MachineLearning/trainedmodel_{m}.pt')
            
            model.conf = 0.6  # confidence threshold (0-1)
            model.iou = 0.2  # NMS IoU threshold (0-1)  

            tests = 0
            dirPath = "./MachineLearning/"
            for path in os.listdir(f"{dirPath}tests/"):
                if os.path.isdir(os.path.join(f"{dirPath}tests/", path)):
                    tests += 1

            for test in range(tests):
                img = 0
                folder = f"{dirPath}tests/{test}/"
                for path in os.listdir(folder):
                    if os.path.isfile(os.path.join(folder, path)):
                        img += 1
                chessboardPath = f"{folder}/blank_chessboard.jpg"
                chessboardImg = cv2.imread(chessboardPath)
                chessboardImg = cv2.resize(chessboardImg, (640, 480))
                pts = []
                if test == 0:
                    pts = [(114, 190), (390, 50), (621, 248), (300, 470)]
                elif test == 1:
                    pts = [(75, 207), (320, 42), (573, 188), (326, 438)]
                elif test == 2:
                    pts = [(291, 54), (580, 150), (373, 420), (44, 230)]
                runWarp(pts, chessboardImg)
                corners = drawDots(chessboardImg)
                chessboardLayout, chessSquares = defineSquares(corners, chessSquares)

                errors = 0
                for image in range(img-1):
                    frame = cv2.imread(f"{folder}{image}.jpg")
                    img = cv2.resize(frame, (640, 480))
                    results = model.predict(img, verbose = False)
                                
                    board = [[' ' for i in range(8)] for i in range(8)]
                    for box in results[0].boxes:

                        rect = box.xywh[0]                      # returns x, y, width and height coordinates for the bounding box
                        pieceClass = model.names[int(box.cls)]  # 
                        
                        if not (pieceClass == "Chessboard" or pieceClass == "Hand"):
                            
                            if pieceClass == "Pawn": pieceClass = "P"
                            elif pieceClass == "Rook": pieceClass = "R"
                            elif pieceClass == "Knight": pieceClass = "N"
                            elif pieceClass == "Bishop": pieceClass = "B"
                            elif pieceClass == "Queen": pieceClass = "Q"
                            elif pieceClass == "King": pieceClass = "K"
                            
                            x0 = int(rect[0] - rect[2] / 2)
                            x1 = int(rect[0] + rect[2] / 2)
                            y0 = int(rect[1] - rect[3] / 2)
                            y1 = int(rect[1] + rect[3]*2/3)         # caclulate the x position for a box 1/3 size 
                            y2 = int(rect[1] + rect[3] / 2)

                            largestOverlap = 0
                            foundSquare = []
                            boundingBoxc1 = (x0, y0)
                            boundingBoxc2 = (x1, y1)
                            for chessRows in chessboardLayout:
                                for chessSquare in chessRows:
                                    overlap = isOverlapping(chessSquare, boundingBoxc1, boundingBoxc2, img)
                                    if(overlap > largestOverlap):
                                        largestOverlap = overlap
                                        foundSquare = chessSquare

                            found, [x, y] = retrieveSquareLocation(foundSquare, chessSquares)

                            if pieceClass.isupper():
                                colour = white
                            else:
                                colour = black

                            if not (largestOverlap == 0) and found:
                                board[x][y] = pieceClass

                            if show:    
                                start_point = (x0, y0)
                                end_point = (x1, y1)
                                cv2.rectangle(img, start_point, end_point, colour, 2)

                                w = img.shape[0]
                                h = img.shape[1]
                                scale = 1e-3
                                fs = min(w, h) * scale

                                if pieceClass == 'p' or pieceClass == 'P': colour = red
                                elif pieceClass == 'q' or pieceClass == 'Q': colour = green
                                elif pieceClass == 'k' or pieceClass == 'K': colour = blue
                                elif pieceClass == 'r' or pieceClass == 'R': colour = cyan
                                elif pieceClass == 'b' or pieceClass == 'B': colour = magenta
                                elif pieceClass == 'n' or pieceClass == 'N': colour = yellow
                                cv2.putText(img, pieceClass, (int(start_point[0]-10), int(start_point[1])), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=fs, color=colour, thickness=1)
                        
                    if m == 1 or m == 3 or m == 15:
                        errLog, errNo = compareBoard6(board, steps[image])
                    else:
                        errLog, errNo = compareBoard(board, steps[image], "err")
                    
                    if show:
                        fencode, fen = displayFen(board, pg = False)
                        cv2.imshow("fen", fen)
                        cv2.imshow("img", img)
                    errors = errors+errNo
                    if errLog == "" and prnt:
                        print(f"{folder}{image}.jpg V{m} Testing\nNo Errors :)\nFENCODE: {fencode}\n")
                    elif prnt:
                        print(f"{folder}{image}.jpg V{m} Testing\n{errLog}FENCODE: {fencode}\n")
                    
                    cv2.waitKey(1)

                cv2.destroyAllWindows()
                print(f"Test {test} finished: {errors} errors.")
        
            print(f"Model V{m} testing complete.\n")