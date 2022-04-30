import scrcpy
from app_system.connector import Connections

class Client():
    def __init__(self, config):
        conn = Connections(config)
        self.client = scrcpy.Client(conn.devices[conn.index_id])
        
    def start(self):
        self.client.start()
        
    def stop(self):
        self.client.stop()
        exit(0)