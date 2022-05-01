# -*- mode: python ; coding: utf-8 -*-
# import sys
# import os
# sys.path.append(os.path.join(os.path.dirname(sys.argv[0]), "lib"))

block_cipher = None

dir = 'AutobotOTTF'

a = Analysis(
    [f'{dir}\\AutoBotOTTF.py'],
    pathex=[],
    binaries=[],
    datas=[
        (f'{dir}/library/scrcpy/*.jar', 'scrcpy'),
        (f'{dir}/README.md', '.'),
        (f'{dir}/config/*.json', 'config'),
        (f'{dir}/params/*.json', 'params'),
        (f'{dir}/resources/LDPlayer', 'resources/LDPlayer'),
    ],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='AutobotOTTF',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='autobot_OTTF',
)
