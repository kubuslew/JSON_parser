//Created by Jakub Lewandowski

#include <cstring>
#include<iostream>

using namespace std;

void showHelp() {
    cout << "h - pomoc" << endl;
    cout << "o <nazwa pliku> - otworz plik" << endl;
    cout << "c - zwaliduj otwarty plik" << endl;
    cout << "Jakub Lewandowski" <<endl;
}

FILE* openFile(char filename[]) {
    FILE* f = fopen(filename, "r+");
    if (f == NULL) {
        cout << "Nie znaleziono pliku o podanej nazwie" << endl;
    }
    else {
        cout << "Otwarto plik o sciezce " << filename << endl;
    }
    return f;
}

bool validateOtherToken(FILE* f, char currCharacter) {
    char otherToken[6];
    if (currCharacter == 't') {
        fread(otherToken, sizeof(char), 4, f);
        otherToken[4] = '\0';
        if (strcmp(otherToken, "true") == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (currCharacter == 'f') {
        fread(otherToken, sizeof(char), 5, f);
        otherToken[5] = '\0';
        if (strcmp(otherToken, "false") == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        fread(otherToken, sizeof(char), 4, f);
        otherToken[4] = '\0';
        if (strcmp(otherToken, "null") == 0) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool validateNumber(FILE* f, char currCharacter) {
    bool isNumber = false;
    for (int i = 0; i < 10; i++) {
        if (currCharacter == '0' + i) {
            isNumber = true;
        }
    }
    isNumber = isNumber || currCharacter == '-';
    if (!isNumber) {
        return false;
    }
    currCharacter = fgetc(f);
    if (currCharacter == '0') {
        return true;
    }
    isNumber = false;
    for (int i = 0; i < 9; i++) {
        if (currCharacter == '1' + i) {
            isNumber = true;
        }
    }
    if (!isNumber) {
        return false;
    }
    do {
        currCharacter = fgetc(f);
        isNumber = false;
        for (int i = 0; i < 10; i++) {
            if (currCharacter == '0' + i) {
                isNumber = true;
            }
        }
    } while (isNumber);
    return true;
}

bool validateString(FILE* f) {
    char currCharacter = '\0';
    do {
        currCharacter = fgetc(f);
        if (currCharacter == '\\') {
            currCharacter = fgetc(f);
            if (currCharacter != '\\' && currCharacter != '\"' && currCharacter != '\t') {
                cout << "Blad w znaku " << ftell(f) << endl;
                return false;
            }
            else {
                currCharacter = 'a';
            }
        }
    } while (currCharacter != '\"');
    return true;
}

char validateWS(FILE* f)
{
    char currCharacter = '\0';
    do {
        currCharacter = fgetc(f);
    } while (currCharacter == '\x20' || currCharacter == '\x09' || currCharacter == '\x0A' || currCharacter == '\x0D');
    return currCharacter;
}

bool validateValue(FILE* f)
{
    bool ret = true;
    char currCharacter = validateWS(f);
    switch (currCharacter) {
        case '"':
            ret = validateString(f);
            break;
            //case '{':
            //      ret = validateObject(f);
            //      break;
            //case '[':
            //      ret = validateArray(f);
            //      break;
        case 't':
        case 'f':
        case 'n':
            ret = validateOtherToken(f, currCharacter);
            break;
        default:
            ret = validateNumber(f, currCharacter);
            break;
    }
    currCharacter = validateWS(f);
    if (currCharacter != EOF) {
        cout << "Blad w znaku " << ftell(f) << endl;
        return false;
    }
    else {
        return true;
    }
}

bool validateJson(FILE* f)
{
    return validateValue(f);
}

void validateFile(FILE* f) {
    if (f == NULL) {
        cout << "Zaden plik nie zostal otwarty." << endl;
        return;
    };
    bool isValid = validateJson(f);
    if (isValid) {
        cout << "Poprawny plik" << endl;
    }
    else {
        cout << "Niepoprwany plik, blad w linijce " << ftell(f) << endl;
    }
}

int main() {
    char command;
    char filename[256];
    FILE* f = NULL;
    // main program loop
    while (1) {
        cout << "Wprowadz komende:" << endl;
        cin >> command;
        switch (command) {
            case 'h':
                showHelp();
                break;
            case 'o':
                // read filename starting from the third character in line
                cin >> filename;
                if (f != NULL) {
                    fclose(f);
                }
                f = openFile(filename);
                break;
            case 'c':
                validateFile(f);
                break;
            default:
                cout << "Nieznana komenda" << endl;
                cout << "h - wyœwietl pomoc" << endl;
                break;
        }
    }
}