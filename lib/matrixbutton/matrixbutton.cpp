#include "matrixbutton.h"

const int row = 3;
const int colo = 3;

String control[row][colo] = {
    {"0", "TIEN", "0"},
    {"TRAI", "OK", "PHAI"},
    {"0", "XUONG", "0"},
};

const int rows[row] = {13, 12, 11};
const int colos[colo] = {10, 9, 46};

void setupMatrix(){
    for(int r = 0; r < row; r++){
        pinMode(rows[r], OUTPUT);
        digitalWrite(rows[r], HIGH);
    }
    for(int c = 0; c < colo; c++){
        pinMode(colos[c], INPUT_PULLUP);
    }
}
String scan(){
    for(int r = 0; r < row; r++){
        digitalWrite(rows[r], LOW);
        for(int c = 0; c < colo; c++){
            if(digitalRead(colos[c]) == LOW){
                digitalWrite(rows[r], HIGH);
                return control[r][c];
            }
        }
        digitalWrite(rows[r], HIGH);
    }
    return "0";
}
