# QSLibSDH

QSLibSDH (long name QSLibSeedhartha) is a new iteration of the [QSP engine version 5.7.0](https://github.com/seedhartha/QuestPlayer/tree/34c74d3a073ee5ce7b053cb68c94699e2949dbe2/app/src/main/jni) specifically for [QSPlugin](https://github.com/l3ger0j/QSPlugin)

## Changes

The main changes relate to JVM bindings and are necessary to enable the library to be used in Android OS applications. The build system has also been modified, with the addition of auto-generation of certain files and the replacement of the keyword 'static' with 'INLINE' in some instances.