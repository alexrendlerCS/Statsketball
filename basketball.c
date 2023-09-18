#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrayz.h"

#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50
#define MAX_GAMES 100

void parsePlayerData(char *line, char *name, long (*stats)[3], int *numGames) {
    char *token;
    token = strtok(line, "[]");
    strcpy(name, token);
    token = strtok(NULL, "[]");
    *numGames = 0;
    while (token != NULL) {
        if (sscanf(token, "%ld,%ld,%ld", &stats[*numGames][0], &stats[*numGames][1], &stats[*numGames][2]) == 3) {
            (*numGames)++;
        }
        token = strtok(NULL, "[]");
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: Expected 1 command-line argument\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open the file\n");
        return 1;
    }

    char line[1000];
    char names[MAX_PLAYERS][MAX_NAME_LENGTH];
    long stats[MAX_PLAYERS][MAX_GAMES][3];
    int numPlayers = 0;
    int numGames[MAX_PLAYERS];

    while (fgets(line, sizeof(line), file) != NULL && numPlayers < MAX_PLAYERS) {
        line[strcspn(line, "\n")] = '\0';
        parsePlayerData(line, names[numPlayers], stats[numPlayers], &numGames[numPlayers]);
        numPlayers++;
    }

    fclose(file);

    if (numPlayers == 0) {
        fprintf(stderr, "Error: No player data found in the file\n");
        return 1;
     }
    // POINTS SCORED VARIABLES
    int mostConsistentScorer = 0;
    int mostInconsistentScorer = 0;
    int highestScorer = 0;
    int lowestScorer = 0;

    double sdevScores[MAX_PLAYERS];
    double avgScores[MAX_PLAYERS];

    // REBOUND VARIABLES
    int mostInconsistentRebounder = 0;
    int mostConsistentRebounder = 0;
    int highestRebounder = 0;
    int lowestRebounder = 0;

    double sdevRebounds[MAX_PLAYERS];
    double avgRebounds[MAX_PLAYERS];

	
    // ASSIST VARIABLES
    int mostInconsistentAssister = 0;
    int mostConsistentAssister = 0;
    int highestAssister = 0;
    int lowestAssister = 0;

    double sdevAssists[MAX_PLAYERS];
    double avgAssists[MAX_PLAYERS];

    // POINTS SCORED LOOP
    for (int i = 0; i < numPlayers; i++) {
        long playerScores[MAX_GAMES + 1];
        int numTotalGames = numGames[i];

        // Copy the player scores to a temporary array
        for (int j = 0; j < numTotalGames; j++) {
            playerScores[j + 1] = stats[i][j][0];
        }
	playerScores[0] = numTotalGames;


        sdevScores[i] = sdev(playerScores);
        avgScores[i] = mean(playerScores);

        if (sdevScores[i] < sdevScores[mostConsistentScorer])
            mostConsistentScorer = i;
        if (sdevScores[i] > sdevScores[mostInconsistentScorer])
            mostInconsistentScorer = i;
        if (avgScores[i] > avgScores[highestScorer])
            highestScorer = i;
        if (avgScores[i] < avgScores[lowestScorer])
            lowestScorer = i;
    }
    
    // REBOUNDS LOOP
    for (int i = 0; i < numPlayers; i++) {
        long playerRebounds[MAX_GAMES + 1];
        int numTotalGames = numGames[i];

        for (int j = 0; j < numTotalGames; j++) {
            playerRebounds[j + 1] = stats[i][j][1]; // Rebound values are stored
        }

        playerRebounds[0] = numTotalGames;
        sdevRebounds[i] = sdev(playerRebounds);
        avgRebounds[i] = mean(playerRebounds);
        if (sdevRebounds[i] < sdevRebounds[mostConsistentRebounder])
            mostConsistentRebounder = i;
        if (sdevRebounds[i] > sdevRebounds[mostInconsistentRebounder])
            mostInconsistentRebounder = i;
        if (avgRebounds[i] > avgRebounds[highestRebounder])
            highestRebounder = i;
        if (avgRebounds[i] < avgRebounds[lowestRebounder])
            lowestRebounder = i;
    }
    
    // ASSISTS LOOP
    for (int i = 0; i < numPlayers; i++) {
        long playerAssists[MAX_GAMES + 1];
        int numTotalGames = numGames[i];

        for (int j = 0; j < numTotalGames; j++) {
            playerAssists[j + 1] = stats[i][j][2]; // Assist values are stored
        }

        playerAssists[0] = numTotalGames;
        sdevAssists[i] = sdev(playerAssists);
        avgAssists[i] = mean(playerAssists);
        if (sdevAssists[i] < sdevAssists[mostConsistentAssister])
            mostConsistentAssister = i;
        if (sdevAssists[i] > sdevAssists[mostInconsistentAssister])
            mostInconsistentAssister = i;
        if (avgAssists[i] > avgAssists[highestAssister])
            highestAssister = i;
        if (avgAssists[i] < avgAssists[lowestAssister])
            lowestAssister = i;
    }


    // POINTS PRINT
    printf("most consistent scorer: %s\n", names[mostConsistentScorer]);
    printf("most inconsistent scorer: %s\n", names[mostInconsistentScorer]);
    printf("highest scorer: %s\n", names[highestScorer]);
    printf("lowest scorer: %s\n", names[lowestScorer]);
     
    // REBOUNDS PRINT
    printf("most inconsistent rebounder: %s\n", names[mostInconsistentRebounder]);
    printf("most consistent rebounder: %s\n", names[mostConsistentRebounder]);
    printf("highest rebounder: %s\n", names[highestRebounder]);
    printf("lowest rebounder: %s\n", names[lowestRebounder]);

    // ASSISTS PRINT
    printf("most inconsistent assister: %s\n", names[mostInconsistentAssister]);
    printf("most consistent assister: %s\n", names[mostConsistentAssister]);
    printf("lowest assister: %s\n", names[lowestAssister]);
    printf("highest assister: %s\n", names[highestAssister]);

    return 0;
}

