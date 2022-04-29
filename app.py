from app_system.client import Client
from app_system.devices import Devices
from app_system.frames import Frames
from controllers.main_controller import MainController

client = Client()
def appsys():
    devices = Devices(client)
    frame = Frames(client)
    main_ctl = MainController(frame, devices)
    client.start()
   
try:
    appsys()
except Exception as err:
   print(err)
finally:
   client.stop()
   print('exit bot.')