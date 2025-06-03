// playlist_manager.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

struct Song
{
    std::string title;
    std::string artist;
    float duration;
};

struct Playlist
{
    std::string name;
    std::vector<Song> songs;
    std::vector<float> ratings;

    float averageRating() const
    {
        if (ratings.empty())
            return 0.0f;
        float sum = 0;
        for (float r : ratings)
            sum += r;
        return sum / ratings.size();
    }
};

std::vector<Playlist> loadPlaylists(const std::string &filename);
void savePlaylists(const std::vector<Playlist> &playlists, const std::string &filename);

int main(int argc, char *argv[])
{
    const std::string filename = "playlists.txt";
    std::vector<Playlist> playlists = loadPlaylists(filename);

    if (argc < 2 || std::string(argv[1]) == "help") {
        std::cout << "Playlist Manager Commands:\n";
        std::cout << "  create_playlist <name>       Create a new playlist\n";
        std::cout << "  delete_playlist <name>       Delete a playlist\n";
        std::cout << "  list_playlists               List all playlists sorted by average rating\n";
        std::cout << "  show_songs <playlist>        Show songs in a playlist\n";
        std::cout << "  help                         Show this help message\n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "create_playlist" && argc == 3)
    {
        std::string newName = argv[2];
        auto it = std::find_if(playlists.begin(), playlists.end(), [&](const Playlist &p)
                               { return p.name == newName; });
        if (it != playlists.end())
        {
            std::cout << "Error: A playlist with that name already exists.\n";
            return 1;
        }

        Playlist newPlaylist;
        newPlaylist.name = newName;
        playlists.push_back(newPlaylist);
        savePlaylists(playlists, filename);
        std::cout << "Playlist created.\n";
    }
    else if (command == "delete_playlist" && argc == 3)
    {
        playlists.erase(std::remove_if(playlists.begin(), playlists.end(), [&](const Playlist &p)
                                       { return p.name == argv[2]; }),
                        playlists.end());
        savePlaylists(playlists, filename);
        std::cout << "Playlist deleted.\n";
    }
    else if (command == "list_playlists")
    {
        std::sort(playlists.begin(), playlists.end(), [](const Playlist &a, const Playlist &b)
                  { return a.averageRating() > b.averageRating(); });
        for (const auto &p : playlists)
        {
            std::cout << p.name << " (Avg Rating: " << std::fixed << std::setprecision(2) << p.averageRating() << ")\n";
        }
    }
    else if (command == "show_songs" && argc == 3) {
        std::string playlistName = argv[2];
        bool found = false;
        for (const auto& p : playlists) {
            if (p.name == playlistName) {
                found = true;
                std::cout << "Songs in playlist \"" << p.name << "\":\n";
                for (size_t i = 0; i < p.songs.size(); ++i) {
                    const auto& s = p.songs[i];
                    std::cout << i << ": " << s.title << " by " << s.artist 
                              << " (" << s.duration << " mins)\n";
                }
                if (p.songs.empty()) {
                    std::cout << "This playlist is empty.\n";
                }
                break;
            }
        }
        if (!found) {
            std::cout << "Error: Playlist not found.\n";
        }
    }
    else
    {
        std::cout << "Invalid command.\n";
    }

    return 0;
}

std::vector<Playlist> loadPlaylists(const std::string &filename)
{
    std::vector<Playlist> playlists;
    std::ifstream file(filename);
    if (!file.is_open())
        return playlists;

    std::string line;
    Playlist current;
    while (getline(file, line))
    {
        if (line.empty())
        {
            playlists.push_back(current);
            current = Playlist();
        }
        else if (line.rfind("Ratings:", 0) == 0)
        {
            std::stringstream ss(line.substr(8));
            std::string val;
            while (getline(ss, val, ','))
            {
                current.ratings.push_back(std::stof(val));
            }
        }
        else if (line.find('|') != std::string::npos)
        {
            std::stringstream ss(line);
            std::string title, artist, dur;
            getline(ss, title, '|');
            getline(ss, artist, '|');
            getline(ss, dur);
            current.songs.push_back({title, artist, std::stof(dur)});
        }
        else
        {
            current.name = line;
        }
    }
    if (!current.name.empty())
        playlists.push_back(current);
    return playlists;
}

void savePlaylists(const std::vector<Playlist> &playlists, const std::string &filename)
{
    std::ofstream file(filename);
    for (const auto &p : playlists)
    {
        file << p.name << "\n";
        for (const auto &s : p.songs)
        {
            file << s.title << "|" << s.artist << "|" << s.duration << "\n";
        }
        file << "Ratings:";
        for (size_t i = 0; i < p.ratings.size(); ++i)
        {
            file << p.ratings[i];
            if (i < p.ratings.size() - 1)
                file << ",";
        }
        file << "\n\n";
    }
}