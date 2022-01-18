#pragma once
#include <deque>
#include <iostream>
void RotationTerrainX(float angle, float* vector[3]) {
    float rotation[3][3] = { {1, 0, 0}, {0, cos(angle), -sin(angle)}, {0, sin(angle), cos(angle)} };
}
void RotationTerrainY(float angle, float* vector[3]) {
    float rotation[3][3] = { {cos(angle), 0, sin(angle)}, {0, 1, 0}, {-sin(angle), 0, cos(angle)} };
}

void RotationTerrainZ(float angle, float* vector[3]) {
    float rotation[3][3] = { {cos(angle), -sin(angle), 0}, {sin(angle), cos(angle), 0}, {0, 0, 1} };

}
class Chunck {
public:
    int map[2];
    float jugador[3];
    int ancho = 100;
    float delta;
    int resolution;  // Esto es temporal, pertenece al terreno.
    std::list<Point> points;
    Point center;
    Chunck(Player* player, float Resolution, int x, int y) {
        map[0] = x;
        map[1] = y;
        center.Fill(player->position[0], player->position[1], player->position[2]);
        resolution = Resolution;
        delta = ancho / (resolution - 1);
    }
    ~Chunck(){}
    void GenerateChunck(float centro[3]) {
        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                Point point1;
                point1.Fill(centro[0] + ancho/2 - delta * i, 10 + centro[1] + rand() % 5, centro[2] - delta * j + ancho / 2);
                points.push_back(point1);
            }
        }
    }
    void RenderChunck(SDL_Renderer* renderer, Player* player) {
        auto iterador1 = points.begin();
        auto iterador2 = points.begin();
        std::advance(iterador2, 1);
        float temporal[3] = { player->position[0], player->position[1], player->position[2] };
        //This vectors will store the points to make the perpendicular lines.
        std::vector<Point> perp1;
        std::vector<Point> perp2;
        (*iterador1).MovePoint(player);
        for (int i = 0; i < points.size() - 1; i++) {
            if (i != 0) { std::advance(iterador1, 1); std::advance(iterador2, 1); }
            (*iterador2).MovePoint(player);
            if (i % resolution != resolution-1) {
                if (i < resolution) {
                    perp1.push_back(*iterador1);
                    perp2.push_back(*iterador1);
                }
                else {
                    perp1[i % resolution] = perp2[i % resolution];
                    perp2[i % resolution] = *iterador1;
                    float pt1[2] = { projectX(perp1[i % resolution].position), projectY(perp1[i % resolution].position) };
                    float pt2[2] = { projectX(perp2[i % resolution].position), projectY(perp2[i % resolution].position) };
                    SDL_RenderDrawLine(renderer, pt1[0], pt1[1], pt2[0], pt2[1]);
                }
                
                if ((*iterador1).position[2] > 0.1 && (*iterador2).position[2] > 0.1) {
                    float pt3[2] = { projectX((*iterador1).position), projectY((*iterador1).position) };
                    float pt4[2] = { projectX((*iterador2).position), projectY((*iterador2).position) };
                    SDL_RenderDrawLine(renderer, pt3[0], pt3[1], pt4[0], pt4[1]);

                }
            }
        }
    }
};
class Terrain {
public:
    std::deque<std::deque <Chunck>> loaded;
    int resolution;
    bool encontrado = 0;
    int curvature;
    int mid = 0;
    Terrain(int Resolution, int Curvature) {
        resolution = Resolution;
        curvature = Curvature;
    }
    void load(int X, int Y) {
        if (loaded.size() == 0) {
            return;
        }
        if (loaded.size() > X - loaded.at(0).at(0).map[0]) {
            std::deque<Chunck> revisar = loaded.at(X - loaded.at(0).at(0).map[0]);  //Can be a slower but makes the sintaxis much easier.
            // This list is going to be sorted so we can perform a binary search.
            int L = 0;
            int R = revisar.size() - 1;
            while (R - L > -1) {
                mid = L + int((R - L) / 2);
                if (Y == revisar[mid].map[1]) {
                    encontrado = 1;
                    break;
                }
                else if (Y > revisar[mid].map[1]) {
                    L = mid + 1;
                }
                else { R = mid - 1; }
            }
        }
        else { return; }
    }
    void CreateChuncks(Player* player, SDL_Renderer* renderer, int X, int Z) {
        float center[3];
        center[0] = X * 100;
        center[1] = 10;
        center[2] = Z * 100;
        Chunck actual(player, resolution, X, Z);
        actual.GenerateChunck(center);
        if (loaded.size() == 0) {
             std::deque<Chunck> hola;
             hola.push_back(actual);
             loaded.push_back(hola);
        }
        else if (loaded.size() <= X - loaded.at(0).at(0).map[0]) {
            if (X > loaded.at(0).at(0).map[0]) {
                std::deque<Chunck> hola;
                hola.push_back(actual);
                loaded.push_back(hola);
            }
            else {
                std::deque<Chunck> hola;
                hola.push_back(actual);
                loaded.push_front(hola);
            }
        }
        else {
            // Add the value acording to Z.
            if (loaded.at(X - loaded.at(0).at(0).map[0]).at(0).map[1] < Z) {
                loaded.at(X - loaded.at(0).at(0).map[0]).push_back(actual);
            }
            else {
                loaded.at(X - loaded.at(0).at(0).map[0]).push_front(actual);
            }
        }
        actual.RenderChunck(renderer, player);
    }
    void LoadChunck(Player* player, SDL_Renderer* renderer, int X, int Z) {
        Chunck actual(player, resolution, X, Z);
        actual.points = loaded.at(X-loaded.at(0).at(0).map[0]).at(mid).points;  // the long term is equal to the first value of X.
        actual.RenderChunck(renderer, player);
    }
    void ManageChuncks(Player* player, SDL_Renderer* renderer) {
        // This is the starting point of each Chunck
        int X = -int(player->position[0] / 100);
        int Z = -int(player->position[2] / 100);
        for (int i = 2; i > -2; i--) {
            for (int j = 0; j < 3; j++) {
                load(X + i, Z + j);
                if (encontrado == 0) {
                    CreateChuncks(player, renderer, X + i, Z + j);  // El bugg esta acá.
                }
                else {
                    LoadChunck(player, renderer, X + i, Z + j);
                    encontrado = 0;
                }
            }
        }
    }


    ~Terrain() {
    }
};