﻿# HDSP-21xx-driver

A driver for LED character displays.

### Compatible Models

- HDSP-2107
- HDSP-2112
- HDSP-2110
- HDSP-2111
- HDSP-2113
- HDSP-2504 \*
- HDSP-2502 \*
- HDSP-2500 \*
- HDSP-2501 \*
- HDSP-2503 \*

\* Due to their dimensions they are only software compatible and do not fit on the PCB.

---

# Serial Protocol
## Frame
| command | length | data | checksum |
|:---:|:---:|:---:|:---:|
| 1 byte |  1 byte |  `length` bytes |  1 byte |

### Answer Status
The answers first data byte is a status byte, which is interpreted as following:
| Byte | Value | Description |
|:---|:---|:---|
| DATA0 | 0x00<br/>0x01<br/>0x02<br/>0x03<br/>0x04 ... 0xFF | OK<br/>General error<br/>Invalid parameter<br/>Invalid checksum<br/>_res._ |


## Commands
### Get Info
#### Command
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x01 ||
| LEN | 0x01 ||
| DATA | 0x00<br/>0x01 ... 0xFF | Version<br/>_res._ |
| CS | 0xXX ||

#### Answer Version
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x01 ||
| LEN | 0x04 ||
| DATA0<br/>1<br/>2<br/>3 | 0xXX<br/>0x00<br/>0xXX<br/>0xXX | Status<br/>Type = Version<br/>Version major<br/>Version minor |
| CS | 0xXX ||

### Set Display
#### Command
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x0A ||
| LEN | 0x08 ||
| DATA0<br/>...<br/>DATA7 | 0xXX<br/>...<br/>0xXX | Digit 0<br/>...<br/>Digit 7 |
| CS | 0xXX ||

The data format is described in the displays datasheet.

#### Answer
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x0A ||
| LEN | 0x01 ||
| DATA | 0xXX | Status |
| CS | 0xXX ||

### Set I/O
#### Command
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x10 ||
| LEN | 0x01 ||
| DATA | 0x0X | **_bit 0_** LED 1<br/>...<br/>**_bit 3_** LED 4 |
| CS | 0xXX ||

#### Answer
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x10 ||
| LEN | 0x01 ||
| DATA | 0xXX | Status |
| CS | 0xXX ||

### Get I/O
#### Command
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x11 ||
| LEN | 0x00 ||
| CS | 0xXX ||

#### Answer
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x11 ||
| LEN | 0x02 ||
| DATA0 | 0xXX | Status |
| DATA1 | 0xXX | **_bit 0_** LED 1<br/>...<br/>**_bit 3_** LED 4<br/>**_bit 4_** Button state<br/>**_bit 5_** Button rising edge<br/>**_bit 6_** Button falling edge |
| CS | 0xXX ||


## Thrown Errors
May be sent at any time and indicate an error.

### Default Error
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0xE0 ||
| LEN | 0x01 ||
| DATA | 0x00<br/>0x01<br/>0x02<br/>0x03 ... 0xFF | Unspecified error<br/>RX buffer overflow<br/>Timeout<br/>_res._ |
| CS | 0xXX ||

### Task Error
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0xE1 ||
| LEN | 0x01 ||
| DATA | 0xXX | The task status byte collected by the error handler |
| CS | 0xXX ||


## Checksum
The checksum is a XOR operation through all `command`, `length` and `data` bytes.
