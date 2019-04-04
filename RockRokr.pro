TEMPLATE = subdirs
CONFIG += ordered

PhoenixPlayerCore.file = thirdparty/libPhoenixPlayer/libPhoenixPlayer.pro
SUBDIRS += PhoenixPlayerCore

Backend_PhoenixBackend.file = thirdparty/libPhoenixPlayer/Backend_PhoenixBackend.pro
Backend_PhoenixBackend.depends = PhoenixPlayerCore
SUBDIRS += Backend_PhoenixBackend

Decoder_ffmpeg.file = thirdparty/libPhoenixPlayer/Decoder_ffmpeg.pro
Decoder_ffmpeg.depends = PhoenixPlayerCore
SUBDIRS += Decoder_ffmpeg

MetadataLookup_Baidu.file = thirdparty/libPhoenixPlayer/MetadataLookup_Baidu.pro
MetadataLookup_Baidu.depends = PhoenixPlayerCore
SUBDIRS += MetadataLookup_Baidu

MetadataLookup_LastFM.file = thirdparty/libPhoenixPlayer/MetadataLookup_LastFM.pro
MetadataLookup_LastFM.depends = PhoenixPlayerCore
SUBDIRS += MetadataLookup_LastFM

MusicLibrary_SQLite3.file = thirdparty/libPhoenixPlayer/MusicLibrary_SQLite3.pro
MusicLibrary_SQLite3.depends = PhoenixPlayerCore
SUBDIRS += MusicLibrary_SQLite3

MusicLibrary_TagParserPro.file = thirdparty/libPhoenixPlayer/MusicLibrary_TagParserPro.pro
MusicLibrary_TagParserPro.depends = PhoenixPlayerCore
SUBDIRS += MusicLibrary_TagParserPro

OutPut_pulseaudio.file = thirdparty/libPhoenixPlayer/OutPut_pulseaudio.pro
OutPut_pulseaudio.depends = PhoenixPlayerCore
SUBDIRS += OutPut_pulseaudio

app.file = src/RockRokr.pro
app.depends = PhoenixPlayerCore
SUBDIRS += app
