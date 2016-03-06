cmake_minimum_required(VERSION 3.2)
project(apa102_display)

set(ENV{PATH} "/home/fuho/Projects/GitHub/esp-open-sdk/xtensa-lx106-elf/bin:/home/fuho/.nvm/versions/node/v0.12.2/bin:/home/fuho/Projects/GitHub/esp-open-sdk/xtensa-lx106-elf/bin:/home/fuho/.rvm/gems/ruby-2.1.0/bin:/home/fuho/.rvm/gems/ruby-2.1.0@global/bin:/home/fuho/.rvm/rubies/ruby-2.1.0/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/jvm/java-8-oracle/bin:/usr/lib/jvm/java-8-oracle/db/bin:/usr/lib/jvm/java-8-oracle/jre/bin:/home/fuho/.rvm/bin:/home/fuho/.rvm/bin:/home/fuho/.node/bin:/home/fuho/.rvm/bin:/home/fuho/.node/bin:/home/fuho/.rvm/bin")
set(PLATFORMIO_CMD "/usr/local/bin/platformio")

include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/variants/nodemcu")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266/libb64")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266/spiffs")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266/umm_malloc")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/tools/sdk/include")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ArduinoOTA")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/DNSServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/EEPROM")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266AVRISP/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266HTTPClient/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266HTTPUpdateServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266SSDP")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WebServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFi/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFiMesh/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266httpUpdate/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266mDNS")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Ethernet/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/GDBStub/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Hash/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SD/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SPI")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Servo/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SoftwareSerial")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/TFT_Touch_Shield_V2")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Ticker")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Wire")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/esp8266/src")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include-fixed")

add_definitions(-DF_CPU=80000000L)
add_definitions(-D__ets__)
add_definitions(-DICACHE_FLASH)
add_definitions(-DESP8266)
add_definitions(-DARDUINO_ARCH_ESP8266)
add_definitions(-DARDUINO_ESP8266_ESP12)
add_definitions(-DARDUINO=20100)
add_definitions(-DPLATFORMIO=020804)

add_custom_target(
    PLATFORMIO_BUILD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOAD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_CLEAN ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_PROGRAM ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOADFS ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPDATE_ALL ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# To enable code auto-completion, please specify path
# to main source file (*.c, *.cpp) and uncomment line below
#
# add_executable(apa102_display src/main_change_me.cpp)
