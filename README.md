# PAS CO2 Sensor Arduino Library

[![Build Status](https://travis-ci.com/Infineon/arduino-pas-co2-sensor.svg?branch=master)](https://travis-ci.com/Infineon/arduino-pas-co2-sensor)

Arduino library of Infineon **Photoacoustic Spectroscopy (PAS) XENSIV™ CO2** miniaturized sensor.

### Supported Sensor Boards
<table>
    <tr>
        <td align="center"><img src="" width=200></td>
        <td align="center"><img  src="" width=200></td>
        <td align="center"><img src="" width=200></td>
    </tr>
    <tr>
        <td style="text-align: center"><a href="">PAS CO2 Shield 2Go</a></td>
        <td style="text-align: center"><a href="">PAS 2Go Kit</a></td>
    </tr>
</table>

### Supported Host Controllers

This library should be supported by any Arduino controller including at least
one the interfaces required to operate the sensor (I2C, UART or GPIO input or Capture Unit)

Just the following evaluation kits have been verified:
- XMC2Go + Engineering samples :D
- ...


### Getting Started (Serial interface)
1. Install library (usual Arduino installation)
2. Connect the device through the I2C interface to your Arduino Board
3. Go to examples and run the oneshot.ino example

4. Explore the rest of the examples to find out about the sensor and library features.

### Serial API

* startMeasure()
* stopMeasure()
* getCO2()
* getDiagnosis()
* calibrate()
* reset();
* getDeviceID()

### More information

The complete library and project documentation can be found in the **[Wiki or Doxygen ??](https://github.com/infineon/pas-co2-sensor/wiki/Home)** page.

[Doxygen Documentation](somewhere)
  
### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


