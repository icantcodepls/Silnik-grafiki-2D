#include "PrimitiveRenderer.h"

#include <cmath>
#include <SFML/Graphics.hpp>

#include <stack>

using namespace std;
using namespace sf;

void PrimitiveRenderer::clear(Image& image, Color color) {
    Image empty(image.getSize(), color);
    image=empty;
}

void PrimitiveRenderer::renderPoint(Image& bufor, unsigned int x, unsigned int y, Color color) {
    if (x<bufor.getSize().x && y<bufor.getSize().y) {
        bufor.setPixel({x,y}, color);
    }
}

void PrimitiveRenderer::renderLine(Image& bufor, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color) {
    auto dx = static_cast<float>(static_cast<int>(x2 - x1)), dy = static_cast<float>(static_cast<int>(y2 - y1));
    if (dx==0) {
        if (y1 > y2) {
            swap(y1, y2);
        }
        for (unsigned int i=y1; i<y2; i++) {
            if (x1<bufor.getSize().x && i<bufor.getSize().y) {
                bufor.setPixel({x1,i}, color);
            }
        }
    }
    if (dy == 0) {
        if (x1>x2) {
            swap(x1, x2);
        }
        for (unsigned int i=x1; i<x2; i++) {
            if (i<bufor.getSize().x && y1<bufor.getSize().y) {
                bufor.setPixel({i,y1}, color);
            }
        }
    } else {
        if (dy/dx<-1 || dy/dx>1) {
            if (y1>y2) {
                swap(y1, y2);
                swap(x1, x2);
                dx = static_cast<float>(static_cast<int>(x2 - x1)), dy = static_cast<float>(static_cast<int>(y2 - y1));
            }
            float m=dx/dy;
            for (unsigned int i=y1; i<=y2; i++) {
                float step=m*(i-y1)+x1;
                unsigned int x=round(step);
                if (x<bufor.getSize().x && i<bufor.getSize().y) {
                    bufor.setPixel({x,i}, color);
                }
            }
        } else {
            if (x1 > x2) {
                swap(x1, x2);
                swap(y1, y2);
                dx = static_cast<float>(static_cast<int>(x2 - x1)), dy = static_cast<float>(static_cast<int>(y2 - y1));
            }
            float m=dy/dx;
            for (unsigned int i=x1; i<=x2; i++) {
                float step=m*(i-x1)+y1;
                unsigned int y=round(step);
                if (i<bufor.getSize().x && y<bufor.getSize().y) {
                    bufor.setPixel({i,y}, color);
                }
            }
        }
    }
}
/*
void PrimitiveRenderer::renderSquare(Image& bufor, unsigned int x, unsigned int y, unsigned int a) {
    renderLine(bufor,x,y,x+a,y);
    renderLine(bufor,x,y,x,y+a);
    renderLine(bufor,x+a,y+a,x+a,y);
    renderLine(bufor,x+a,y+a,x,y+a);
}

void PrimitiveRenderer::renderRectangle(Image& bufor, unsigned int x, unsigned int y, unsigned int a, unsigned int b) {
    renderLine(bufor,x,y,x+a,y);
    renderLine(bufor,x,y,x,y+b);
    renderLine(bufor,x+a,y+b,x+a,y);
    renderLine(bufor,x+a,y+b,x,y+b);
}
*/
void PrimitiveRenderer::renderCircle(Image& bufor, unsigned int x, unsigned int y, unsigned int r, Color color) {
    for (float a=0; a<M_PI/4; a+=0.01) {
        unsigned int x1 = r*cos(a);
        unsigned int y1 = r*sin(a);
        unsigned int tmpX, tmpY;
        if ((tmpX=x+x1)<bufor.getSize().x && (tmpY=y+y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-x1)<bufor.getSize().x && (tmpY=y+y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x+x1)<bufor.getSize().x && (tmpY=y-y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-x1)<bufor.getSize().x && (tmpY=y-y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x+y1)<bufor.getSize().x && (tmpY=y+x1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-y1)<bufor.getSize().x && (tmpY=y+x1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x+y1)<bufor.getSize().x && (tmpY=y-x1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-y1)<bufor.getSize().x && (tmpY=y-x1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        /*
        bufor.setPixel({x+x1,y+y1},Color::White);
        bufor.setPixel({x-x1,y+y1},Color::White);
        bufor.setPixel({x+x1,y-y1},Color::White);
        bufor.setPixel({x-x1,y-y1},Color::White);
        bufor.setPixel({x+y1,y+x1},Color::White);
        bufor.setPixel({x-y1,y+x1},Color::White);
        bufor.setPixel({x+y1,y-x1},Color::White);
        bufor.setPixel({x-y1,y-x1},Color::White);
        */
    }
    floodFill(bufor,x,y,color);
}

void PrimitiveRenderer::renderElipsis(Image& bufor, unsigned int x, unsigned int y, unsigned int r1, unsigned int r2, Color color) {
    for (float a=0; a<M_PI/2; a+=0.01) {
        unsigned int x1 = r1*cos(a);
        unsigned int y1 = r2*sin(a);
        unsigned int tmpX, tmpY;
        if ((tmpX=x+x1)<bufor.getSize().x && (tmpY=y+y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-x1)<bufor.getSize().x && (tmpY=y+y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x+x1)<bufor.getSize().x && (tmpY=y-y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        if ((tmpX=x-x1)<bufor.getSize().x && (tmpY=y-y1)<bufor.getSize().y) {
            bufor.setPixel({tmpX,tmpY},color);
        }
        /*
        bufor.setPixel({x+x1,y+y1},Color::White);
        bufor.setPixel({x-x1,y+y1},Color::White);
        bufor.setPixel({x+x1,y-y1},Color::White);
        bufor.setPixel({x-x1,y-y1},Color::White);
        */
    }
    floodFill(bufor,x,y,color);
}

void PrimitiveRenderer::floodFill(Image &bufor, unsigned int x, unsigned int y, Color color) {
    Color background = bufor.getPixel({x,y});
    stack<Vector2u> floodStack;
    floodStack.push({x,y});
    while (!floodStack.empty()) {
        Vector2u f = floodStack.top();
        floodStack.pop();
        if (f.x<bufor.getSize().x && f.y<bufor.getSize().y && bufor.getPixel(f)!=color && bufor.getPixel(f)==background) {
            bufor.setPixel({f.x,f.y}, color);
            floodStack.push({f.x,f.y+1});
            floodStack.push({f.x+1,f.y});
            floodStack.push({f.x-1,f.y});
            floodStack.push({f.x,f.y-1});
        }
    }
}

Color PrimitiveRenderer::blend(Color s, Color d) {
    if (s.a == 255) return s;
    if (s.a == 0) return d;
    if (d.a == 255) {
        auto mix = [&](int sc, int dc) { return static_cast<uint8_t>((sc * s.a + dc * (255 - s.a)) / 255); };
        return Color(mix(s.r, d.r), mix(s.g, d.g), mix(s.b, d.b), 255);
    }
    const float sa = s.a / 255.f, da = d.a / 255.f;
    const float oa = sa + da * (1.f - sa);
    if (oa <= 0.f) return Color(0, 0, 0, 0);
    auto mix = [&](int sc, int dc) { return static_cast<uint8_t>(lround((sc * sa + dc * da * (1.f - sa)) / oa)); };
    return Color(mix(s.r, d.r), mix(s.g, d.g), mix(s.b, d.b), static_cast<uint8_t>(lround(oa * 255.f)));
}

void PrimitiveRenderer::fillCircle(Image& bufor, int x, int y, int r, Color color) {
    if (r < 0) return;
    const int w = static_cast<int>(bufor.getSize().x), h = static_cast<int>(bufor.getSize().y);
    for (int dy = -r; dy <= r; ++dy) {
        const int py = y + dy;
        if (py < 0 || py >= h) continue;
        const int half = static_cast<int>(sqrt(static_cast<float>(r*r - dy*dy)) + 0.5f);
        for (int px = max(x - half, 0); px <= min(x + half, w - 1); ++px) {
            bufor.setPixel({static_cast<unsigned int>(px), static_cast<unsigned int>(py)}, color);
        }
    }
}
