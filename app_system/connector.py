import json
from adbutils import adb

class Connections():
    def __init__(self):
        conf = open('config.json')
        self.config = json.load(conf)
        self.index_id = self.config["connect"]["Id"]
        self.ip_port = f'{self.config["connect"]["IPaddress"]}:{self.config["connect"]["Port"]}'
        self.connect()
        
    def connect(self):
        try:
            adb.connect(self.ip_port)
            self.devices = adb.device_list()
        except:
          print('error pless check config.json file or emulator')
        finally:
            self.disconnect()
            
    def disconnect(self):
        try:
            adb.disconnect(self.ip_port)
        except:
            pass