import json
import sys
from win32api import *

def get_version_number(file_path):
    File_information = GetFileVersionInfo(file_path, "\\")
    ms_file_version = File_information['FileVersionMS']
    ls_file_version = File_information['FileVersionLS']
  
    return [str(HIWORD(ms_file_version)), str(LOWORD(ms_file_version)),
            str(HIWORD(ls_file_version)), str(LOWORD(ls_file_version))]

def get_version(emulator):
    try:
      source_path = json.load(open('params/emu_path_file.json'))
      file_path = source_path[emulator]
      return ".".join(get_version_number(file_path))
    except:
      print('please check config file "emu_path_file.json" in directory params')
      sys.exit(0)
    