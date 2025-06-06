#include "PlaylistIO.h"
#include <fstream>
#include <sstream>

std::vector<Playlist> loadPlaylists(const std::string &filename) {
    std::vector<Playlist> playlists;
    std::ifstream file(filename);
    if (!file.is_open()) return playlists;

    std::string line;
    Playlist current;
    while (getline(file, line)) {
        if (line.empty()) {
            if (!current.getName().empty()) playlists.push_back(current);
            current = Playlist();
        }
        else if (line.rfind("Ratings:", 0) == 0) {
            std::stringstream ss(line.substr(8));
            std::string val;
            while (getline(ss, val, ',')) {
                current.addRating(std::stof(val));
            }
        }
        else if (line.find('|') != std::string::npos) {
            std::stringstream ss(line);
            std::string title, artist, dur;
            getline(ss, title, '|');
            getline(ss, artist, '|');
            getline(ss, dur);
            current.addSong(Song(title, artist, std::stof(dur)));
        }
        else {
            current.setName(line);
        }
    }
    if (!current.getName().empty()) playlists.push_back(current);
    return playlists;
}

void savePlaylists(const std::vector<Playlist> &playlists, const std::string &filename) {
    std::ofstream file(filename);
    for (const auto &p : playlists) {
        file << p.getName() << "\n";
        for (const auto &s : p.getSongs()) {
            file << s.getTitle() << "|" << s.getArtist() << "|" << s.getDuration() << "\n";
        }
        file << "Ratings:";
        const auto& ratings = p.getRatings();
        for (size_t i = 0; i < ratings.size(); ++i) {
            file << ratings[i];
            if (i < ratings.size() - 1) file << ",";
        }
        file << "\n\n";
    }
}