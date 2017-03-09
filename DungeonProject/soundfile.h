#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <string>

enum class SoundType
{
    MP3, WAV, INVALID
};

class SoundFile
{
public:
    SoundFile(std::string filename, SoundType type);
    SoundFile();

    virtual void play() const = 0;
    virtual void stop() const = 0;

    virtual std::string getFilename() const;

private:
    std::string filename;
    SoundType type;
};

class Mp3File : public SoundFile
{
public:
    Mp3File(std::string filename);
    Mp3File(std::string filename, std::string variant);
    Mp3File();

    virtual void play() const override;
    void stop() const override;
    std::string getFilename() const override;

private:
    std::string variant;
    bool hasVariant;
    bool useVariant() const;
};

class WavFile : public SoundFile
{
public:
    WavFile(std::string filename, bool loop, bool async);
    WavFile();

    void play() const override;
    void stop() const override;

private:
    bool loop;
    bool async;
};

void playSound(SoundFile& sound);
void stopSound(SoundType type);

#endif