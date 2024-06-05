# Embedding System Final Project

## Setup

### Mbed Libraries (On Mbed GUI)

1. BB Car library.
   - Click **`+`** in the Library tab under program source.
   - Fill in `https://gitlab.larc-nthu.net/ee2405_2022/bbcar.git` And click "Next"
   - Select "main" branch and click "Finish"
2. PWMIn library.
   - Click **`+`** in the Library tab under program source.
   - Fill in `https://gitlab.larc-nthu.net/ee2405_2022/pwmin.git` And click "Next"
   - Select "main" branch and click "Finish"
3. BSP library for running accelerometer
   - Click **`+`** in the Library tab under program source.
   - Fill in `https://gitlab.larc-nthu.net/ee2405_2022/bsp-b-l475e-iot01.git` And click "Next"
   - Select "main" branch and click "Finish"

### Virtual Python Environment for BLE (On Windows PowerShell)

``` shell=
$ cd D:\User\virtualenvs
$ ee2405\Scripts\activate
```

- Download Python Libraries
``` shell=
$ python.exe -m pip install asyncio
$ python.exe -m pip install bleak
```

- Execute
``` shell=
$ cd 'D:\Mbed Programs\mbed-final-project\'
$ python.exe ble.py
```

## Compile and Run Program

Copy the following files to the Mbed folder(the same folder as main.cpp) then run the program 

1. `main.cpp` 
2. `ble_process.h`
3. `gatt_server_process.h`
4. `pretty_printer.h`
5. `accelerometer.h`
6. `gyro.h`
7. `.mbedignore`
8. `ble.py`