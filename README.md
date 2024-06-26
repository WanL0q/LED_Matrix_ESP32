# LED_Matrix_ESP32
Controlling LED Matrix with ESP32
![Screenshot 2024-03-22 094515](https://github.com/WanL0q/LED_Sceen_ESP32/assets/134664967/50ce7511-66b9-4182-a1b7-aac662588595)
# Convert .mp4 to .bin
- Move the .mp4 file to be converted into the *tool_mp4* folder
- Run the *tool.py* file
- Note:
  - Files saved to the memory card must have names in the format "video + video sequence number . mp4"
  - Video in .mp4 format, 30 fps, and maximum duration of 8s
# CAN Frames (Device ID: 0x3A)
## Led Matrix Command Frame
| **ID package** | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:--------------:|:-------------:|:-----------------------:|:---------------:|
| 0x01           | 100ms         | 500ms                   | 0x08            |
| **Location**   | **Function**  | **Data type**           | **Description** |
| byte[0]        | Emoji mode    | unsigned int8           | 0: OFF          |
|                |               |                         | 1: Rest         |
|                |               |                         | 2: Driving      |
|                |               |                         | 3: Delivery     |
|                |               |                         | 4: Success confirm |
|                |               |                         | 5: Turn Right   |
|                |               |                         | 6: Turn Left    |
| byte[1]        |  Reserve      | -                       | 0x00            |
| byte[2]        |  Reserve      | -                       | 0x00            |
| byte[3]        |  Reserve      | -                       | 0x00            |
| byte[4]        |  Reserve      | -                       | 0x00            |
| byte[5]        |  Reserve      | -                       | 0x00            |
| byte[6]        |  Reserve      | -                       | 0x00            |
| byte[7]        |  Reserve      | -                       | 0x00            |

## Led Matrix Feedback Frame 

| **ID package** | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:--------------:|:-------------:|:-----------------------:|:---------------:|
| 0x11           | 100ms         | 500ms                   | 0x08            |
| **Location**   | **Function**  | **Data type**           | **Description** |
| byte[0]        | Emoji mode    | unsigned int8           | [0,255]         |
| byte[1]        | Number of video | unsigned int8         | 0x00            |
| byte[2]        | Error         | bit[0-1]                | 0: Normal       |
|                |               |                         | 1: Unable to boot SD card |
|                |               |                         | 2: File not found |
|                |               | bit[2-7]                | Reserve         |
| byte[3]        |  Reserve      | -                       | 0x00            |
| byte[4]        |  Reserve      | -                       | 0x00            |
| byte[5]        |  Reserve      | -                       | 0x00            |
| byte[6]        |  Reserve      | -                       | 0x00            |
| byte[7]        |  Reserve      | -                       | 0x00            |

