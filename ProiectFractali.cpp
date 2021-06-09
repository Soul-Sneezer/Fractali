#include <iostream>
#include <unordered_map>
#include <stack>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;

struct objInfo
{
    int x;
    int y;
    double theta;
};

struct Fractal
{
    string axiom;
    unordered_map<char, string> rules;
    double length = 100;
    double theta;
    int x1 = 0;
    int y1 = 0;
    int iterations = 1;
    int aproxScaling = 1;
    olc::Pixel col = olc::BLACK;
    olc::Pixel secondaryCol = olc::WHITE;

    string CreateLString(string axiom, unordered_map<char, string> rules, int iterations)
    {
        string output = "";
        for (int i = 0; i < iterations; i++)
        {
            if (output != "")
                output = "";
            for (int i = 0; i < axiom.size(); i++)
            {
                if (rules[axiom[i]] != "")
                {
                    output += rules[axiom[i]];
                }
                else
                {
                    output += axiom[i];
                }
            }
            axiom = output;
        }

        return output;
    }

    virtual void DrawFractal(olc::PixelGameEngine* cge) = 0;

    void DecrementTheta()
    {
        theta -= 0.01;
    }

    void IncrementTheta()
    {
        theta += 0.01;
    }
};

struct SierpinskiTriangle : public Fractal
{
    SierpinskiTriangle()
    {
        theta = 60;
        length = 700;
        x1 = 0;
        y1 = 50;
    }
    
    void DrawFractal(olc::PixelGameEngine*cge) override
    {
        DrawSierpinskiTriangle(cge, x1, y1, length, iterations);
    }

    void DrawSierpinskiTriangle(olc::PixelGameEngine* cge, double x1, double y1, double sideLength, int iterations)
    {
        if (iterations > 0)
        {
            if(iterations == this->iterations)
                x1 = (cge->ScreenWidth() - sideLength) / 2.0;
            double angle = theta * 3.1415926535 / 180.0;
            double x2 = x1 + sideLength * cos(angle);
            double y2 = y1 + sideLength * sin(angle);
            double x3 = x1 + sideLength * cos(0);
            double y3 = y1 + sideLength * sin(0);

            if(iterations == this->iterations)
            cge->FillTriangle((int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3, col);

            double nx1 = (x1 + x2) / 2.0;
            double nx2 = (x2 + x3) / 2.0;
            double nx3 = (x3 + x1) / 2.0;

            double ny1 = (y1 + y2) / 2.0;
            double ny2 = (y2 + y3) / 2.0;
            double ny3 = (y3 + y1) / 2.0;
            cge->FillTriangle((int)nx1, (int)ny1, (int)nx2, (int)ny2, (int)nx3, (int)ny3, secondaryCol);
            iterations--;

            sideLength /= 2.0;
            DrawSierpinskiTriangle(cge, x1, y1, sideLength, iterations);
            DrawSierpinskiTriangle(cge, nx1, ny1, sideLength, iterations);
            DrawSierpinskiTriangle(cge, nx3, ny3, sideLength, iterations);
        }
        
        return;
    }
};

struct KochIsland : public Fractal
{
    KochIsland()
    {
        axiom = "F-F-F-F";
        rules['F'] = "F-F+F+FF-F-F+F";
        theta = 90.0;
        length = 300;
        x1 = 0;
        y1 = 0;
        aproxScaling = 4;
    }
    
