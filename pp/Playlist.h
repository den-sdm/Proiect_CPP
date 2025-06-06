#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <vector>
#include <string>
#include "Song.h"

class Playlist {
private:
    std::string name;
    std::vector<Song> songs;
    std::vector<float> ratings;

public:
    void setName(const std::string& newName);
    const std::string& getName() const;

    void addSong(const Song& song);
    void removeSong(size_t index);
    const std::vector<Song>& getSongs() const;

    void addRating(float rating);
    const std::vector<float>& getRatings() const;
    float averageRating() const;
};

#endif