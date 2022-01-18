#pragma once
#include "SDL.h"
#include <iostream>
#include<cmath>
#include <list>
#include <vector>
float Sigmoid(float a, float y) {
    float ans = 1 / (1 + exp(y / a));
    return ans;
}
float projectX(float position[3]) {
    int Hight = 50;
    float Project;
    if (position[2] > 0) {
        Project = position[0] / position[2] * 640 + 320;
    }
    else {
        Project = 0;
    }
    return Project;
}
float projectY(float position[3]) {
    int Hight = 50;
    float Project;
    if (position[2] > 0) {
        Project = Hight + 0.7*position[1] / position[2] * 480;
    }
    else {
        Project = 0;
    }
    return Project;
}

class Player {
public:
    float angleX, angleY;
    float position[3];
    Player() {
        position[0] = 0;
        position[1] = 0;
        position[2] = 0;
        angleX = 0;
        angleY = 0;
    }
    void Move(float x, float y, float z) {
        position[0] += cos(angleX) * x + sin(angleX) * z;
        position[1] += y;
        position[2] += cos(angleX) * z - sin(angleX) * x;
    }
    void Rotate(float Ax, float Ay) {
        angleX += pow((Ax-320)/3000, 3);
        angleY += pow((Ay - 320) / 2000, 3);
    }
};


class Point {
public:
    float position[3];
    void Fill(float pos0, float pos1, float pos2) {
        // Position to the center of mass.
        position[0] = pos0;
        position[1] = pos1;
        position[2] = pos2;
    }
    void MovePoint(Player* player) {
        position[0] += player->position[0];
        position[1] += player->position[1];
        position[2] += player->position[2];

    }
    void Multiplication(float rotation[3][3], float Centro[3]) {
        float ans[3] = { 0, 0, 0 };
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ans[i] += rotation[i][j] * (Centro[j] - position[j]);
            }
        }
        for (int i = 0; i < 3; i++) { position[i] = ans[i] + Centro[i]; }

    }

    void RotationX(float angle, float Centro[3]) {
        float rotation[3][3] = { {1, 0, 0}, {0, cos(angle), -sin(angle)}, {0, sin(angle), cos(angle)} };
        Multiplication(rotation, Centro);
    }
    void RotationY(float angle, float Centro[3]) {
        float rotation[3][3] = { {cos(angle), 0, sin(angle)}, {0, 1, 0}, {-sin(angle), 0, cos(angle)} };
        Multiplication(rotation, Centro);
    }

    void RotationZ(float angle, float Centro[3]) {
        float rotation[3][3] = { {cos(angle), -sin(angle), 0}, {sin(angle), cos(angle), 0}, {0, 0, 1} };
        Multiplication(rotation, Centro);

    }
};


class Cube {
public:
    Point pt1;
    Point pt2;
    Point pt3;
    Point pt4;
    Point pt5;
    Point pt6;
    Point pt7;
    Point pt8;
    Point centro;

    Cube(Point point1, Point point2, Player* player) {
        pt1 = point1;
        pt2 = point2;
        pt1.MovePoint(player);
        pt2.MovePoint(player);
        float a = abs(pt1.position[0]-pt2.position[0]);
        float b = abs(pt1.position[1] - pt2.position[1]);
        float c = abs(pt1.position[2] - pt2.position[2]);
        centro.position[0] = pt1.position[0] + (a / 2);
        centro.position[1] = pt1.position[1] + (b / 2);
        centro.position[2] = pt1.position[2] + (c / 2);
        pt3.Fill(pt1.position[0]+a, pt1.position[1], pt1.position[2]);
        pt4.Fill(pt1.position[0], pt1.position[1]+b, pt1.position[2]);
        pt5.Fill(pt1.position[0], pt1.position[1], pt1.position[2]+c);
        pt6.Fill(pt2.position[0] - a, pt2.position[1], pt2.position[2]);
        pt7.Fill(pt2.position[0], pt2.position[1]-b, pt2.position[2]);
        pt8.Fill(pt2.position[0], pt2.position[1], pt2.position[2]-c);
    }
    void RenderCube(SDL_Renderer* renderer, Player* player) {
        float proyection1[2] = { projectX(pt1.position), projectY(pt1.position) };
        float proyection2[2] = { projectX(pt2.position), projectY(pt2.position) };
        float proyection3[2] = { projectX(pt3.position), projectY(pt3.position) };
        float proyection4[2] = { projectX(pt4.position), projectY(pt4.position) };
        float proyection5[2] = { projectX(pt5.position), projectY(pt5.position) };
        float proyection6[2] = { projectX(pt6.position), projectY(pt6.position) };
        float proyection7[2] = { projectX(pt7.position), projectY(pt7.position) };
        float proyection8[2] = { projectX(pt8.position), projectY(pt8.position) };
        SDL_RenderDrawLine(renderer, proyection1[0], proyection1[1], proyection3[0], proyection3[1]);
        SDL_RenderDrawLine(renderer, proyection1[0], proyection1[1], proyection4[0], proyection4[1]);
        SDL_RenderDrawLine(renderer, proyection1[0], proyection1[1], proyection5[0], proyection5[1]);
        SDL_RenderDrawLine(renderer, proyection2[0], proyection2[1], proyection6[0], proyection6[1]);
        SDL_RenderDrawLine(renderer, proyection2[0], proyection2[1], proyection7[0], proyection7[1]);
        SDL_RenderDrawLine(renderer, proyection2[0], proyection2[1], proyection8[0], proyection8[1]);
        SDL_RenderDrawLine(renderer, proyection3[0], proyection3[1], proyection7[0], proyection7[1]);
        SDL_RenderDrawLine(renderer, proyection4[0], proyection4[1], proyection6[0], proyection6[1]);
        SDL_RenderDrawLine(renderer, proyection4[0], proyection4[1], proyection8[0], proyection8[1]);
        SDL_RenderDrawLine(renderer, proyection5[0], proyection5[1], proyection7[0], proyection7[1]);
        SDL_RenderDrawLine(renderer, proyection3[0], proyection3[1], proyection8[0], proyection8[1]);
        SDL_RenderDrawLine(renderer, proyection5[0], proyection5[1], proyection6[0], proyection6[1]);

    }
    void RotateX(float angle, float Centro[3]) {
        pt1.RotationX(angle, Centro);
        pt2.RotationX(angle, Centro);
        pt3.RotationX(angle, Centro);
        pt4.RotationX(angle, Centro);
        pt5.RotationX(angle, Centro);
        pt6.RotationX(angle, Centro);
        pt7.RotationX(angle, Centro);
        pt8.RotationX(angle, Centro);
    }
    void RotateY(float angle, float Centro[3]) {
        pt1.RotationY(angle, Centro);
        pt2.RotationY(angle, Centro);
        pt3.RotationY(angle, Centro);
        pt4.RotationY(angle, Centro);
        pt5.RotationY(angle, Centro);
        pt6.RotationY(angle, Centro);
        pt7.RotationY(angle, Centro);
        pt8.RotationY(angle, Centro);
    }
    void RotateZ(float angle, float Centro[3]) {
        pt1.RotationZ(angle, Centro);
        pt2.RotationZ(angle, Centro);
        pt3.RotationZ(angle, Centro);
        pt4.RotationZ(angle, Centro);
        pt5.RotationZ(angle, Centro);
        pt6.RotationZ(angle, Centro);
        pt7.RotationZ(angle, Centro);
        pt8.RotationZ(angle, Centro);
    }

    ~Cube() {}
};