    void DrawFractal(olc::PixelGameEngine* cge) override
    {
        double x2, y2, length1;
        x2 = x1;
        y2 = y1;
        x2 += (cge->ScreenWidth() - length) * 0.5;
        y2 += (cge->ScreenHeight() - length) * 0.5;
 
        length1 = length / (pow(aproxScaling, iterations));
        double currentAngle = 0;
        string LString = CreateLString(this->axiom, this->rules, this->iterations);

        for (int i = 0; i < LString.size(); i++)
        {
            if (LString[i] == 'F')
            {
                double nx1 = x2 + length1 * cos(currentAngle);
                double ny1 = y2 + length1 * sin(currentAngle);

                int outputx1, outputy1, outputx2, outputy2;

                cge->DrawLine(x2, y2, nx1, ny1, col);
                x2 = nx1;
                y2 = ny1;
            }
            else if (LString[i] == 'f')
            {
                x2 = x2 + length1 * cos(currentAngle); 
                y2 = y2 + length1 * sin(currentAngle);
            }
            else if (LString[i] == '-')
            {
                currentAngle += theta * 3.1415926535 / 180.0;
            }
            else
            {
                currentAngle -= theta * 3.1415926535 / 180.0;
            }
        }
    }
};

struct Fractal2 : public KochIsland
{
    Fractal2()
    {
        axiom = "F-F-F-F";
        rules['F'] = "F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F";
        theta = 90.0;
        length = 300;
        x1 = 0;
        y1 = 0;
        aproxScaling = 6;
    }
};

struct Fractal3 : public KochIsland
{
    Fractal3()
    {
        axiom = "F";
        rules['F'] = "F+F-F-F+F";
        theta = 90.0;
        length = 600;
        x1 = 0;
        y1 = 400;
        aproxScaling = 3;
    }
};

struct Fractal4 : public KochIsland
{
    Fractal4()
    {
        axiom = "F+F+F+F";
        rules['F'] = "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF";
        rules['f'] = "ffffff";
        theta = 90.0;
        length = 200;
        x1 = 0;
        y1 = 200;
        aproxScaling = 6;
    }
};

struct Fractal5 : public KochIsland
{
    Fractal5()
    {
        axiom = "F-F-F-F";
        rules['F'] = "FF-F-F-F-F-F+F";
        theta = 90.0;
        length = 300;
        x1 = 100;
        y1 = 0;
        aproxScaling = 3;
    }
};

struct Fractal6 : public KochIsland
{
    Fractal6()
    {
        axiom = "F-F-F-F";
        rules['F'] = "FF-F-F-F-FF";
        theta = 90.0;
        length = 300;
        aproxScaling = 3;
    }
};

struct Fractal7 : public KochIsland
{
    Fractal7()
    {
        axiom = "F-F-F-F";
        rules['F'] = "FF-F+F-F-FF";
        theta = 90.0;
        length = 100;
        aproxScaling = 2;
    }
};

struct Fractal8 : public KochIsland
{
    Fractal8()
    {
        axiom = "F-F-F-F";
        rules['F'] = "FF-F--F-F";
        theta = 90.0;
        length = 300;
        aproxScaling = 3;
    }
};

struct Fractal9 : public KochIsland
{
    Fractal9()
    {
        axiom = "F-F-F-F";
        rules['F'] = "F-FF--F-F";
        theta = 90.0;
        length = 100;
        aproxScaling = 2;
    }
};

struct Fractal10 : public KochIsland
{
    Fractal10()
    {
        axiom = "F-F-F-F";
        rules['F'] = "F-F+F-F-F";
        theta = 90.0;
        length = 200;
        aproxScaling = 2;
    }
};

struct Fractal11 : public Fractal
{
    Fractal11()
    {
        axiom = "L";
        rules['L'] = "L+R+";
        rules['R'] = "-L-R";
        theta = 90.0;
        length = 100;
        x1 = 0;
        y1 = 0;
        aproxScaling = 1;
    }

    void DrawFractal(olc::PixelGameEngine* cge) override
    {
        double x2, y2, length1;
        x2 = x1;
        y2 = y1;
        x2 += (cge->ScreenWidth() - length) * 0.5;
        y2 += (cge->ScreenHeight() - length) * 0.5;

        length1 = length / (pow(aproxScaling, iterations));
        double currentAngle = 0;
        string LString = CreateLString(this->axiom, this->rules, this->iterations);

        for (int i = 0; i < LString.size(); i++)
        {
            if (LString[i] == 'L')
            {
                double nx1 = x2 + length1 * cos(currentAngle);
                double ny1 = y2 + length1 * sin(currentAngle);
                cge->DrawLine(x2, y2, nx1, ny1, col);
                x2 = nx1;
                y2 = ny1;
            }
            else if (LString[i] == 'R')
            {
                double nx1 = x2 + length1 * cos(currentAngle);
                double ny1 = y2 + length1 * sin(currentAngle);
                cge->DrawLine(x2, y2, nx1, ny1, col);
                x2 = nx1;
                y2 = ny1;
            }
            else if (LString[i] == '-')
            {
                currentAngle += theta * 3.1415926535 / 180.0;
            }
            else
            {
                currentAngle -= theta * 3.1415926535 / 180.0;
            }
        }
    }
};

struct Fractal12 : public Fractal11
{
    Fractal12()
    {
        axiom = "R";
        rules['L'] = "R+L+R";
        rules['R'] = "L-R-L";
        theta = 60.0;
        length = 300;
        x1 = 0;
        y1 = 0;
        aproxScaling = 2;
    }
};

struct Fractal13 : public Fractal11
{
    Fractal13()
    {
        axiom = "L";
        rules['L'] = "L+R++R-L--LL-R+";
        rules['R'] = "-L+RR++R+L--L-R";
        theta = 60.0;
        length = 200;
        x1 = 0;
        y1 = 0;
        aproxScaling = 2;
    }
};

struct Fractal14 : public Fractal
{
    Fractal14()
    {
        axiom = "F";
        rules['F'] = "F[+F]F[-F]F";
        theta = 25.7;
        length = 300;
        x1 = 100;
        y1 = 500;
        aproxScaling = 3;
    }

