import cv2
import scrcpy
import glob
import sys
from collections import deque

class Frames():
    def __init__(self, client):
        self.client_ = client
        self.client = client.client
        self.client.add_listener(
            scrcpy.EVENT_FRAME, 
            self.on_frame
        )
        self.frames = deque(maxlen=5)
        
    def on_frame(self, frame):
        if frame is not None:
            self.frames.append(frame)
            cv2.imshow("viz", frame)
        key = cv2.waitKey(1)
        if key == 27:
            self.client_.client.stop()
            sys.exit(0)
            
    def getFrame(self):
        return self.frames
    
    def stop(self):
        self.client_.client.stop()