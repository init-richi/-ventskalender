# @ventskalender
Ein Adventskalender für die Liebsten, welcher für jeden Tag ein Soundfile abspielt

Der "@ventskalender" (nicht mit dem CSU Twitteraccount verwechseln), ist ein Projekt für den ESP8266.
Er besteht aus einer Holzkiste, die, wenn sie geöffnet wird, ein Soundfile für jeden Tag im Dezember abspielt.
Dazu brauch man einen DFPlayerMini und einen EP8266, in meinem Fall einen ESP12F (Eine deteilierte Teilliste folgt noch).

Das Projekt ist nicht in der Arduino IDE, sondern in VisualStudio Code mit PlattformIO entwickelt worden.

Es werden bestimmte Librarys gebraucht, um das Projekt zu compilen und auf den ESP Hochzuladen:
* [ConfigManager](https://github.com/mariacarlinahernandez/ConfigManager)
* [EasyNTPClient](https://github.com/aharshac/EasyNTPClient)
* [DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)
* [Time](https://github.com/PaulStoffregen/Time)
* [WS2812FX](https://github.com/kitesurfer1404/WS2812FX)