    void DrawFractal(olc::PixelGameEngine* cge) override
    {
        stack<objInfo> states;        
        double x2, y2, length1;
        x2 = x1;
        y2 = y1;
        x2 += (cge->ScreenWidth() - length) * 0.5;
        y2 += cge->ScreenHeight();

        length1 = length / (pow(aproxScaling, iterations));
        double currentAngle = 0;
        string LString = CreateLString(this->axiom, this->rules, this->iterations);


        for (int i = 0; i < LString.size(); i++)
        {
            if (LString[i] == 'F' || LString[i] == 'X')
            {
                double nx1 = x2 - length1 * sin(currentAngle);
                double ny1 = y2 - length1 * cos(currentAngle);
                if (i + 1 < LString.size() && LString[i + 1] == ']')
                    DrawFrunza(cge,nx1, ny1, currentAngle, olc::GREEN, iterations, true);
                else
                    col = olc::Pixel(150, 120, 10);
                cge->DrawLine(x2, y2, nx1, ny1, col);
                x2 = nx1;
                y2 = ny1;
            }
            else if (LString[i] == '-')
            {
                currentAngle += theta * 3.1415926535 / 180.0;
            }
            else if(LString[i] == '+')
            {
                currentAngle -= theta * 3.1415926535 / 180.0;
            }
            else if (LString[i] == '[')
            {
                objInfo nObj;
                nObj.x = x2;
                nObj.y = y2;
                nObj.theta = currentAngle;
                states.push(nObj);
            }
            else if (LString[i] == ']')
            {
                x2 = states.top().x;
                y2 = states.top().y;
                currentAngle = states.top().theta;
                states.pop();
            }
        }
    }

    void DrawFrunza(olc::PixelGameEngine *cge, int x, int y, double currentAngle, olc::Pixel col, int iterations, bool scalable)
    {
        double scale = 1.0;
        if (scalable == true)
            scale = pow(0.9, iterations);
        int nx = x - (length / 100.0 * sin(currentAngle + 3.1415926535 / 2.0)) * scale;
        int nx2 = x + (length / 100.0 * sin(currentAngle + 3.1415926535 / 2.0)) * scale;
        int ny = y - (length / 100.0 * cos(currentAngle + 3.1415926535 / 2.0)) * scale;
        int ny2 = y + (length / 100.0 * cos(currentAngle + 3.1415926535 / 2.0)) * scale;
        int nx3 = x - (length / 30.0 * sin(currentAngle)) * scale;
        int ny3 = y - (length / 30.0 * cos(currentAngle)) * scale;
        cge->FillCircle(x, y, (length / 100.0) * scale, col);
        cge->FillTriangle(nx, ny, nx2, ny2, nx3, ny3, col);
    }
};

struct Fractal15 : public Fractal14
{
    Fractal15()
    {
        axiom = "F";
        rules['F'] = "F[+F]F[-F][F]";
        theta = 20.0;
        length = 300;
        x1 = 100;
        y1 = 400;
        aproxScaling = 2;
    }
};

struct Fractal16 : public Fractal14
{
    Fractal16()
    {
        axiom = "F";
        rules['F'] = "FF-[-F+F+F]+[+F-F-F]";
        theta = 22.5;
        length = 300;
        x1 = 100;
        y1 = 400;
        aproxScaling = 2;
    }
};

struct Fractal17 : public Fractal14
{
    Fractal17()
    {
        axiom = "X";
        rules['X'] = "F[+X]F[-X]+X";
        rules['F'] = "FF";
        theta = 20.0;
        length = 300;
        x1 = 100;
        y1 = 400;
        aproxScaling = 2;
    }
};

struct Fractal18 : public Fractal14
{
    Fractal18()
    {
        axiom = "X";
        rules['X'] = "F[+X][-X]FX";
        rules['F'] = "FF";
        theta = 25.7;
        length = 300;
        x1 = 100;
        y1 = 400;
        aproxScaling = 2;
    }
};

struct Fractal19 : public Fractal14
{
    Fractal19()
    {
        axiom = "X";
        rules['X'] = "F-[[X]+X]+F[+FX]-X";
        rules['F'] = "FF";
        theta = 22.5;
        length = 300;
        x1 = 100;
        y1 = 400;
        aproxScaling = 2;
    }
};

struct Fractal20 : public Fractal
{
    Fractal20()
    {
        axiom = "A";
        rules['A'] = "F[+A][-A]";
        rules['F'] = "0";
        rules['0'] = "1";
        rules['1'] = "2";
        rules['2'] = "3";
        rules['3'] = "4";
        rules['4'] = "5";
        rules['5'] = "6";
        rules['6'] = "7";
        rules['7'] = "8";
        rules['8'] = "9";
        theta = 85.0;
    }

