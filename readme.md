# cli over serial

basic cli example for control arduino via cli connection via serial 

## upload code to arduino

```bash
pio run -t upload
```
## test it via minicom

```
minicom -b 115200 -D /dev/ttyUSB0
# or
minicom -b 115200 -D /dev/ttyACM0
```
