# -*- mode: python ; coding: utf-8 -*-
import os

block_cipher = None

directory = 'AutobotOTTF'

a = Analysis(
    [f'{directory}\\AutoBotOTTF.py'],
    pathex=[],
    binaries=[],
    datas=[
        (f'{directory}/library/scrcpy/*.jar', 'scrcpy'),
        (f'{directory}/README.md', '.'),
        (f'{directory}/config/*.json', 'config'),
        (f'{directory}/params/*.json', 'params'),
        (f'{directory}/resources/LDPlayer', 'resources/LDPlayer'),
    ],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[f'{directory}\\hooker.py'],
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
    name='AutoBotOTTF',
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
    name='AutoBotOTTF',
)
