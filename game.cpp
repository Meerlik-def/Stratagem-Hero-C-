#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <cstdio>

using namespace std;

// COLORS 
void setColor(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clearScreen() {
    system("cls");
}

// UTILS 
void animatedPrint(const string& text, int color = 7, int delay = 25) {
    setColor(color);
    for (char c : text) {
        cout << c;
        Sleep(delay);
    }
    cout << "\n";
    setColor(7);
}

// JINGLES 
void playStartupJingle() {
    Beep(880, 180); Beep(988, 180); Beep(1046, 200);
    Beep(1318, 160); Beep(1174, 160); Beep(1046, 200);
    Beep(880, 180); Beep(659, 220); Beep(880, 260);
}

void playNormalJingle() {
    Beep(660, 150); Beep(880, 150); Beep(990, 180); Beep(880, 180);
}

void playHellJingle() {
    Beep(400, 200); Beep(500, 200); Beep(600, 200); Beep(500, 200); Beep(400, 250);
}

void playEndlessHellJingle() {
    Beep(350, 200); Beep(450, 200); Beep(550, 200); Beep(650, 200);
}

void playTrainingJingle() {
    Beep(700, 150); Beep(800, 150); Beep(900, 150);
}

void playUltraHellJingle() {
    Beep(300, 200); Beep(450, 200); Beep(600, 200); Beep(750, 200);
}

void playExitJingle() {
    Beep(500, 200); Beep(420, 200); Beep(330, 250);
}

void playFailMelody() {
    Beep(600, 200); Beep(500, 200); Beep(400, 250); Beep(330, 300); Beep(220, 350);
}

void stageSound() {
    Beep(900, 120); Beep(1100, 120); Beep(1300, 150);
}

void playMegaJingle() {
    int notes1[] = { 660, 880, 990, 1320, 1180, 990, 880, 740, 660, 550 };
    int durations1[] = { 300, 300, 300, 300, 300, 300, 300, 300, 300, 300 };
    for (int i = 0; i < 10; i++) Beep(notes1[i], durations1[i]);
    for (int i = 0; i < 10; i++) Beep(880 + (i % 3) * 120, 300);
}

void playMenuMusic() {
    Beep(700, 120); Beep(900, 120); Beep(1100, 150); Beep(900, 150); Beep(700, 180);
}

//  LOGIC 
char randomArrow() {
    return "UDLR"[rand() % 4];
}

string arrowToText(char c) {
    switch (c) {
    case 'U': return "^";
    case 'D': return "v";
    case 'L': return "<";
    case 'R': return ">";
    }
    return "?";
}

char convertInput(char c) {
    if (c == 'w' || c == 'W') return 'U';
    if (c == 's' || c == 'S') return 'D';
    if (c == 'a' || c == 'A') return 'L';
    if (c == 'd' || c == 'D') return 'R';

    if (GetAsyncKeyState('W') & 0x8000) return 'U';
    if (GetAsyncKeyState('S') & 0x8000) return 'D';
    if (GetAsyncKeyState('A') & 0x8000) return 'L';
    if (GetAsyncKeyState('D') & 0x8000) return 'R';

    return '?';
}

// SECRET MESSAGES 
const vector<string> secretMessages = {
    "FOR DEMOCRACY AND LIBERTY!",
    "SUPER EARTH SALUTES YOUR SACRIFICE.",
    "YOU ARE A TRUE HELLDIVER.",
    "MISSION ACCOMPLISHED. GLORY TO SUPER EARTH.",
    "ONLY THE BRAVE REACH THIS POINT.",
    "YOU HAVE EARNED YOUR PLACE IN VALHALLA."
};

void showRandomSecrets() {
    vector<int> idx(secretMessages.size());
    for (int i = 0; i < (int)secretMessages.size(); i++) idx[i] = i;
    for (int i = 0; i < (int)idx.size(); i++) {
        int j = rand() % (int)idx.size();
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
    }
    clearScreen();
    setColor(14);
    cout << "=== SECRET HELLDIVERS MESSAGE ===\n\n";
    setColor(10);
    cout << secretMessages[idx[0]] << "\n";
    cout << secretMessages[idx[1]] << "\n\n";
    setColor(7);
    cout << "Press any key to continue...\n";
    (void)_getch();
}

// SCREEN SHAKE 
void screenShake() {
    for (int i = 0; i < 6; i++) {
        COORD pos;
        pos.X = (i % 2 == 0) ? 1 : 3;
        pos.Y = (i % 2 == 0) ? 1 : 3;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        setColor(12);
        cout << "!!!";
        this_thread::sleep_for(chrono::milliseconds(30));
        clearScreen();
    }
    setColor(7);
}

// HIGHSCORES 
struct Scores {
    int globalHigh;
    int normalHigh;
    int hellHigh;
    int endlessHigh;
    int trainingHigh;
    int ultraHigh;
    int medals;
};

bool loadScores(Scores& s) {
    FILE* f = fopen("scores.dat", "rb");
    if (!f) {
        s.globalHigh = 0;
        s.normalHigh = 0;
        s.hellHigh = 0;
        s.endlessHigh = 0;
        s.trainingHigh = 0;
        s.ultraHigh = 0;
        s.medals = 0;
        return false;
    }
    fread(&s, sizeof(Scores), 1, f);
    fclose(f);
    return true;
}

void saveScores(const Scores& s) {
    FILE* f = fopen("scores.dat", "wb");
    if (!f) return;
    fwrite(&s, sizeof(Scores), 1, f);
    fclose(f);
}

// GAME
enum Mode {
    MODE_NORMAL,
    MODE_HELL,
    MODE_ENDLESS_HELL,
    MODE_TRAINING,
    MODE_ULTRA_HELL
};

int main() {
    srand((unsigned)time(0));

    Scores scores;
    loadScores(scores);

    playStartupJingle();

    while (true) {
        // MENU
        clearScreen();
        playMenuMusic();

        setColor(11);
        cout <<
            "   +----------------------------------------------------------------------------------+\n"
            "   |                               STRATAGEM HERO V3                                  |\n"
            "   +----------------------------------------------------------------------------------+\n\n";

        animatedPrint("=== SELECT YOUR MISSION ===", 14, 15);
        cout << "\n";
        animatedPrint("[1] NORMAL", 10, 10);
        animatedPrint("[2] HELL DIVE", 12, 10);
        animatedPrint("[3] ENDLESS HELL", 13, 10);
        animatedPrint("[4] TRAINING SIMULATION", 11, 10);
        animatedPrint("[5] REAL HELL", 5, 10);
        cout << "\n";
        animatedPrint("[Q] RETURN TO SUPER EARTH", 7, 10);

        cout << "\n";
        setColor(14);
        cout << "Global High Score: " << scores.globalHigh << "\n";
        cout << "Gold Medals: " << scores.medals << "\n\n";
        setColor(7);

        char modeChar = _getch();
        if (modeChar == 'Q' || modeChar == 'q') {
            playExitJingle();
            abort();
            return 0;
        }

        Mode mode;
        int baseTime;
        int minTime = 1500;

        if (modeChar == '1') {
            mode = MODE_NORMAL;
            baseTime = 5000;
            playNormalJingle();
        }
        else if (modeChar == '2') {
            mode = MODE_HELL;
            baseTime = 2500;
            playHellJingle();
        }
        else if (modeChar == '3') {
            mode = MODE_ENDLESS_HELL;
            baseTime = 2500;
            minTime = 200;
            playEndlessHellJingle();
        }
        else if (modeChar == '4') {
            mode = MODE_TRAINING;
            baseTime = 5000;
            playTrainingJingle();
        }
        else if (modeChar == '5') {
            mode = MODE_ULTRA_HELL;
            baseTime = 500;
            minTime = 300;
            playUltraHellJingle();
        }
        else {
            continue;
        }

        int timeLimit = baseTime;
        int score = 0;
        int round = 1;

        while (true) {

            if (mode != MODE_TRAINING && round % 8 == 0) {
                stageSound();
                baseTime -= 1000;
                if (mode == MODE_ENDLESS_HELL) {
                    if (baseTime < 200) baseTime = 200;
                }
                else if (mode == MODE_ULTRA_HELL) {
                    if (baseTime < minTime) baseTime = minTime;
                }
                else {
                    if (baseTime < minTime) baseTime = minTime;
                }
                timeLimit = baseTime;
            }

            int len = 3 + (rand() % 6);
            vector<char> pattern(len);
            for (int i = 0; i < len; i++)
                pattern[i] = randomArrow();

            bool success = false;

            while (!success) {
                vector<char> input;
                input.reserve(len);

                auto start = chrono::high_resolution_clock::now();
                bool failed = false;

                while (true) {
                    clearScreen();

                    setColor(11);
                    cout << "=== STRATAGEM HERO 3.0 ===\n";
                    setColor(7);

                    cout << "Round: " << round << "\n";
                    cout << "Score: " << score << " | Global High: " << scores.globalHigh << "\n";
                    cout << "Gold Medals: " << scores.medals << "\n\n";

                    cout << "Mode: ";
                    switch (mode) {
                    case MODE_NORMAL:       cout << "NORMAL\n"; break;
                    case MODE_HELL:         cout << "HELL DIVE\n"; break;
                    case MODE_ENDLESS_HELL: cout << "ENDLESS HELL\n"; break;
                    case MODE_TRAINING:     cout << "TRAINING SIMULATION\n"; break;
                    case MODE_ULTRA_HELL:   cout << "REAL HELL\n"; break;
                    }
                    cout << "\n";

                    cout << "Stratagem: ";
                    for (int i = 0; i < len; i++) {
                        if (i < (int)input.size()) {
                            if (input[i] == pattern[i]) {
                                setColor(10);
                            }
                            else {
                                setColor(12);
                            }
                        }
                        else {
                            setColor(14);
                        }
                        cout << arrowToText(pattern[i]) << " ";
                    }
                    setColor(7);
                    cout << "\n";

                    auto now = chrono::high_resolution_clock::now();
                    int elapsed = (int)chrono::duration_cast<chrono::milliseconds>(now - start).count();
                    int remaining = timeLimit - elapsed;
                    if (remaining < 0) remaining = 0;

                    setColor(6);
                    cout << "Time: " << remaining / 1000.0 << " s\n\n";
                    setColor(7);

                    cout << "Input: ";
                    for (char c : input) {
                        cout << arrowToText(c) << " ";
                    }
                    cout << "\n";

                    if (remaining == 0) {
                        clearScreen();
                        setColor(12);
                        cout << "TIME OUT!\n";
                        setColor(7);
                        playFailMelody();
                        goto GAME_OVER;
                    }

                    if (_kbhit()) {
                        char raw = _getch();
                        char conv = convertInput(raw);
                        if (conv != '?') {
                            input.push_back(conv);
                            Beep(900, 40);

                            int idx = (int)input.size() - 1;
                            if (idx < len && input[idx] != pattern[idx]) {
                                failed = true;
                                screenShake();
                                break;
                            }

                            if (input.size() == pattern.size()) {
                                success = true;
                                break;
                            }
                        }
                    }

                    this_thread::sleep_for(chrono::milliseconds(30));
                }

                if (failed) {
                    Beep(300, 200);
                }
            }

            Beep(1200, 80);
            score++;

            if (score > scores.globalHigh) scores.globalHigh = score;

            switch (mode) {
            case MODE_NORMAL:       if (score > scores.normalHigh)   scores.normalHigh = score; break;
            case MODE_HELL:         if (score > scores.hellHigh)     scores.hellHigh = score; break;
            case MODE_ENDLESS_HELL: if (score > scores.endlessHigh)  scores.endlessHigh = score; break;
            case MODE_TRAINING:     if (score > scores.trainingHigh) scores.trainingHigh = score; break;
            case MODE_ULTRA_HELL:   if (score > scores.ultraHigh)    scores.ultraHigh = score; break;
            }

            if (score >= 999) {
                scores.medals++;
                score = 0;
                playMegaJingle();
                showRandomSecrets();
            }

            saveScores(scores);
            round++;
        }

    GAME_OVER:
        clearScreen();
        setColor(12);
        cout << "MISSION FAILED.\n";
        setColor(7);
        cout << "Score: " << score << "\n";
        cout << "Global High: " << scores.globalHigh << "\n";
        cout << "Gold Medals: " << scores.medals << "\n\n";

        cout << "Press any key to return to mission selection...\n";
        (void)_getch();
    }
}
