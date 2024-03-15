#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Song Song;
typedef struct Node Node;

struct Song
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
};

struct Node
{
    Song song;
    Node* next;
};

void printSong(Song song)
{
    printf("-----\nSong ID: %d\nSong title: %sSong artist: %sRoyality per play: %.2f\n-----\n",
           song.trackId, song.title, song.artist, song.royalityPerPlay);
}

Song readSong(FILE* f)
{
    Song song;
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    song.trackId = (int)strtol(buffer, &endptr, 10);

    fgets(buffer, LINESIZE, f);
    song.title = (char*)malloc(strlen(buffer) + 1);
    strcpy(song.title, buffer);

    fgets(buffer, LINESIZE, f);
    song.artist = (char*)malloc(strlen(buffer) + 1);
    strcpy(song.artist, buffer);

    fgets(buffer, LINESIZE, f);
    song.royalityPerPlay = (float)strtof(buffer, &endptr);

    return song;
}

void readSongs(char* fileName, Song** songs, int* songsNo)
{
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    *songsNo = (int)strtol(buffer, &endptr, 10);

    *songs = malloc(sizeof(Song) * (*songsNo));
    for(int i = 0; i < (*songsNo); i++)
        (*songs)[i] = readSong(f);

}

void insertAtBeginning(Node** head, Song song)
{
    Node* newNode = malloc(sizeof(Node));

    newNode->song = song;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node** head, Song song)
{
    if(*head)
    {
        Node* aux = *head;
        while(aux->next)
            aux = aux->next;

        Node* newNode = malloc(sizeof(Node));
        newNode->song = song;
        newNode->next = NULL;

        aux->next = newNode;
    }
    else insertAtBeginning(head, song);
}

void parseList(Node* head)
{
    if(head)
    {
        while(head)
        {
            printSong(head->song);
            head = head->next;
        }
    }
    else
    {
        printf("List is empty!");
    }
}

int main()
{
    int songsNumber;
    Song* songsArray;
    Node* head = NULL;

    readSongs("playlist.txt", &songsArray, &songsNumber);

    for (int i = 0; i < songsNumber; i++)
        printSong(songsArray[i]);

    insertAtBeginning(&head, songsArray[0]);
    printf("\nLista 1\n");
    parseList(head);

    insertAtEnd(&head, songsArray[1]);
    printf("\nLista 2\n");
    parseList(head);
}