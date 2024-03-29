API_KEY = "r79ZHB2zt3wvu5WZWnd7"
PROJECT_NAME = "chess-piece-detection-fyp"
WORKSPACE_NAME = "fyp-pbsgo"
VERSION_NO = 12

camera = 1

dirPath = "C:/Users/User/Documents/GitHub/ChessRecognition/MachineLearning/"

black = (0, 0, 0)
white = (255, 255, 255)

red = (0, 0, 255)
blue = (255, 0, 255)
green = (0, 255, 0)
cyan  = (255, 255, 0)
magenta = (255, 0, 255)
yellow = (0, 255, 255)
sky = (250, 230, 230)
brown = (42, 42, 165)

#ROW 1
a8 = [1, 2, 10, 11]     # 1  (1, 1)
b8 = [2, 3, 11, 12]     # 2  (2, 1)
c8 = [3, 4, 12, 13]     # 3  (3, 1)
d8 = [4, 5, 13, 14]     # 4  (4, 1)
e8 = [5, 6, 14, 15]     # 5  (5, 1)
f8 = [6, 7, 15, 16]     # 6  (6, 1)
g8 = [7, 8, 16, 17]     # 7  (7, 1)
h8 = [8, 9, 17, 18]     # 8  (8, 1)
row1 = [a8, b8, c8, d8, e8, f8, g8, h8]

#ROW 2
a7 = [10, 11, 19, 20]   # 9  (1, 2)
b7 = [11, 12, 20, 21]   # 10 (2, 2)
c7 = [12, 13, 21, 22]   # 11 (3, 2)
d7 = [13, 14, 22, 23]   # 12 (4, 2)
e7 = [14, 15, 23, 24]   # 13 (5, 2)
f7 = [15, 16, 24, 25]   # 14 (6, 2)
g7 = [16, 17, 25, 26]   # 15 (7, 2)
h7 = [17, 18, 26, 27]   # 16 (8, 2)
row2 = [a7, b7, c7, d7, e7, f7, g7, h7]

#ROW 3
a6 = [19, 20, 28, 29]   # 17 (1, 3)
b6 = [20, 21, 29, 30]   # 18 (2, 3)
c6 = [21, 22, 30, 31]   # 19 (3, 3)
d6 = [22, 23, 31, 32]   # 20 (4, 3)
e6 = [23, 24, 32, 33]   # 21 (5, 3)
f6 = [24, 25, 33, 34]   # 22 (6, 3)
g6 = [25, 26, 34, 35]   # 23 (7, 3)
h6 = [26, 27, 35, 36]   # 24 (8, 3)
row3 = [a6, b6, c6, d6, e6, f6, g6, h6]

#ROW 4
a5 = [28, 29, 37, 38]   # 25 (1, 4)
b5 = [29, 30, 38, 39]   # 26 (2, 4)
c5 = [30, 31, 39, 40]   # 27 (3, 4)
d5 = [31, 32, 40, 41]   # 28 (4, 4)
e5 = [32, 33, 41, 42]   # 29 (5, 4)
f5 = [33, 34, 42, 43]   # 30 (6, 4)
g5 = [34, 35, 43, 44]   # 31 (7, 4)
h5 = [35, 36, 44, 45]   # 32 (8, 4)
row4 = [a5, b5, c5, d5, e5, f5, g5, h5]

#ROW 5
a4 = [37, 38, 46, 47]   # 33 (1, 5)
b4 = [38, 39, 47, 48]   # 34 (2, 5)
c4 = [39, 40, 48, 49]   # 35 (3, 5)
d4 = [40, 41, 49, 50]   # 36 (4, 5)
e4 = [41, 42, 50, 51]   # 37 (5, 5)
f4 = [42, 43, 51, 52]   # 38 (6, 5)
g4 = [43, 44, 52, 53]   # 39 (7, 5)
h4 = [44, 45, 53, 54]   # 40 (8, 5)
row5 = [a4, b4, c4, d4, e4, f4, g4, h4]

# ROW 6
a3 = [46, 47, 55, 56]   # 41 (1, 6)
b3 = [47, 48, 56, 57]   # 42 (2, 6)
c3 = [48, 49, 57, 58]   # 43 (3, 6)
d3 = [49, 50, 58, 59]   # 44 (4, 6) 
e3 = [50, 51, 59, 60]   # 45 (5, 6)
f3 = [51, 52, 60, 61]   # 46 (6, 6)
g3 = [52, 53, 61, 62]   # 47 (7, 6)
h3 = [53, 54, 62, 63]   # 48 (8, 6)
row6 = [a3, b3, c3, d3, e3, f3, g3, h3]


# ROW 7
a2 = [55, 56, 64, 65]   # 49 (1, 7)
b2 = [56, 57, 65, 66]   # 50 (2, 7)
c2 = [57, 58, 66, 67]   # 51 (3, 7)
d2 = [58, 59, 67, 68]   # 52 (4, 7)
e2 = [59, 60, 68, 69]   # 53 (5, 7)
f2 = [60, 61, 69, 70]   # 54 (6, 7)
g2 = [61, 62, 70, 71]   # 55 (7, 7)
h2 = [62, 63, 71, 72]   # 56 (8, 7)
row7 = [a2, b2, c2, d2, e2, f2, g2, h2]

# ROW 8
a1 = [64, 65, 73, 74]   # 57 (1, 8)
b1 = [65, 66, 74, 75]   # 58 (2, 8)
c1 = [66, 67, 75, 76]   # 50 (3, 8)
d1 = [67, 68, 76, 77]   # 60 (4, 8)
e1 = [68, 69, 77, 78]   # 61 (5, 8)
f1 = [69, 70, 78, 79]   # 62 (6, 8)
g1 = [70, 71, 79, 80]   # 63 (7, 8) 
h1 = [71, 72, 80, 81]   # 64 (8, 8)
row8 = [a1, b1, c1, d1, e1, f1, g1, h1]

