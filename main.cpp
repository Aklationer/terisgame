#include <sfml/Graphics.hpp>
#include <iostream>
using namespace sf;
int main(){
    Texture Background,Yellow,blue,green,purple,orange,red;
    Background.loadFromFile("picture/background.png");
    Yellow.loadFromFile("picture/yellow.png");
    blue.loadFromFile("picture/blue.png");
    green.loadFromFile("picture/green.png");
    purple.loadFromFile("picture/purple.png");
    orange.loadFromFile("picture/orange.png");
    red.loadFromFile("picture/red.png");
    Background.setRepeated(true);
    Font f;
    f.loadFromFile("msjh.ttc");
    int number=0;
    std::string str=std::to_string(number)+" score";
    Text gride(str,f),over("You Lose",f);
    over.setFillColor(Color(Color::Red));
    over.setCharacterSize(80);
    gride.setFillColor(Color(Color::Cyan));
    Sprite background(Background);
    Vector2u size=Yellow.getSize();
    Vector2i base(20,20),start(base.x/2,0),orign(base.x/2,0);
    background.setTextureRect({0,0,int(size.x*base.x),int(size.y*base.y)});
    Clock clock;
    RenderWindow w(VideoMode(size.x*base.x,size.y*base.y),"TetrisGame");
    Event evt;
    enum  di{right,left,hold,down,rotate,space};
    enum  shape{none,o,i,l,t,s,z};
    shape field[20][20]={};
    std::map<shape,std::vector<std::vector<Vector2i>>> dir = {
        {o,{{Vector2i(0,0),Vector2i(1,0),Vector2i(0,-1),Vector2i(1,-1)}}},
        {i,{{Vector2i(0,0),Vector2i(1,0),Vector2i(2,0),Vector2i(3,0)},{Vector2i(0,0),Vector2i(0,-1),Vector2i(0,-2),Vector2i(0,-3)}}},
        {l,{{Vector2i(0,0),Vector2i(1,0),Vector2i(2,0),Vector2i(0,-1)},{Vector2i(0,0),Vector2i(0,-1),Vector2i(0,-2),Vector2i(-1,0)},
            {Vector2i(0,0),Vector2i(-1,0),Vector2i(-2,0),Vector2i(0,1)},{Vector2i(0,0),Vector2i(0,1),Vector2i(0,2),Vector2i(1,0)}}},
        {t,{{Vector2i(0,0),Vector2i(1,0),Vector2i(2,0),Vector2i(1,1)},{Vector2i(0,0),Vector2i(0,-1),Vector2i(0,-2),Vector2i(1,-1)},
            {Vector2i(0,0),Vector2i(-1,0),Vector2i(-2,0),Vector2i(-1,-1)},{Vector2i(0,0),Vector2i(0,1),Vector2i(0,2),Vector2i(-1,1)}}},
        {s,{{Vector2i(0,0),Vector2i(1,0),Vector2i(1,-1),Vector2i(2,-1)},{Vector2i(0,0),Vector2i(0,-1),Vector2i(-1,-1),Vector2i(-1,-2)}}},
        {z,{{Vector2i(0,0),Vector2i(1,0),Vector2i(1,1),Vector2i(2,1)},{Vector2i(0,0),Vector2i(0,-1),Vector2i(1,-1),Vector2i(1,-2)}}}
    };
    std::map<shape, Sprite> color = {
        {o,Sprite(Yellow)},
        {i,Sprite(blue)},
        {l,Sprite(green)},
        {t,Sprite(purple)},
        {s,Sprite(orange)},
        {z,Sprite(red)}
    };
    int index=0;
    shape key=o;
    bool g=true;
    while (w.isOpen())
    {
        di move=hold;
        int count=0,nextindex=index;
        Vector2i next=start;
        std::vector<Vector2i> startshape=dir[key][index];
        if(w.pollEvent(evt)){
            if(evt.type == Event::Closed)
                w.close();
            if(evt.type == Event::KeyPressed){
                if(g){
                    if(evt.key.code == Keyboard::Right)
                        move=right;
                    else if(evt.key.code == Keyboard::Left)
                        move=left;
                    else if(evt.key.code == Keyboard::Down)
                        move=down;
                    else if(evt.key.code == Keyboard::Up){
                        nextindex++;
                        if(nextindex == dir[key].size()){
                            nextindex=0;
                        }
                    }
                    else if(evt.key.code == Keyboard::Space)
                        move=space;
                }
                else{
                    for (int i = 0; i < 20; i++)
                        for (int j = 0; j < 20; j++)
                            field[i][j] = shape::none;
                    g=true;
                    number=0;
                    str="0 score";
                }
            }
        }
        std::vector<Vector2i> nextshape=dir[key][nextindex];
        if(clock.getElapsedTime().asSeconds()>0.3 ) {
            if(g)
                move=down;
            clock.restart();
        }
        if(move == right)
            next.x++;
        else if(move == left)
            next.x--;
        else if(move == down)
            next.y++;
        if(move !=space){
            for(Vector2i& i : nextshape){
                Vector2i temp=i+next;
                if(temp.x >=0 && temp.x<base.x && temp.y<base.y && (temp.y<0 || field[temp.x][temp.y]==none))
                    count++;
            }
            if(count == 4){
                start=next;
                startshape=nextshape;
                index=nextindex;
            }
            else{
                if(move==down){
                        for(Vector2i& i:startshape){
                        Vector2i temp=i+start;
                        if(start.y>=0)
                            field[temp.x][temp.y]=key;
                    }
                    for(int i=base.x-1;i>=0;i--){
                        int ans=0;
                        for(int j=0;j<base.y;j++){
                            if(field[j][i]==none)
                                ans++;
                        }
                        if(ans==base.x)
                            break;
                        else if(ans == 0){
                            for(int k=i;k>0;k--){
                                for(int j=0;j<base.y;j++){
                                    field[j][k]=field[j][k-1];
                                }
                            }
                            for(int j=0;j<base.y;j++)
                                field[j][0]=none;
                            i++;
                            str=std::to_string(++number)+" score";
                            gride.setString(str);
                        }
                    }
                    key=shape(rand()%6 + 1);
                    start=orign;
                    index=0;
                    if(field[orign.x][orign.y]!=none || field[orign.x+1][orign.y]!=none)
                        g=false;
                }
            }
        }
        else if(move==space){
            while(true){
                for(Vector2i& i : nextshape){
                    Vector2i temp=i+next;
                    if(temp.x >=0 && temp.x<base.x && temp.y<base.y && (temp.y<0 || field[temp.x][temp.y]==none))
                        count++;
                }
                if(count %4==0)
                    next.y++;
                else{
                    start.x=next.x;
                    start.y=next.y-1;
                    startshape=nextshape;
                    index=nextindex;
                    for(Vector2i& i:startshape){
                        Vector2i temp=i+start;
                        if(start.y>=0)
                            field[temp.x][temp.y]=key;
                    }
                    break;
                }
            }
            key=shape(rand()%6 + 1);
            start=orign;
            index=0;
            for(int i=base.x-1;i>=0;i--){
                int ans=0;
                for(int j=0;j<base.y;j++){
                    if(field[j][i]==none)
                        ans++;
                }
                if(ans==base.x)
                    break;
                else if(ans == 0){
                    for(int k=i;k>0;k--){
                        for(int j=0;j<base.y;j++){
                            field[j][k]=field[j][k-1];
                        }
                    }
                    for(int j=0;j<base.y;j++)
                        field[j][0]=none;
                    i++;
                    str=std::to_string(++number)+" score";
                    gride.setString(str);
                }
                if(field[orign.x][orign.y]!=none || field[orign.x+1][orign.y]!=none)
                        g=false;
            }
        }
        w.clear();
        w.draw(background);
        w.draw(gride);
        for(int i=0;i<base.x;i++){
            for(int j=0;j<base.y;j++){
                if(field[i][j]){
                    Sprite p = color[field[i][j]];
                    p.setPosition(float(size.x* i), float(size.y* j));
                    w.draw(p);
                }
            }
        }
        if(g){
            for(Vector2i &i: startshape){
                Vector2i temp=i+start;
                Sprite p =color[key];
                p.setPosition(float(size.x*temp.x), float(size.y*temp.y));
                w.draw(p);
            }
        }
        else{
            over.setPosition(float(size.x*base.x/2-(over.getGlobalBounds().width)/2),float(size.x*base.x/2-(over.getGlobalBounds().height)/2));
            w.draw(over);
        }
        w.display();
    }
}