    void DrawFractal(olc::PixelGameEngine* cge) override
    {
        stack<objInfo> states;
        int depth = 0;
        double x2, y2, length1;
        x2 = x1;
        y2 = y1;
        x2 += (cge->ScreenWidth() - length) * 0.5;
        y2 += (cge->ScreenHeight() - length) * 0.5;

        length1 = length / (pow(aproxScaling, iterations));
        double currentAngle = 0;
        string LString = CreateLString(this->axiom, this->rules, this->iterations);

        for (int i = 0; i < LString.size(); i++)
        {
            if (LString[i] - '0' >= 0 && LString[i] - '0' < 10)
            {
                int c = length1;
                if (LString[i] - '0' >= 0 && LString[i] - '0' < 10);
                {
                    length1 *= pow(1.5, LString[i] - '0');
                }
                double nx1 = x2 - length1 * sin(currentAngle);
                double ny1 = y2 - length1 * cos(currentAngle);

                cge->DrawLine(x2, y2, nx1, ny1, col);
                x2 = nx1;
                y2 = ny1;
                length1 = c;
            }
            else if (LString[i] == '-')
            {
                currentAngle += theta * 3.1415926535 / 180.0;
            }
            else if (LString[i] == '+')
            {
                currentAngle -= theta * 3.1415926535 / 180.0;
            }
            else if (LString[i] == '[')
            {
                depth++;
                objInfo nObj;
                nObj.x = x2;
                nObj.y = y2;
                nObj.theta = currentAngle;
                states.push(nObj);
            }
            else if (LString[i] == ']')
            {
                x2 = states.top().x;
                y2 = states.top().y;
                currentAngle = states.top().theta;
                states.pop();
            }
        }
    }
};

class FractalPlants : public olc::PixelGameEngine
{
public:
    FractalPlants()
    {
        sAppName = "Proiect - Fractali";
    }

private:
    Fractal* fractal = nullptr;
    float offsetX = 0;
    float offsetY = 0;
    int index = 0;
    bool lSystem = false;

    float startPanX = 0.0f;
    float startPanY = 0.0f;

