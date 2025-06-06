#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "PlaylistIO.h"

std::string colorText(const std::string& text, int colorCode) {
    return "\033[1;" + std::to_string(colorCode) + "m" + text + "\033[0m";
}

std::string formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes
        << ":" 
        << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

int main(int argc, char *argv[]) {
    const std::string filename = "playlists.txt";
    std::vector<Playlist> playlists = loadPlaylists(filename);

    if (argc < 2 || std::string(argv[1]) == "help") {
        std::cout << colorText("Playlist Manager Commands:\n", 34);
        std::cout << colorText("  create_playlist <name>       Create a new playlist\n", 34);
        std::cout << colorText("  delete_playlist <name>       Delete a playlist\n",34);
        std::cout << colorText("  list_playlists               List all playlists sorted by average rating\n",34);
        std::cout << colorText("  show_songs <playlist>        Show songs in a playlist\n",34);
        std::cout << colorText("  help                         Show this help message\n", 34);
        return 0;
    }

    std::string command = argv[1];

    if (command == "create_playlist" && argc == 3) {
        std::string newName = argv[2];
        auto it = std::find_if(playlists.begin(), playlists.end(), [&](const Playlist &p) {
            return p.getName() == newName;
        });
        if (it != playlists.end()) {
            std::cout << colorText("Error: A playlist with that name already exists.\n", 31);
            return 1;
        }
        Playlist newPlaylist;
        newPlaylist.setName(newName);
        playlists.push_back(newPlaylist);
        savePlaylists(playlists, filename);
        std::cout << colorText("Playlist created.\n",32);
    }
    else if (command == "delete_playlist" && argc == 3) {
        playlists.erase(std::remove_if(playlists.begin(), playlists.end(), [&](const Playlist &p) {
            return p.getName() == argv[2];
        }), playlists.end());
        savePlaylists(playlists, filename);
        std::cout << colorText("Playlist deleted.\n", 32);
    }
    else if (command == "list_playlists") {
        std::sort(playlists.begin(), playlists.end(), [](const Playlist &a, const Playlist &b) {
            return a.averageRating() > b.averageRating();
        });
        for (const auto &p : playlists) {
            std::cout << colorText(p.getName(), 34) << " (Avg Rating: " << std::fixed << std::setprecision(2) << p.averageRating() << ")\n";
        }
    }
    else if (command == "show_songs" && argc == 3) {
        std::string playlistName = argv[2];
        bool found = false;
        for (const auto& p : playlists) {
            if (p.getName() == playlistName) {
                found = true;
                std::cout << "Songs in playlist \"" << p.getName() << "\":\n";
                const auto& songs = p.getSongs();
                for (size_t i = 0; i < songs.size(); ++i) {
                    const auto& s = songs[i];
                    std::cout << i << ": " << colorText(s.getTitle(), 34) << " by " << s.getArtist()
                              << " (" << colorText(formatDuration(static_cast<int>(s.getDuration())), 35) << ")\n";
                }
                if (songs.empty()) {
                    std::cout << "This playlist is empty.\n";
                }
                break;
            }
        }
        if (!found) {
            std::cout << colorText("Error: Playlist not found.\n",31);
        }
    }
    else {
        std::cout << colorText("Invalid command.\n",31);
    }

    return 0;
}