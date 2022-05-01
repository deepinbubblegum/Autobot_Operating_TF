import sys,os
sys.path.append(os.path.join(os.getcwd(), 'deps'))
sys._MEIPASS=os.path.join(sys._MEIPASS, 'deps')
sys.path.append(os.path.join(os.path.dirname(sys.argv[0]), "lib")) # for pyd
sys.path.append(os.path.join(os.path.dirname(sys.argv[0]), "dll")) # for dll