    olc::Pixel backgroundCol = olc::WHITE;

public:
    virtual bool OnUserCreate() override
    {
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        float mouseX = (float)GetMouseX();
        float mouseY = (float)GetMouseY();

        if (GetMouse(2).bPressed)
        {
            startPanX = mouseX;
            startPanY = mouseY;
        }

        if (GetMouse(2).bHeld)
        {
            offsetX += (mouseX - startPanX);
            offsetY += (mouseY - startPanY);

            startPanX = mouseX;
            startPanY = mouseY;
        }

        Clear(backgroundCol);

        if (lSystem)
        {
            switch (index)
            {
            case 0:
                fractal = new SierpinskiTriangle();
                lSystem = false;
                break;
            case 1:
                fractal = new KochIsland();
                lSystem = false;
                break;
            case 2:
                fractal = new Fractal2();
                lSystem = false;
                break;
            case 3:
                fractal = new Fractal3();
                lSystem = false;
                break;
            case 4:
                fractal = new Fractal4();
                lSystem = false;
                break;
            case 5:
                fractal = new Fractal5();
                lSystem = false;
                break;
            case 6:
                fractal = new Fractal6();
                lSystem = false;
                break;
            case 7:
                fractal = new Fractal7();
                lSystem = false;
                break;
            case 8:
                fractal = new Fractal8();
                lSystem = false;
                break;
            case 9:
                fractal = new Fractal9();
                lSystem = false;
                break;
            case 10:
                fractal = new Fractal10();
                lSystem = false;
                break;
            case 11:
                fractal = new Fractal11();
                lSystem = false;
                break;
            case 12:
                fractal = new Fractal12();
                lSystem = false;
                break;
            case 13:
                fractal = new Fractal13();
                lSystem = false;
                break;
            case 14:
                fractal = new Fractal14();
                lSystem = false;
                break;
            case 15:
                fractal = new Fractal15();
                lSystem = false;
                break;
            case 16:
                fractal = new Fractal16();
                lSystem = false;
                break;
            case 17:
                fractal = new Fractal17();
                lSystem = false;
                break;
            case 18:
                fractal = new Fractal18();
                lSystem = false;
                break;
            case 19:
                fractal = new Fractal19();
                lSystem = false;
                break;
            case 20:
                fractal = new Fractal20();
                lSystem = false;
                break;
            }
        }

        if (GetKey(olc::Key::L).bPressed)
        {
            lSystem = true;
        }

        if (index < 20 && GetKey(olc::Key::EQUALS).bPressed)
        {
            index++;
            offsetX = 0;
            offsetY = 0;
            lSystem = true;
        }

        if (index > 0 && GetKey(olc::Key::MINUS).bPressed)
        {
            index--;
            offsetX = 0;
            offsetY = 0;
            lSystem = true;
        }

        if (GetKey(olc::Key::E).bPressed)
        {
            fractal = nullptr;
            lSystem = false;
        }

        if (fractal != nullptr && GetKey(olc::Key::RIGHT).bPressed)
        {
            fractal->iterations++;
        }
        
        if (fractal != nullptr && fractal->iterations > 0 && GetKey(olc::Key::LEFT).bPressed)
        {
            fractal->iterations--;
        }

        if (fractal != nullptr && GetKey(olc::Key::UP).bHeld)
        {
            fractal->length += 0.01 * fractal->length;
        }

        if (fractal != nullptr && GetKey(olc::Key::DOWN).bHeld)
        {
            fractal->length -= 0.01 * fractal->length;
        }

        if (fractal != nullptr && GetKey(olc::Key::W).bHeld)
        {
            fractal->IncrementTheta();
        }

        if (fractal != nullptr && GetKey(olc::Key::S).bHeld)
        {
            fractal->DecrementTheta();
        }

        if (GetKey(olc::Key::B).bPressed)
        {
            if (backgroundCol == olc::WHITE)
                backgroundCol = olc::BLACK;
            else
                backgroundCol = olc::WHITE;

        }

        if (fractal != nullptr && fractal->length < 0)
            fractal->length = 1;

        // Draw
        if (fractal != nullptr)
        {
            if (backgroundCol == olc::BLACK)
            {
                fractal->col = olc::WHITE;
                fractal->secondaryCol = olc::BLACK;
            }
            else
            {
                fractal->col = olc::BLACK;
                fractal->secondaryCol = olc::WHITE;
            }
            fractal->x1 = offsetX;
            fractal->y1 = offsetY;
            fractal->DrawFractal(this);
            DrawString(50, 50, "Iterations: " + to_string(fractal->iterations), olc::RED);
            DrawString(50, 70, "Current theta: " + to_string(fractal->theta), olc::RED);
        }
        DrawString(50, 60, "Current fractal number: " + to_string(index), olc::RED);

        return true;
    }
};

int main()
{
    FractalPlants proj;
    if (proj.Construct(1280, 720, 1, 1, false))
        proj.Start();

    return 0;
}