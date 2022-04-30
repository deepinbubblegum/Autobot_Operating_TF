import scrcpy
class Devices():
    def __init__(self, client):
        self.client = client.client
        self.client.add_listener(
            scrcpy.EVENT_INIT, 
            self.on_device_init
        )

    def on_device_init(self):
        self.device_name = self.client.device_name
        self.resolution = self.client.resolution
        # print(self.device_name)
        print(f'resolution : {self.resolution}')