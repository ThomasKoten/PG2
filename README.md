# PG2
## Závislosti
WINDOWS: 
Download from https://opencv.org/releases/ 

Extract opencv-4.6.0-vc14_vc15.exe to C:\

Add user specific variable:
**OPENCV_DIR=C:\opencv\build**

Add default path
**PATH=%PATH%;%OPENCV_DIR%\x64\vc15\bin**

## Instalace
1. Stáhněte si zkomprimovaný soubor `PG2-main.zip` a na svém zařízení ho rozbalte.
2. Soubor otevřete ve Visual Studiu nebo otevřete soubor `PG2.sln` ve složce `PG2-main`.
3. Zmáčkněte společně klávesy **_Alt_** a **_Enter_**.
4. V nastavení pro konfiguraci **All configurations**, v sekci **Debugging** nastavte položku **Environment** na `PATH=%PATH%;$(ProjectDir)bin;`.
5. Nyní by mělo být možné projekt spustit.

## Ovládání
* `W` - Pohyb vpřed
* `A` - Pohyb doleva
* `S` - Pohyb vzad
* `D` - Pohyb doprava
* `V` - Zapnutí/vypnutí Vsync
* `F` - Zapnutí/vypnutí režimu Fullscreen
* `ESC` - Vypnutí aplikace
