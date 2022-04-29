import cv2
import threading
import time
from modules.framesprocess import FramesProcess
class MainController:
    def __init__(self, frame, devices):
        self.frame_ = frame
        self.devices = devices
        main_ctl_ = threading.Thread(target=self.update)
        main_ctl_.daemon = True
        main_ctl_.start()
        self.device_name = None
        self.resolution = None
        self.frist_init = False
        self.frame_process_ = FramesProcess()
        
    def process(self, frame):
        self.locations = self.frame_process_.matchTemplate(frame)
        for name, image_template, location in self.locations:
            location = list(zip(*location[::-1]))
            if location:
                needle_w = image_template.shape[1]
                needle_h = image_template.shape[0]
                line_color = (0, 255, 0)
                line_type = cv2.LINE_4
                for loc in location:
                    # Determine the box positions
                    top_left = loc
                    bottom_right = (top_left[0] + needle_w, top_left[1] + needle_h)
                    # Draw the box
                    cv2.rectangle(frame, top_left, bottom_right, line_color, line_type)
        cv2.imshow("debug", frame)
        key = cv2.waitKey(1)
        if key == 27:
            self.frame_.stop()

    def update(self):
        while True:
            time.sleep(0.01)
            try:
                self.frame = self.frame_.getFrame()[-1]
                if self.frame is not None:
                    if self.frist_init is False:
                        self.device_name = self.devices.device_name
                        self.resolution = self.devices.resolution
                    self.process(self.frame)
            except Exception as e:
                print(e)
