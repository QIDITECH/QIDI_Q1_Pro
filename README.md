<p align="center"><img src="other/QIDI.png" height="240" alt="QIDI's logo" /></p>
<p align="center"><a href="/LICENSE"><img alt="GPL-V3.0 License" src="other/qidi.svg"></a></p>

# Documentation Guidelines

Q1 Pro is a 3D printer that uses Klipper as its foundation. This repository is used for updates and releases for the Q1 Pro model, as well as for issue tracking.

For convenience, QIDI provides version-specific packaged files. Please download the necessary compressed package file prefixed with "Q1_Pro." Select the appropriate branch for download, with each branch name reflecting the corresponding version.

## V4.4.24 Update Content

**Note:** After updating, the Klipper configuration file will be replaced. The previous configuration file will be backed up as `printer_{datetime}.cfg`, printer recalibration will be required.

1. Update the QIDI-LINK connection method to make the connection more secure.

2. Added Fluidd account management module

3. Fixed the issue of USB not being read on startup

4. Updated configuration files
    

We also addressed an issue where the display of connected WiFi names containing Unicode characters appeared blank, and this has been fixed.

**6.** Adjusted default parameters.

## Detailed update process

#### Packaged files

Note that all updates can not be updated from higher versions

1. Select the latest version in the version release bar next to it, download the compressed file package starting with Q1_Pro and extract it locally. <a href="https://github.com/QIDITECH/QIDI_Q1_Pro/releases">Jump link</a>

2. Transfer the files to a USB drive. For example:

    <p align="left"><img src="other/sample.png" height="240" alt="sample"></p>

3. Insert the USB drive into the machine's USB interface, click the `Chcek for updates` button and an update prompt will appear on the version information interface. Update according to the prompt.

## Report Issues and Make Suggestions

For any concerns or suggestions, feel free to reach out to our [After-Sales Service](https://qidi3d.com/pages/warranty-policy-after-sales-support).

Should you encounter any issues related to machine mechanics, slicing software, firmware, or various other machine-related problems, our after-sales team is ready to assist. They aim to respond to all inquiries within twelve hours. Alternatively, you can post an issue in this repository, our developers will reply to you directly.

## Others
QIDI's 3D printers operate based on the Klipper system. Building on the Klipper open-source project, we've tailored its source code to meet specific user requirements.

Similarly, we've adapted Moonraker to ensure our designed screens align with web operations.

We also use Fluidd as one of the ways for users to interact with the printer.

We extend our gratitude to the developers and maintainers of these open-source projects and encourage users to explore or support these great projects.

| Software      |Official| QIDI edition                                                                     |
| ------------- |----------| -------------------------------------------------------------------------------- |
| **Klipper**   |**[https://github.com/Klipper3d/klipper](https://github.com/Klipper3d/klipper)**| **[https://github.com/QIDITECH/klipper](https://github.com/QIDITECH/klipper)**   |
| **Moonraker** |**[https://github.com/Arksine/moonraker](https://github.com/Arksine/moonraker)**| **[https://github.com/QIDITECH/moonrake](https://github.com/QIDITECH/moonrake)** |
|**Fluidd**|**[https://github.com/fluidd-core/fluidd](https://github.com/fluidd-core/fluidd)**||
