# LED_Sceen_ESP32
Controlling LED Screen with ESP32
![Screenshot 2024-03-22 094515](https://github.com/WanL0q/LED_Sceen_ESP32/assets/134664967/50ce7511-66b9-4182-a1b7-aac662588595)

# CAN Frames (Device ID: 0x3A)
## Led Matrix Command Frame
| **ID package** | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:--------------:|:-------------:|:-----------------------:|:---------------:|
| 0x01           | 100ms         | 500ms                   | 0x08            |
| **Location**   | **Function**  | **Data type**           | **Description** |
| byte[0]        | Emoji mode    | unsigned int8           | [0,255]         |
| byte[1]        |  Reserve      | -                       | 0x00            |
| byte[2]        |  Reserve      | -                       | 0x00            |
| byte[3]        |  Reserve      | -                       | 0x00            |
| byte[4]        |  Reserve      | -                       | 0x00            |
| byte[5]        |  Reserve      | -                       | 0x00            |
| byte[6]        |  Reserve      | -                       | 0x00            |
| byte[7]        |  Reserve      | -                       | 0x00            |

## Led matrix Command Frame 

| **ID package** | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:--------------:|:-------------:|:-----------------------:|:---------------:|
| 0x11           | 100ms         | 500ms                   | 0x08            |
| **Location**   | **Function**  | **Data type**           | **Description** |
| byte[0]        | Emoji mode    | unsigned int8           | [0,255]         |
| byte[1]        | Number of video | unsigned int8         | 0x00            |
| byte[2]        | error         | bit[0]                  | Communication SD failure (0 Normal, 1 failure) |
|                |               | bit[1-7]                | Reserve         |
| byte[3]        |  Reserve      | -                       | 0x00            |
| byte[4]        |  Reserve      | -                       | 0x00            |
| byte[5]        |  Reserve      | -                       | 0x00            |
| byte[6]        |  Reserve      | -                       | 0x00            |
| byte[7]        |  Reserve      | -                       | 0x00            |

