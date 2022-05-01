import json
import sys
class LoadConfig:
    def __init__(self):
        print("start load config...")
        self.config = None
        self.source_game = None
        self.init_main_config()
        self.init_source_config()
        print(self.config["emulator"])
        if self.config["force_not_support"] is False:
            from app_system.emu_versions_check import get_version
            self.isSupport = False
            # check version emulator support
            version = get_version(self.config["emulator"])
            for emulist in self.source_game["emulist"]:
                if emulist["emu_name"] == self.config["emulator"]:
                    for emu_version in emulist["version"]:
                        if emu_version == version:
                            print(f'Emulator version {version}')
                            self.isSupport = True
            if self.isSupport is False:
                print("Emulator version not supported")
                sys.exit(0)
        else:
            print("force support mode")
        print(self.source_game["game_name"])
    
    def init_main_config(self):
        config = json.load(open('config.json'))
        # set variable
        index_id = config["connect"]["Index_emu"]
        ip_address = config["connect"]["IPaddress"]
        port = config["connect"]["Port"]
        source_game_id = config["connect"]["SourceGameId"]
        gpu = config["connect"]["GPU"]
        force_not_support = config["connect"]["ForceNotSupport"]
        debug = config["connect"]["Debug"]
        emulator = config["connect"]["emulator"]
        self.config = {
            'index_id' : index_id, 
            'ip_address': ip_address,
            'port' : port,
            'source_game_id' : source_game_id, 
            'gpu' : gpu,
            'force_not_support' : force_not_support,
            'debug' : debug,
            "emulator" : emulator
        }
        
    def init_source_config(self):
        source_list = json.load(open('params/source_list.json'))
        name = source_list[self.config["source_game_id"]]["name"]
        threshold = source_list[self.config["source_game_id"]]["threshold"]
        emulator_info = source_list[self.config["source_game_id"]]["emu_support"]
        emulist = []
        for emu in emulator_info:
            emulist.append(emu)
        self.source_game = {
            "game_name" : name,
            "threshold" : threshold,
            "emulist": emulist
        }