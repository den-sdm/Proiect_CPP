#include "Song.h"

Song::Song(const std::string& title, const std::string& artist, float duration)
    : title(title), artist(artist), duration(duration) {}

const std::string& Song::getTitle() const {
    return title;
}

const std::string& Song::getArtist() const {
    return artist;
}

float Song::getDuration() const {
    return duration;
}