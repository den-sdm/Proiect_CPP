#ifndef SONG_H
#define SONG_H

#include <string>

class Song {
private:
    std::string title;
    std::string artist;
    float duration;

public:
    Song(const std::string& title, const std::string& artist, float duration);

    const std::string& getTitle() const;
    const std::string& getArtist() const;
    float getDuration() const;
};

#endif