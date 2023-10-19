#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <math.h>
#include<SFML/Audio.hpp>
#include<SFML/Audio/SoundBuffer.hpp>
#include<map>

#define PI 3.14159265358979323846 

using namespace sf;

char   DTMF_char[17] = "123a456b789c*0#d";

char opred(int* fr, const int mode)
{
    if (mode == 6) {
        std::string konk = std::to_string(fr[0]) + std::to_string(fr[1]);
        if (konk == "10" || konk == "01") {
            printf("1\n");
            return '1';
        }
        if (konk == "20" || konk == "02") {
            printf("2\n");
            return '2';
        }
        if (konk == "12" || konk == "21") {
            printf("3\n");
            return '3';
        }
        if (konk == "30" || konk == "03") {
            printf("4\n");
            return '4';
        }
        if (konk == "13" || konk == "31") {
            printf("5\n");
            return '5';
        }
        if (konk == "23" || konk == "32") {
            printf("6\n");
            return '6';
        }
        if (konk == "40" || konk == "04") {
            printf("7\n");
            return '7';
        }
        if (konk == "14" || konk == "41") {
            printf("8\n");
            return '8';
        }
        if (konk == "24" || konk == "42") {
            printf("9\n");
            return '9';
        }
        if (konk == "34" || konk == "43") {
            printf("0\n");
            return '0';
        }
        if (konk == "25" || konk == "52") {
            printf("N\n");
            return 'N';
        }
        if (konk == "35" || konk == "53") {
            printf("P\n");
            return 'P';
        }
        printf("X\n");
        return 'X';
    }
    else {
        return DTMF_char[4*fr[0] + fr[1] - 4];
    }
    
}


char gertz(sf::Int16* samples, int sampleCount, int* number, int* K, const int mode) {

    long double W;
    long double cosine;
    long double Q0, Q1, Q2;
    std::map < long double, int > map;
    std::map < int , long double> mp;
    std::map <long double, int> ::iterator it = map.begin();
    std::map < int, long double> ::iterator it2 = mp.begin();

    for (int j = 0; j < mode; j++)
    {
        W = (2.0 * PI * number[j]) / sampleCount;
        cosine = cos(W);

        Q0 = 0;
        Q1 = 0;
        Q2 = 0;

        for (int i = 0; i < sampleCount; i++)
        {
            Q0 = (double)samples[i] + (2 * cos(W) * Q1) - Q2;
            Q2 = Q1;
            Q1 = Q0;
        }
        long double magn = Q1 * Q1 - 2 * cosine * Q1 * Q2 + Q2 * Q2;
        map.insert(std::make_pair(magn, j));
    }

    advance(it, -2);
    int fr[2];
    for (int i = 0; it != map.end(); i++, it++) { 
        mp.insert(std::make_pair(it->second, it->first));
        //fr[i] = it->second;
    }
    advance(it2, -2);
    for (int i = 0; it2 != mp.end(); i++, it2++) {
        fr[i] = it2->first;
    }
    return opred(fr, mode);
   
}


void main() {
    
    int ANI[6] = { 700, 900, 1100, 1300, 1500, 1700 };
    int DTMF[8] = { 697,  770,  852,  941, 1209, 1336, 1477, 1633};

    const int mode = 6;
    int fragm = 40;
    std::string res = "D_";
    char result;

    sf::SoundBuffer Buffer;
    Buffer.loadFromFile("ANI_3_C.wav");
    const sf::Int16* input = Buffer.getSamples();
    int sampleRate = Buffer.getSampleRate();
    sf::Uint64 sampleCountRe = Buffer.getSampleCount();
    int sampleCount = sampleCountRe / fragm;
    for (int n = 0; n < fragm; n++)
    {
        sf::Int16* samples = (sf::Int16*)calloc(sizeof(sf::Int16),  sampleCount);
        for (int i = 0; i < sampleCount; i++)
            samples[i] = input[i + n*sampleCount];
        int number[mode];
        for (int i = 0; i < mode; i++)
            number[i] = ANI[i] * sampleCount / sampleRate;
        //printf("%d - nabor\n\n", n);
        result = gertz(samples, sampleCount, number, ANI, mode);
        //printf("\n\n");
        if (res[res.size() - 1] != result)
            res = res + result;
        free(samples);
    }
    std::cout << res;
}


