#include <iostream>
#include <sstream>
#include <algorithm>
#include "PlaylistIO.h"

std::string colorText(const std::string& text, int colorCode) {
    return "\033[1;" + std::to_string(colorCode) + "m" + text + "\033[0m";
}

int main(int argc, char *argv[]) {
    const std::string filename = "playlists.txt";
    std::vector<Playlist> playlists = loadPlaylists(filename);

    if (argc < 2 || std::string(argv[1]) == "help") {
        std::cout << colorText("Playlist Interaction Commands:\n", 34);
        std::cout << colorText("  add_song <playlist> <title|artist|duration>    Add a song\n", 34);
        std::cout << colorText("  remove_song <playlist> <index>                 Remove song by index\n", 34);
        std::cout << colorText("  rate_playlist <playlist> <rating>              Rate a playlist (1.0 - 5.0)\n",34);
        std::cout << colorText("  help                                           Show this help message\n", 34);
        return 0;
    }

    std::string command = argv[1];

    if (command == "add_song" && argc == 4) {
        std::string playlistName = argv[2];
        std::stringstream ss(argv[3]);
        std::string title, artist, durStr;
        getline(ss, title, '|');
        getline(ss, artist, '|');
        getline(ss, durStr);

        bool found = false;
        for (auto &p : playlists) {
            if (p.getName() == playlistName) {
                found = true;
                auto songExists = std::any_of(p.getSongs().begin(), p.getSongs().end(), [&](const Song &s) {
                    return s.getTitle() == title && s.getArtist() == artist;
                });
                if (songExists) {
                    std::cout << colorText("Error: Song already exists in the playlist.\n", 31);
                    return 1;
                }
                p.addSong(Song(title, artist, std::stof(durStr)));
                break;
            }
        }
        if (!found) {
            std::cout << colorText("Error: Playlist not found.\n", 31);
            return 1;
        }
        savePlaylists(playlists, filename);
        std::cout << colorText("Song added.\n", 32);
    }
    else if (command == "remove_song" && argc == 4) {
    std::string playlistName = argv[2];
    int index = std::stoi(argv[3]);
    bool found = false;

    for (auto &p : playlists) {
        if (p.getName() == playlistName) {
            found = true;
            auto songs = p.getSongs();  // get a copy or const reference
            if (index >= 0 && index < static_cast<int>(songs.size())) {
                p.removeSong(index);
                savePlaylists(playlists, filename);
                std::cout << colorText("Song removed.\n", 32);  // Green
            } else {
                std::cerr << colorText("Error: Invalid song index. Playlist has " 
                    + std::to_string(songs.size()) + " songs. Index strats from 0 if there are songs.\n", 31);  // Red
            }
            break;
        }
    }

    if (!found) {
        std::cerr << colorText("Error: Playlist not found.\n", 31);  // Red
    }
}

    else if (command == "rate_playlist" && argc == 4) {
        std::string playlistName = argv[2];
        float rating = std::stof(argv[3]);
        if (rating < 1.0f || rating > 5.0f) {
            std::cout << colorText("Invalid rating. Must be between 1.0 and 5.0\n", 31);
            return 1;
        }
        for (auto &p : playlists) {
            if (p.getName() == playlistName) {
                p.addRating(rating);
                break;
            }
        }
        savePlaylists(playlists, filename);
        std::cout << colorText("Playlist rated.\n", 32);
    }
    else {
        std::cout << colorText("Invalid command.\n", 31);
    }

    return 0;
}
