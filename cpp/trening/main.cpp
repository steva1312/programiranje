#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

//global variables
int wait_time=3, set_time, pause_time, sets, time_left, sc_width=400, sc_height=400;
sf::Font font;
sf::Text secText;
sf::SoundBuffer buffer;
sf::Sound sound;

enum class State
{
    wait, train, pause, over
};

State state=State::wait;

//function declarations
void Input(int& var,string text);
void Draw(sf::RenderWindow&);
void Events(sf::RenderWindow&);
void Tick();
void SetUpText();
void SetUpSound();
void CenterText();
sf::Color ScreenColor();

int main()
{
    Input(set_time,"Seconds per set: ");
    Input(pause_time,"Seconds per pause: ");
    Input(sets,"Amount of sets: ");
    time_left=wait_time+1;

    sf::RenderWindow window(sf::VideoMode(sc_width, sc_height), "Namaks Nagas!");
    window.setFramerateLimit(1);

    SetUpText();
    SetUpSound();

    while (window.isOpen())
    {
        Events(window);
        Draw(window);
        if(state!=State::over)Tick();
    }
}

void Input(int& var,string text)
{
    cout<<text;
    cin>>var;
}

void Draw(sf::RenderWindow& window)
{
    window.clear(ScreenColor());

    CenterText();
    window.draw(secText);

    window.display();
}

void Events(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Tick()
{
    if(time_left==0)
    {
        if(state==State::train)
        {
            time_left=pause_time;
            state=State::pause;
            sets--;
            if(sets==0)state=State::over;
        }
        else
        {
            time_left=set_time;
            state=State::train;
        }
        sound.play();
    }
    else
    {
        time_left--;
    }

    if(state!=State::over)
    {
        if(state==State::wait&&time_left==0)
        {
            secText.setString("Go!");
        }
        else
        {
            secText.setString(to_string(time_left));
        }
    }
    else
    {
        secText.setString("Done!");
    }
}

void SetUpText()
{
    if(!font.loadFromFile("times.ttf"))cout<<"Font not found!"<<endl;

    secText.setFont(font);
    secText.setCharacterSize(150);
    secText.setFillColor(sf::Color::White);
    secText.setStyle(sf::Text::Bold);
}

void SetUpSound()
{
    if(!buffer.loadFromFile("sound1.wav"))cout<<"Sound not found!";
    sound.setBuffer(buffer);
}

void CenterText()
{
    sf::FloatRect textRect = secText.getLocalBounds();
    secText.setOrigin(textRect.left+textRect.width/2.0f, textRect.top+textRect.height/2.0f);
    secText.setPosition(sc_width/2.f,sc_height/2.f);
}

sf::Color ScreenColor()
{
    sf::Color c;
    switch(state)
    {
        case State::train: c=sf::Color(255,102,102); break;
        case State::pause: c=sf::Color(102,178,255); break;
        case State::over: case State::wait: c=sf::Color(102,255,102); break;
    }
    return c;
}
