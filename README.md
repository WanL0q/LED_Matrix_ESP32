# LED_Sceen_ESP32
Controlling LED Screen with ESP32
![Screenshot 2024-03-22 094515](https://github.com/WanL0q/LED_Sceen_ESP32/assets/134664967/50ce7511-66b9-4182-a1b7-aac662588595)

# CAN Frames (Device ID: 0x2A)
## Led Strip Command Frame

| **ID package**          | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length**                                                             |
|:-----------------------:|:-------------:|:-----------------------:|:---------------------------------------------------------------------------:|
| 0x01                    | 100ms         | 500ms                   | 0x08                                                                        |
| **Location**            |  **Function** | **Data type**           | **Description**                                                             |
| byte[0]                 | Led mode      | bit[0,1]                | 0: normal<br>1: left<br>2: right<br>3: error                                |
|                         |               | bit[2,3]                | 0: off<br>1: auto<br>2: manual<br>3: custom (bypass)                        |
|                         |               | bit[4,5]                | Reserve                                                                     |
|                         |               | bit[6,7]                | Reserve                                                                     |
| byte[1]                 | Effect        | unsigned int            | 0: Solid<br>1: Pulse<br>2: Rainbow<br>3: ColorCycle<br>4: Blink<br>5: Comet |
| byte[2]                 | Reserve       | _                       | 0x00                                                                        |
| byte[3]                 | Reserve       | _                       | 0x00                                                                        |
| byte[4]                 | Reserve       | _                       | 0x00                                                                        |
| byte[5]                 | Reserve       | _                       | 0x00                                                                        |
| byte[6]                 | Reserve       | _                       | 0x00                                                                        |
| byte[7]                 | Reserve       | _                       | 0x00                                                                        |
## Led Strip Configuration Frame

| **ID package**         | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:----------------------:|:-------------:|:-----------------------:|:---------------:|
| 0x02                   | -             | -                       | 0x08            |
| **Location**           | **Function**  | **Data type**           | **Description** |
| byte[0]                |Led_front_left | unsigned int8           | [0,255]         |
| byte[1]                |Led_front_right| unsigned int8           | [0,255]         |
| byte[2]                |Led_right      | unsigned int8           | [0,255]         |
| byte[3]                |Led_back_right | unsigned int8           | [0,255]         |
| byte[4]                |Led_back       | unsigned int8           | [0,255]         |
| byte[5]                |Led_back_left  | unsigned int8           | [0,255]         |
| byte[6]                |Led_left       | unsigned int8           | [0,255]         |
| byte[7]                |Led_flag       | unsigned int8           | [0,255]         |

## Led Strip Feedback Frame
### Mode Feedback Frame
| **ID package**         | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:----------------------:|:-------------:|:-----------------------:|:---------------:|
| 0x10                   | -             | -                       | 0x08            |
| **Location**           | **Function**  | **Data type**           | **Description** |
| byte[0]                | Led mode      | bit[0,1]                | 0: normal<br>1: left<br>2: right<br>3: error                                |
|                        |               | bit[2,3]                | 0: off<br>1: auto<br>2: manual<br>3: custom (bypass)                        |
|                        |               | bit[4,5]                | Reserve                                                                     |
|                        |               | bit[6,7]                | Reserve                                                                     |
| byte[1]                | Effect        | unsigned int            | 0: Solid<br>1: Pulse<br>2: Rainbow<br>3: ColorCycle<br>4: Blink<br>5: Comet |
### Configuration Feedback Frame
| **ID package**         | **Cycle(ms)** | **Receive-timeout(ms)** | **Data length** |
|:----------------------:|:-------------:|:-----------------------:|:---------------:|
| 0x11                   | -             | -                       | 0x08            |
| **Location**           | **Function**  | **Data type**           | **Description** |
| byte[0]                |Led_front_left | unsigned int8           | [0,255]         |
| byte[1]                |Led_front_right| unsigned int8           | [0,255]         |
