# Serial Protocol

If an older software release is used, this doc has to bee checked out from the corresponding commit.

## Frame
| command | length | data | checksum |
|:---:|:---:|:---:|:---:|
| 1 byte |  1 byte | `length` bytes |  1 byte |

### Answer Status
The answers first data byte is a status byte, which is interpreted as following:
| Byte | Value | Description |
|:---|:---|:---|
| DATA0 | 0x00<br/>0x01<br/>0x02<br/>0x03<br/>0x04 ... 0xFF | OK<br/>General error<br/>Invalid checksum<br/>Invalid parameter<br/>_res._ |


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
| DATA0<br/>1<br/>2<br/>3 | 0xXX<br/>0x00<br/>0xXX<br/>0xXX | Status<br/>Info: Version<br/>Version major<br/>Version minor |
| CS | 0xXX ||

### Set Display
#### Command
| Byte | Value | Description |
|:---|:---|:---|
| CMD | 0x0A ||
| LEN | 0xXX | Number of chars |
| DATA0<br/>...<br/>DATAn | 0xXX<br/>...<br/>0xXX | Digit 0<br/>...<br/>Digit n |
| CS | 0xXX ||

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


## Charset
### HDSP Standart
The data format is described in the displays [datasheet](https://docs.broadcom.com/doc/AV02-0629EN). It's a standard ASCII table except for the control chars and the following two chars:

| char | displayed as |
|:---:|:---:|
| 0x5E &#94; | &#8593; (U+2191) |
| 0x7C &#124; | &#166; (U+00A6) |

### Default UDC (User Defined Characters)
The HDSP does not have any default UDCs, but the driver software provides the following defaults:

| char | displayed as || char | displayed as |
|:---:|:---:|:---:|:---:|:---:|
| 0x80 | _not set_ || 0x88 | &#9617; (U+2591) |
| 0x81 | Battery 0% || 0x89 | &#9619; (U+2593) |
| 0x82 | Battery 25% || 0x8A | &#9608; (U+2588) |
| 0x83 | Battery 50% || 0x8B | &#9205; (U+23F5) |
| 0x84 | Battery 75% || 0x8C | &#124; (U+007C) |
| 0x85 | Battery 100% || 0x8D | _not set_ |
| 0x86 | Smiley || 0x8E | _not set_ |
| 0x87 | &#94; (U+005E) || 0x8F | _not set_ |
