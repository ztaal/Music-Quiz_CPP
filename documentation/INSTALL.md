# Installation of Software
  
## Qt 

Download [Qt](https://www.qt.io/offline-installers) and install it.

### Set Environmental Variables

Windows 10:

- Open Control Panel
- Go to 'System and Security\System'
- Click on 'Advance system settings'
- Click on 'Environmental Variables'
- Under System variables open 'Path'
- Add a new path to the Qt '\msvc' and '\msvc\bin' folder
    - Example:
       - D:\Qt5-14-2\5.14.2\msvc2017\bin
       - D:\Qt5-14-2\5.14.2\msvc2017

## Boost

Download [Boost](https://sourceforge.net/projects/boost/files/boost-binaries/) and install it.

### Set Environmental Variables

Windows 10:

- Open Control Panel
- Go to 'System and Security\System'
- Click on 'Advance system settings'
- Click on 'Environmental Variables'
- Add 'BOOST_ROOT' with the root folder as value.
  - Example 'D:\boost_1_69_0"
- Add 'BOOST_INCLUDEDIR' with the root folder as value.
  - Example 'D:\boost_1_69_0"
- Add 'BOOST_LIBRARYDIR' with the msvc folder as value.
  - Example 'D:\boost_1_69_0\lib32-msvc-14.1"
- Add 'boost_1_69_0\lib32-msvc-14.1' to the path variable.
