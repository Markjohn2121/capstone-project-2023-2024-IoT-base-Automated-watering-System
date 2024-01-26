#include <Arduino.h>
void playMarioSound(MarioSound sound) {
  int melody[20];         // Placeholder for melody (adjust the size based on the maximum number of notes in a melody)
  int noteDurations[20];  // Placeholder for note durations (adjust the size based on the maximum number of notes in a melody)
  int size = 0;           // Size of the melody and note durations arrays

  switch (sound) {
    case SUPER_MARIO:
      {
        int superMarioMelody[] = { 959 + 1500, 959 + 1500, 0, 959 + 1500, 0, 823 + 1500, 959 + 1500, 0, 1083 + 1500, 0 };
        int superMarioNoteDurations[] = { 120, 120, 120, 120, 120, 120, 120, 120, 120, 120 };
        size = sizeof(superMarioMelody) / sizeof(superMarioMelody[0]);
        memcpy(melody, superMarioMelody, size * sizeof(int));
        memcpy(noteDurations, superMarioNoteDurations, size * sizeof(int));
        break;
      }
    case POWER_UP:
      {
        int powerUpMelody[] = { 659 + 1500, 783 + 1500, 987 + 1500 };
        int powerUpNoteDurations[] = { 150, 150, 300 };
        size = sizeof(powerUpMelody) / sizeof(powerUpMelody[0]);
        memcpy(melody, powerUpMelody, size * sizeof(int));
        memcpy(noteDurations, powerUpNoteDurations, size * sizeof(int));
        break;
      }
    case GAME_OVER:
      {
        int gameOverMelody[] = { 392 + 1500, 0, 392 + 1500, 0, 392 + 1500, 0, 311 + 1500, 0, 466 + 1500, 0, 392 + 1500 };
        int gameOverNoteDurations[] = { 150, 150, 150, 150, 150, 300 };
        size = sizeof(gameOverMelody) / sizeof(gameOverMelody[0]);
        memcpy(melody, gameOverMelody, size * sizeof(int));
        memcpy(noteDurations, gameOverNoteDurations, size * sizeof(int));
        break;
      }
    case FLAGPOLE:
      {
        int flagpoleMelody[] = { 523, 659, 783, 987 };
        int flagpoleNoteDurations[] = { 150, 150, 150, 300 };
        size = sizeof(flagpoleMelody) / sizeof(flagpoleMelody[0]);
        memcpy(melody, flagpoleMelody, size * sizeof(int));
        memcpy(noteDurations, flagpoleNoteDurations, size * sizeof(int));
        break;
      }
    case STOMP:
      {
        int stompMelody[] = { 196, 261 };
        int stompNoteDurations[] = { 100, 100 };
        size = sizeof(stompMelody) / sizeof(stompMelody[0]);
        memcpy(melody, stompMelody, size * sizeof(int));
        memcpy(noteDurations, stompNoteDurations, size * sizeof(int));
        break;
      }
    case COIN:
      {
        int coinMelody[] = { 823 + 1900, 959 + 1900 };
        int coinNoteDurations[] = { 100, 100 };
        size = sizeof(coinMelody) / sizeof(coinMelody[0]);
        memcpy(melody, coinMelody, size * sizeof(int));
        memcpy(noteDurations, coinNoteDurations, size * sizeof(int));
        break;
      }
    case JUMP:
      {
        int jumpMelody[] = { 1084, 1287 };
        int jumpNoteDurations[] = { 150, 150 };
        size = sizeof(jumpMelody) / sizeof(jumpMelody[0]);
        memcpy(melody, jumpMelody, size * sizeof(int));
        memcpy(noteDurations, jumpNoteDurations, size * sizeof(int));
        break;
      }
    case BRICK_BREAK:
      {
        int brickBreakMelody[] = { 523 + 1500, 587 + 1500 };
        int brickBreakNoteDurations[] = { 150, 150 };
        size = sizeof(brickBreakMelody) / sizeof(brickBreakMelody[0]);
        memcpy(melody, brickBreakMelody, size * sizeof(int));
        memcpy(noteDurations, brickBreakNoteDurations, size * sizeof(int));
        break;
      }
    case PIPE_ENTER:
      {
        int pipeEnterMelody[] = { 392 + 1500, 523 + 1500 };
        int pipeEnterNoteDurations[] = { 150, 150 };
        size = sizeof(pipeEnterMelody) / sizeof(pipeEnterMelody[0]);
        memcpy(melody, pipeEnterMelody, size * sizeof(int));
        memcpy(noteDurations, pipeEnterNoteDurations, size * sizeof(int));
        break;
      }
    case POWER_DOWN:
      {
        int powerDownMelody[] = { 392 + 1500, 293 + 1500 };
        int powerDownNoteDurations[] = { 150, 150 };
        size = sizeof(powerDownMelody) / sizeof(powerDownMelody[0]);
        memcpy(melody, powerDownMelody, size * sizeof(int));
        memcpy(noteDurations, powerDownNoteDurations, size * sizeof(int));
        break;
      }

    case ERROR:
      {
        int coinMelody[] = { 500, 600 };
        int coinNoteDurations[] = { 80, 100 };
        size = sizeof(coinMelody) / sizeof(coinMelody[0]);
        memcpy(melody, coinMelody, size * sizeof(int));
        memcpy(noteDurations, coinNoteDurations, size * sizeof(int));
        break;
      }
  }

  for (int i = 0; i < size; i++) {
    if (melody[i] == 0) {
      delay(noteDurations[i]);
    } else {
      ledcWriteTone(LEDC_CHANNEL, melody[i]);
      delay(noteDurations[i]);
    }
    ledcWrite(LEDC_CHANNEL, 0);
    delay(50);
  }
}
