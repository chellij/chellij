from src.chess.helperFunctions import *
from src.chess.autoChessboardDetect import lineDetect, cornerDetect
import pygame

import pygame.camera
from pygame.locals import *
from pygame_gui.ui_manager import UIManager
from pygame_gui.elements import UIButton
from pygame_gui.elements.ui_text_box import UITextBox
from pygame_gui import UI_BUTTON_PRESSED

fps = 30 
DISPLAY_SIZE = (1600, 900)
background_colour = (32, 49, 77)
caption = "Chess Piece Detection"
currentBoard = [[' ' for i in range(8)] for i in range(8)]
changeLog = "LOG:"
turn = 0

from pygrabber.dshow_graph import FilterGraph

def get_available_cameras() :

    devices = FilterGraph().get_input_devices()

    available_cameras = {}

    for device_index, device_name in enumerate(devices):
        available_cameras[device_index] = device_name

    return available_cameras


def clickUpdate(cam, textBox3):
    global turn
    turn = turn + 1
    vid, fen = showWebcam(cam, textBox3)
    return vid, fen

# =============================================================================

def runGUI():
    global changeLog, chessSquares
    
    chessSquares = setupChess.chessDict
    pygame.init()
    pygame.camera.init()    

    pygame.display.set_caption(caption)
    window_surface = pygame.display.set_mode(DISPLAY_SIZE)

    background = pygame.Surface(DISPLAY_SIZE)
    background.fill(pygame.Color(background_colour))
        
    fen_background = pygame.image.load("./MachineLearning/data/images/FENBackground.png").convert()

    manager = UIManager(DISPLAY_SIZE, './MachineLearning/data/theme.json')

    clock = pygame.time.Clock()
    is_running = True

    textbox1 = UITextBox('Please choose:', pygame.Rect(10, 10, 280, 50), manager=manager, object_id='#text_box_1')
    textbox3 = UITextBox(changeLog, pygame.Rect(300, 600, 800, 200), manager=manager, object_id="#text_box_1")
    
    button1 = UIButton(relative_rect=pygame.Rect((10, 80),  (280, 50)), text='UPDATE', manager=manager)
    button2 = UIButton(relative_rect=pygame.Rect((10, 130), (280, 50)), text='QUIT', manager=manager)
    button3 = UIButton(relative_rect=pygame.Rect((10, 180), (280, 50)), text='PRINT', manager=manager)
    button4 = UIButton(relative_rect=pygame.Rect((10, 230), (280, 50)), text='V12', manager=manager)
    button5 = UIButton(relative_rect=pygame.Rect((10, 280), (280, 50)), text='V15', manager=manager)
    
    cam = pygame.camera.Camera("Logi C270 HD WebCam",(640,480))
    cam.start()
    
    vid, fen = clickUpdate(cam, textbox3)
    while is_running:
        time_delta = clock.tick(60)/1000.0
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                is_running = False
            if event.type == pygame.KEYDOWN and (event.key == ord('q') or event.key == ord('Q')):
                is_running = False
            if event.type == pygame.KEYDOWN and (event.key == ord('u') or event.key == ord('U')):
                vid, fen = clickUpdate(cam, textbox3)
            if event.type == UI_BUTTON_PRESSED and event.ui_element == button1:
                vid, fen = clickUpdate(cam, textbox3)
            if event.type == UI_BUTTON_PRESSED and event.ui_element == button2:
                cam.stop()
                is_running = False
            if event.type == UI_BUTTON_PRESSED and event.ui_element == button3:
                print(changeLog)
            if event.type == UI_BUTTON_PRESSED and event.ui_element == button4:
                model = loadDataset(12)
            if event.type == UI_BUTTON_PRESSED and event.ui_element == button5:
                model = loadDataset(15)

            manager.process_events(event)

        window_surface.blit(background, (0,0))
        window_surface.blit(fen_background, (950,10))
        window_surface.blit(vid, (300, 10))
        window_surface.blit(fen, (990, 50))
        manager.update(time_delta)
        manager.draw_ui(window_surface)
        pygame.display.update()

    pygame.quit()


# ==============================================

def showWebcam(vid, textBox3):
    global currentBoard, model, changeLog, corners
    image = vid.get_image()
    im = pygame.surfarray.array3d(image)
    frame=cv2.cvtColor(im, cv2.COLOR_RGB2BGR)
    frame=np.rot90(frame, 3)
    frame=np.flip(frame, 1)
    cv2.imwrite("./MachineLearning/results/test.jpg", frame)
    if not ('model' in globals()):
        model = loadDataset()
    try: 
        newBoard, im = runMainTask(model, corners, frame)
    except NameError as e:
        print("Chessboard not yet defined, manually select corners.")
        mask, masked_original, pts = create_mask(chessboardPath)
        runWarp(pts, masked_original)
        corners = drawDots(masked_original)
        currentBoard = starterBoard
        newBoard, im = runMainTask(model, corners, frame)
        
    _ = compareBoard(currentBoard, newBoard, "compare")
    fenCode, fen = displayFen(newBoard, True)
    changeLog = (f"{changeLog}\n{turn}: {fenCode}")
    textBox3.set_text(changeLog)
    currentBoard = newBoard
    cv2.imwrite("./MachineLearning/results/temp.jpg", im)
    im = pygame.image.load("./MachineLearning/results/temp.jpg").convert()
    return im, fen
            

# =============================================================================

if __name__ == "__main__":
    print(f"Camera Options: {get_available_cameras()}. \n")
    while True:
        value = input("""\nPlease choose from the following options:\n
                      1. Load model
                      2. Setup with live background image
                      3. Setup with preset background image
                      4. Run GUI
                      5. Run Testing
                      6: Example Videos
                      X: Close program\n""")
        print(f'\nYou entered {value}.\n')

        if value == '1':
            model = loadDataset()
        elif value == '2':
            _, masked_original = cornerDetect("webcam")
            pts = lineDetect(masked_original)
            runWarp(pts, masked_original)
            corners = drawDots(masked_original)
            currentBoard = starterBoard
        elif value == '3':
            mask, masked_original, pts = create_mask(chessboardPath)
            runWarp(pts, masked_original)
            corners = drawDots(masked_original)
            currentBoard = starterBoard
        elif value == '4':
            runGUI()
        elif value == '5':
            m = input("Which test do you want to start at? (V1, V3, V8, V12, or V15) ")
            runTest(m)
        elif value == '6':
            exampleVideo()
        elif value == 'X' or value == 'x':
            quit()
        else:
            print("Please enter one of the options\n")
    