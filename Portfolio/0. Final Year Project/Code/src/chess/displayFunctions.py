from IPython.display import Image, display
from fentoboardimage import fenToImage, loadPiecesFolder
import cv2
import pygame.image as py_im


def displayFen(board, pg):
    fenCode = []
    for row in board:
        row = list(map(lambda x: x.replace(' ', '1'), row))
        row = "".join(row)
        fenCode.append(row)

    fenCode = "/".join(fenCode)
    boardImage = fenToImage(fen=fenCode, squarelength=50, pieceSet=loadPiecesFolder("./MachineLearning/data/pieces"), darkColor="#D18B47", lightColor="#FFCE9E")

    path = "./MachineLearning/results/FEN.png"
    boardImage.save(path)
    if pg:
        fen = py_im.load(path).convert()
    else: 
        fen = cv2.imread(path)
    return fenCode, fen