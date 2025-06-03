# Proiect_CPP

Example inputs

# Create playlists
./playlist_manager create_playlist "Chill Vibes"
./playlist_manager create_playlist "Workout Mix"

# Add songs
./playlist_interact add_song "Chill Vibes" "Sunset|Artist A|3.5"
./playlist_interact add_song "Chill Vibes" "Moonlight|Artist B|4.2"
./playlist_interact add_song "Workout Mix" "Beast Mode|Artist X|2.8"

# Rate playlists
./playlist_interact rate_playlist "Chill Vibes" 4.5
./playlist_interact rate_playlist "Workout Mix" 5.0
./playlist_interact rate_playlist "Workout Mix" 4.0

# List playlists (should show Workout Mix first due to higher average)
./playlist_manager list_playlists

# Remove a song (0-based index)
./playlist_interact remove_song "Chill Vibes" 0

# Delete a playlist
./playlist_manager delete_playlist "Chill Vibes"
