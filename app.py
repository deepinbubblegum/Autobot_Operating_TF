from app_system.loadconfig import LoadConfig
from app_system.client import Client
from app_system.devices import Devices
from app_system.frames import Frames
from controllers.frame_query import FrameQuery

# load config file
config = LoadConfig()

# create client connect
client = Client(config)
def appsys():
    # device info
    devices = Devices(client)
    # frame capture
    frame = Frames(client)
    main_frame_ctl = FrameQuery(
        frame=frame, 
        devices = devices, 
        config= config
    )
    client.start()
   
try:
    appsys()
except Exception as err:
   print(err)
finally:
   client.stop()