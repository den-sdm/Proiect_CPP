#ifndef PLAYLIST_IO_H
#define PLAYLIST_IO_H

#include <vector>
#include <string>
#include "Playlist.h"

std::vector<Playlist> loadPlaylists(const std::string &filename);
void savePlaylists(const std::vector<Playlist> &playlists, const std::string &filename);

#endif