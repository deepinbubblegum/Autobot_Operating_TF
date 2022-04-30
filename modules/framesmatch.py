import cv2
import numpy as np
import os
import glob

class FramesMatch():
    def __init__(self, config):
        self.names_template = []
        self.images_template = []
        # self.game_name = f'{config.source_game["game_name"]}-{config.source_game["game_name"]["ldplayer"]["keyword"]}'
        # print(self.game_name)
        self.directory = './resources/ragnarok_labyrinth_nft-ldp/*.jpg'
        self.locations = []
        self.threshold = 0.005
        self.load_image()
        self.frist_math_gpu = True
    
    def load_image(self):
        for filename in glob.glob(self.directory):
            name = filename.split('\\')[-1][:-4]
            self.names_template.append(name)
            self.images_template.append(cv2.imread(filename, cv2.IMREAD_UNCHANGED))

    def matchTemplate(self, frame):
        self.locations = []
        self.index_list = 0
        for image in self.images_template:
            result = cv2.matchTemplate(frame, image, cv2.TM_SQDIFF_NORMED)
            self.locations.append([
                self.names_template[self.index_list], image, np.where(result <= self.threshold)])
            self.index_list += 1
        return self.locations
    
    def init_gpu(self):
        self.gsrc = cv2.cuda_GpuMat()
        self.gtmpl = cv2.cuda_GpuMat()
        self.gresult = cv2.cuda_GpuMat()
        
    def matchTemplateGPU(self, frame):
        self.index_list = 0
        if self.frist_math_gpu:
            self.init_gpu()
            self.frist_math_gpu = False
        for image in self.images_template:
            self.gsrc.upload(frame)
            self.gtmpl.upload(image)
            matcher = cv2.cuda.createTemplateMatching(cv2.CV_8UC1, cv2.TM_SQDIFF_NORMED)
            result = matcher.match(self.gsrc, self.gtmpl)
            self.locations.append([
                self.names_template[self.index_list], image, np.where(result <= self.threshold)])
            self.index_list += 1
        return self.locations