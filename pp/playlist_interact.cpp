// playlist_interact.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

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
};

std::vector<Playlist> loadPlaylists(const std::string &filename);
void savePlaylists(const std::vector<Playlist> &playlists, const std::string &filename);

int main(int argc, char *argv[])
{
    const std::string filename = "playlists.txt";
    std::vector<Playlist> playlists = loadPlaylists(filename);

    if (argc < 2 || std::string(argv[1]) == "help") {
        std::cout << "Playlist Interaction Commands:\n";
        std::cout << "  add_song <playlist> <title|artist|duration>    Add a song\n";
        std::cout << "  remove_song <playlist> <index>                 Remove song by index\n";
        std::cout << "  rate_playlist <playlist> <rating>              Rate a playlist (1.0 - 5.0)\n";
        std::cout << "  help                                           Show this help message\n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "add_song" && argc == 4)
    {
        std::string playlistName = argv[2];
        std::stringstream ss(argv[3]);
        std::string title, artist, durStr;
        getline(ss, title, '|');
        getline(ss, artist, '|');
        getline(ss, durStr);
        bool found = false;
        for (auto &p : playlists)
        {
            if (p.name == playlistName)
            {
                found = true;
                auto songExists = std::any_of(p.songs.begin(), p.songs.end(), [&](const Song &s)
                                              { return s.title == title && s.artist == artist; });
                if (songExists)
                {
                    std::cout << "Error: Song already exists in the playlist.\n";
                    return 1;
                }
                p.songs.push_back({title, artist, std::stof(durStr)});
                break;
            }
        }
        if (!found)
        {
            std::cout << "Error: Playlist not found.\n";
            return 1;
        }
        savePlaylists(playlists, filename);
        std::cout << "Song added.\n";
    }
    else if (command == "remove_song" && argc == 4)
    {
        std::string playlistName = argv[2];
        int index = std::stoi(argv[3]);
        for (auto &p : playlists)
        {
            if (p.name == playlistName && index >= 0 && index < (int)p.songs.size())
            {
                p.songs.erase(p.songs.begin() + index);
                break;
            }
        }
        savePlaylists(playlists, filename);
        std::cout << "Song removed.\n";
    }
    else if (command == "rate_playlist" && argc == 4)
    {
        std::string playlistName = argv[2];
        float rating = std::stof(argv[3]);
        if (rating < 1.0f || rating > 5.0f)
        {
            std::cout << "Invalid rating. Must be between 1.0 and 5.0\n";
            return 1;
        }
        for (auto &p : playlists)
        {
            if (p.name == playlistName)
            {
                p.ratings.push_back(rating);
                break;
            }
        }
        savePlaylists(playlists, filename);
        std::cout << "Playlist rated.\n";
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