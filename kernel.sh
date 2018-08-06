#!/bin/bash

export ARCH=arm
export SUBARCH=arm
export CROSS_COMPILE=~/t72hm/arm-eabi-4.8/bin/arm-eabi-
make O=out TARGET_ARCH=arm t72hm_defconfig
make O=out TARGET_ARCH=arm | tee build.log
