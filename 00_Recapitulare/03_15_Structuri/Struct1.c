// fisierul contine info pentru cate o structura Product, pe o linie, cu virgula ca separator
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LINESIZE 128

typedef struct Song Song;

struct Song
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
};

Song initSong(int trackId, char* title, char* artist, float royalityPerPlay)
{
    Song song;

    song.trackId = trackId;
    song.title = (char*)malloc(strlen(title) + 1);
    strcpy(song.title, title);
    song.artist = (char*)malloc(strlen(artist) + 1);
    strcpy(song.artist, artist);
    song.royalityPerPlay = royalityPerPlay;

    return song;
}

void printSong(Song song)
{
    printf("----------\nSong ID: %d\nSong Title: %s\nSong Artist: %s\nSong Royality: %.3f\n----------",
           song.trackId, song.title, song.artist, song.royalityPerPlay);
}

Song readSongFromFile(FILE* f)
{
    Song song;

    char buffer[LINESIZE];
    char* endptr = NULL; // stocheaza restul stringului (pentru strtol, strtof)
    char* songData = NULL; // stocheaza tokenul la fiecare apel al lui strtok

    fgets(buffer, LINESIZE, f); // citeste din f un char (buffer), de dim LINESIZE

    songData = strtok(buffer, ","); // sparge linia in token-uri cu ajutorul sep-ului
    song.trackId = (int)strtol(songData, &endptr, 10); // str to long

    songData = strtok(NULL, ",");
    song.title = (char*)malloc(strlen(songData) + 1);
    strcpy(song.title, songData);

    songData = strtok(NULL, ",");
    song.artist = (char*)malloc(strlen(songData) + 1);
    strcpy(song.artist, songData);

    songData = strtok(NULL, ",");
    song.royalityPerPlay = (float)strtof(songData, &endptr);

    return song;
}

Song* readSongsFromFile(char* fileName, int* songsNo)
{
    FILE* f = fopen(fileName, "r");

    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);

    *songsNo = (int)strtol(buffer, &endptr, 10);

    Song* songs = (Song*)malloc(sizeof(Song) * (*songsNo));
    for(int i = 0; i < (*songsNo); i++)
    {
        songs[i] = readSongFromFile(f);
    }
    fclose(f);

    return songs;
}

int main()
{
    int songsNo;
    Song* songs = readSongsFromFile("songs.txt", &songsNo);

    for(int i = 0; i < songsNo; i++)
    {
        printSong(songs[i]);
    }
}

