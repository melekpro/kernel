/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef __HWMSEN_CUSTOM_H__ 
#define __HWMSEN_CUSTOM_H__

#ifdef CUSTOM_KERNEL_ACCELEROMETER
    #define ACCELEROMETER           "KXTJ2-1009 3-axis Accelerometer"
    #define ACCELEROMETER_VENDER    "Kionix"
#endif

#ifdef CUSTOM_KERNEL_MAGNETOMETER
    #define MAGNETOMETER "S623 Magnetic Field Sensor"
    #define MAGNETOMETER_VENDER "Smartsensor Inc."
    #define MAGNETOMETER_RANGE 1400.0f
    #define MAGNETOMETER_RESOLUTION 0.3571428f
    #define MAGNETOMETER_POWER 0.6f
    #define ORIENTATION "S623 Orientation Sensor"
    #define ORIENTATION_VENDER "Smartsensor Inc."
    #define ORIENTATION_RANGE 360.0f
    #define ORIENTATION_RESOLUTION 1.0f
    #define ORIENTATION_POWER 0.6f
#endif

#endif

