#!/bin/bash
export QT_QPA_EGLFS_PHYSICAL_WIDTH=1920 #in [mm]
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=1080 #in [mm]
export QT_QPA_FONT_DIR=/usr/share/fonts/truetype/dejavu
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/qt6/lib/
./StellwerkQt
