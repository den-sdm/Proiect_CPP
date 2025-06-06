#include "Playlist.h"

void Playlist::setName(const std::string& newName) {
    name = newName;
}

const std::string& Playlist::getName() const {
    return name;
}

void Playlist::addSong(const Song& song) {
    songs.push_back(song);
}

void Playlist::removeSong(size_t index) {
    if (index < songs.size()) songs.erase(songs.begin() + index);
}

const std::vector<Song>& Playlist::getSongs() const {
    return songs;
}

void Playlist::addRating(float rating) {
    ratings.push_back(rating);
}

const std::vector<float>& Playlist::getRatings() const {
    return ratings;
}

float Playlist::averageRating() const {
    if (ratings.empty()) return 0.0f;
    float sum = 0;
    for (float r : ratings) sum += r;
    return sum / ratings.size();
}
