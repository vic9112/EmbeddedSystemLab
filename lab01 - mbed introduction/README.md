# Lab 1 

## Introduction
- With the STMicroelectronics Discovery kit for IoT node, users develop applications with direct connection to cloud servers. The Discovery kit enables a wide diversity of applications by exploiting low-power communication, multiway sensing and Arm® Cortex®-M4 core-based STM32L4+ Series features. The support for ARDUINO® Uno V3 and Pmod™ connectivity provides unlimited expansion capabilities with a large choice of specialized add-on boards.

## Equipment
- B-L4S5I-IOT01A
  ![B-L4S5I-IOT01A](https://github.com/vic9112/Embedded_System_Lab/assets/137171415/a5c38a9c-5b0e-4724-b4ed-c32fb681fa69)

## Programming with Mbed Studio

### Install Mbed Studio

[Download and Install](https://os.mbed.com/studio/)

- Step 1
  ![Step 1](https://github.com/vic9112/Embedded_System_Lab/assets/137171415/805279fe-cc6f-469b-929a-c855589a25dd)
- Step 2
  -> I Agree
- Step 3
  -> Only for me (user name)
- Step 4
  ![Step 4](https://github.com/vic9112/Embedded_System_Lab/assets/137171415/6fdb035d-8d6c-4512-99ad-31b1486d5648)

### Create & Compile a Mbed Program

- [Reference intructions to create and compile a mbed program](https://os.mbed.com/docs/mbed-studio/current/create-import/index.html)
- Open the **File** -> **New Program**
- Select `empty Mbed OS program` under **MBED OS 6**.
- Enter program name
- Make this the active program(default)
- Under **Mbed OS Location**, check "Store Mbed OS in the program folder (+1GB)"
- Add Program
- After above steps, the **`mbed-os/`** stores the current Mbed OS. We will reuse this copy of Mbed OS for other labs

### Create a Second Mbed Program

- Create a new program
- **File** -> **New Program**
- Select `empty Mbed OS program` under **MBED OS 6**.
- Enter program name. Chekc "Make this the active program"
- Under **Mbed OS Location**, checl "Link to an existing shared Mbed OS instance"
- Select "~/Mbed Programs/mbed01/mbed-os/". This will reuse Mbed OS source codes is mbed01/.
- Add Program

#### .mbedignore
- To expedite the compilation process, create a new file named **`.mbedignore`** in the same directory as our `main.cpp` file. This file will help us ignore unnecessary components, speeding up the compilation time.
``` txt=
/* Bootloader */
mbed-os/features/FEATURE_BOOTLOADER/*

/* BLE */
mbed-os/connectivity/drivers/ble/*
mbed-os/connectivity/FEATURE_BLE/*

/* Cellular */
mbed-os/connectivity/cellular/*
mbed-os/connectivity/drivers/cellular/*
mbed-os/connectivity/netsocket/source/Cellular*.*

/* Device Key */
mbed-os/drivers/device_key/*

/* Experimental /
mbed-os/platform/FEATURE_EXPERIMENTAL_API/*

/* FPGA */
mbed-os/features/frameworks/COMPONENT_FPGA_CI_TEST_SHIELD/*

/* Greentea client */
mbed-os/features/frameworks/greentea-client/*

/* LORAWAN */
mbed-os/connectivity/drivers/lora/*
mbed-os/connectivity/lorawan/*

/* LWIP */
mbed-os/connectivity/drivers/emac/*
mbed-os/connectivity/lwipstack/*

/* Mbed-client-cli */
mbed-os/features/frameworks/mbed-client-cli/*

/* MBED TLS */
mbed-os/connectivity/drivers/mbedtls/*
mbed-os/connectivity/mbedtls/*

/* MBED randlib */
mbed-os/platform/randlib/*

/* Nanostack */
mbed-os/connectivity/drivers/emac/*
mbed-os/connectivity/libraries/mbed-coap/*
mbed-os/connectivity/libraries/nanostack-libservice/*
mbed-os/connectivity/libraries/ppp/*
mbed-os/connectivity/nanostack/*

/* Netsocket */
mbed-os/connectivity/drivers/emac/*
mbed-os/connectivity/netsocket/*
mbed-os/connectivity/libraries/mbed-coap/*
mbed-os/connectivity/libraries/ppp/*

/* NFC */
mbed-os/connectivity/drivers/nfc/*
mbed-os/connectivity/nfc/*

/* RF */
mbed-os/connectivity/drivers/802.15.4_RF/*

/* Storage */
mbed-os/storage/filesystem/*
mbed-os/storage/kvstore/*
mbed-os/storage/platform/*

/* Tests */
mbed-os/platform/tests/*
mbed-os/TEST_APPS/*
mbed-os/TESTS/*
mbed-os/UNITTESTS/*

/* Unity */
mbed-os/features/frameworks/unity/*

/* Utest */
mbed-os/features/frameworks/utest/*

/* USB */
mbed-os/drivers/usb/source/*
mbed-os/hal/usb/source/*
mbed-os/hal/usb/TARGET_Templates/*

/* WiFi */
mbed-os/connectivity/drivers/wifi/*
```

### Setup Mbed OS

- Change the default printf library to **`std`**
  Find `~/Mbed Programs/mbed01/mbed-os/targets/targets.json` in the file browser of Mbed Studio and click to edit it (note the file is in the Mbed OS folder, not in your current project mbed01-3). Search for "printf_lib", and replace "`minimal-printf`" with **`std`**
- Enable a callback feature in task scheduling, which will be used in later labs
  Find `~/Mbed Programs/mbed01/mbed-os/platform/mbed_lib.json` Search and Set "callback-nontrivial" parameter to **`true`**.