chessBoard = [row1, row2, row3, row4, row5, row6, row7, row8]

chessSquares = {}
chessboardPath = "./MachineLearning/data/images/blank_chessboard.jpg"

starterBoard = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", "p", "p", "p", "p", "p"],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", "N", "B", "Q", "K", "B", "N", "R"]
    ]


step0 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", "p", "p", "p", "p", "p"],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", "N", "B", "Q", "K", "B", "N", "R"]
    ]
step1 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", "p", "p", "p", "p", "p"],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", "N", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", "N", "B", "Q", "K", "B", " ", "R"]
    ]
step2 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", " ", "p", "p", "p", "p"],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", "N", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", "N", "B", "Q", "K", "B", " ", "R"]
    ]
step3 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", " ", "p", "p", "p", "p"],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", " ", "B", "Q", "K", "B", " ", "R"]
    ]
step4 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", " ", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", "P", "P", "P", "P", "P"],
        ["R", " ", "B", "Q", "K", "B", " ", "R"]
    ]
step5 = [
        ["r", "n", "b", "q", "k", "b", "n", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", "B", "Q", "K", "B", " ", "R"]
    ]
step6 = [
        ["r", "n", "b", "q", "k", " ", "n", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", "b", " ", "P", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", "B", "Q", "K", "B", " ", "R"]
    ]
step7 = [
        ["r", "n", "b", "q", "k", " ", "n", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", "B", " "],
        [" ", "b", " ", "P", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step8 = [
        ["r", "n", "b", "q", "k", " ", " ", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        [" ", " ", " ", "p", " ", " ", "B", " "],
        [" ", "b", " ", "P", " ", " ", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step9 = [
        ["r", "n", "b", "q", "k", " ", " ", "r"],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", "b", " ", "P", " ", "B", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step10 = [
        ["r", "n", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", "b", " ", "P", " ", "B", " ", " "],
        [" ", " ", "N", " ", " ", "N", " ", " "],
        ["P", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step11 = [
        ["r", "n", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", " "],
        [" ", "b", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", " ", "N", " ", " "],
        [" ", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step12 = [
        ["r", "n", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", " ", "N", " ", " "],
        [" ", "P", "P", " ", "P", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step13 = [
        ["r", "n", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step14 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", "n", " ", "p", "n", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", "B", " ", "R"]
    ]
step15 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", " ", "p", "p", "p"],
        [" ", " ", "n", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step16 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", "n", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", " "],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", "P"],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step17 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", "n", "p", "p", "p"],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", " "],
        [" ", " ", " ", "P", " ", "B", " ", "P"],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step18 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", "n", "p", "p", " "],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", "p"],
        [" ", " ", " ", "P", " ", "B", " ", "P"],
        ["P", " ", "N", " ", "P", "N", " ", " "],
        [" ", "P", "P", " ", " ", "P", "P", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step19 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", "p", " ", "n", "p", "p", " "],
        [" ", " ", " ", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", "p"],
        [" ", " ", " ", "P", " ", "B", " ", "P"],
        ["P", " ", "N", " ", "P", "N", "P", " "],
        [" ", "P", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step20 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", " ", "p", " ", "p", "n", " ", " "],
        ["b", "B", " ", "p", " ", " ", " ", "p"],
        [" ", " ", " ", "P", " ", "B", " ", "P"],
        ["P", " ", "N", " ", "P", "N", "P", " "],
        [" ", "P", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step21 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", " ", "p", " ", "p", "n", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", "p"],
        [" ", " ", " ", "P", " ", "B", " ", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", "P", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step22 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", " ", "p", " ", "p", " ", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", "p"],
        [" ", " ", " ", "P", " ", "B", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", "P", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step23 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", " ", "p", " ", "p", " ", " ", " "],
        ["b", " ", " ", "p", " ", " ", " ", "p"],
        [" ", "P", " ", "P", " ", "B", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step24 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", " ", "p"],
        [" ", "P", " ", "P", " ", "B", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step25 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", "n", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", " ", "B", "p"],
        [" ", "P", " ", "P", " ", " ", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step26 = [
        ["r", " ", "b", "q", " ", "r", "k", " "],
        ["p", "p", " ", " ", " ", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", "n", "B", "p"],
        [" ", "P", " ", "P", " ", " ", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step27 = [
        ["r", " ", "b", "B", " ", "r", "k", " "],
        ["p", "p", " ", " ", " ", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", "n", " ", "p"],
        [" ", "P", " ", "P", " ", " ", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step28 = [
        ["r", " ", "b", "r", " ", " ", "k", " "],
        ["p", "p", " ", " ", " ", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", "n", " ", "p"],
        [" ", "P", " ", "P", " ", " ", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", "K", " ", " ", "R"]
    ]
step29 = [
        ["r", " ", "b", "r", " ", " ", "k", " "],
        ["p", "p", " ", " ", " ", "p", "p", " "],
        [" ", "b", "p", " ", "p", " ", " ", " "],
        [" ", " ", " ", "p", " ", "n", " ", "p"],
        [" ", "P", " ", "P", " ", " ", "n", "P"],
        ["P", " ", "N", "B", "P", "N", "P", " "],
        [" ", " ", "P", " ", " ", "P", " ", " "],
        ["R", " ", " ", "Q", " ", "R", "K", " "]
    ]

steps = [step0, step1, step2, step3, step4, step5, step6, step7, step8, step9, 
        step10, step11, step12, step13, step14, step15, step16, step17, step18, step19,
        step20, step21, step22, step23, step24, step25, step26, step27, step28, step29]
