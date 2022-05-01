import json
from adbutils import adb

class Connections():
    def __init__(self, config):
        self.index_id = config.config["index_id"]
        self.ip_port = f'{config.config["ip_address"]}:{config.config["port"]}'
        self.connect()
        
    def connect(self):
        try:
            adb.connect(self.ip_port)
            self.devices = adb.device_list()
        except Exception as e:
          print(e)
        finally:
            self.disconnect()
            
    def disconnect(self):
        try:
            adb.disconnect(self.ip_port)
        except Exception as e:
            print(e)