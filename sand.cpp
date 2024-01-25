
#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 450;

class Point{
public:
    int x, y;
    Point(int a, int b){x=a;y=b;}
};

std::vector<Point> points;
std::vector<Point> floorVector(screenWidth, {0, screenHeight});
std::vector<Color> colours= {RED, GREEN, BLUE, BLACK};
int colorPtr = 0;

int main(void)
{
    points.push_back({0,450});
    for(int i=0;i<screenWidth;i++){
        floorVector[i].x=i;
    }
    InitWindow(screenWidth, screenHeight, "Falling Sand");

    float brushSize = 10.0f;
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    SetTargetFPS(120);              
    while (!WindowShouldClose())    
    {

        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        EndTextureMode();
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            colorPtr=(colorPtr+1)%colours.size();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)|| (GetGestureDetected() == GESTURE_DRAG))
        {
            BeginTextureMode(target);
            DrawRectangle((int)mousePos.x, (int)mousePos.y, brushSize, brushSize, colours[colorPtr]);
            Point p = Point(mousePos.x, mousePos.y);
            points.push_back(p);
            EndTextureMode();
        }

        for(int i=0;i<points.size();i++){
            if(points[i].y<floorVector[points[i].x].y-1){
                points[i].y++;
            }
           else{
                floorVector[points[i].x].y--;
                points.erase(points.begin()+i);
                i--;
            }
        }

        for(Point point:points){
            BeginTextureMode(target);
            DrawRectangle(point.x, point.y, brushSize, brushSize, colours[colorPtr]);
            EndTextureMode();
        }
        for(int i=0;i<screenWidth;i++){
            BeginTextureMode(target);
            for(int j=floorVector[i].y;j<450;j++)
                DrawRectangle(floorVector[i].x, j, brushSize, brushSize, colours[colorPtr]);
            EndTextureMode();
        }
        
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

        EndDrawing();
    }
    UnloadRenderTexture(target);    

    CloseWindow();              
    return 0;
}
//compile command: g++ sand.cpp -o sand -lraylib  && ./sand
