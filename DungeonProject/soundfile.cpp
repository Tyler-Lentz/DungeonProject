#include "soundfile.h"
#include "utilities.h"

#include <string>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

void playSound(SoundFile& sound)
{
    sound.play();
}

void stopSound(SoundType type)
{
    if (type == SoundType::MP3)
    {
        mciSendString("stop mp3", NULL, 0, NULL);
        mciSendString("close mp3", NULL, 0, NULL);
    }
    else if (type == SoundType::WAV)
    {
        PlaySound(NULL, 0, 0);
    }
}

SoundFile::SoundFile(std::string filename, SoundType type)
{
    this->filename = filename;
    this->type = type;
}

SoundFile::SoundFile()
{
    filename = "NULL";
    type = SoundType::INVALID;
}

std::string SoundFile::getFilename() const
{
    return filename;
}

Mp3File::Mp3File(std::string filename) :
    SoundFile(filename, SoundType::MP3)
{
    hasVariant = false;
}

Mp3File::Mp3File(std::string filename, std::string variant):
    SoundFile(filename, SoundType::MP3)
{
    hasVariant = true;
    this->variant = variant;
}

Mp3File::Mp3File():
    SoundFile()
{

}

std::string Mp3File::getFilename() const
{
    if (hasVariant && useVariant())
    {
        return variant;
    }
    else
    {
        return SoundFile::getFilename();
    }
}

bool Mp3File::useVariant() const
{
    return (random(0, 1) == 0);
}

void Mp3File::play() const
{
    std::string fullFilename = getFilename() + ".mp3";

    std::string file = "open \"" + fullFilename + "\" type mpegvideo alias mp3";
    mciSendString(file.c_str(), NULL, 0, NULL);
    mciSendString("play mp3 repeat", NULL, 0, NULL);
}

void Mp3File::stop() const
{
    stopSound(SoundType::MP3);
}

WavFile::WavFile(std::string filename, bool loop, bool async):
    SoundFile(filename, SoundType::WAV)
{
    this->loop = loop;
    this->async = async;
}

WavFile::WavFile():
    SoundFile()
{
    loop = false;
    async = false;
}

void WavFile::play() const
{
    std::string fullFileName = getFilename() + ".wav";

    if (loop)
    {
        PlaySound(TEXT(fullFileName.c_str()), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    }
    else if (async)
    {
        PlaySound(TEXT(fullFileName.c_str()), NULL, SND_FILENAME | SND_ASYNC);
    }
    else
    {
        PlaySound(TEXT(fullFileName.c_str()), NULL, SND_FILENAME);
    }
}

void WavFile::stop() const
{
    stopSound(SoundType::WAV);
}