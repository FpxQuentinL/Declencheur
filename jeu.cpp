#include "jeu.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonObject>
#include <audiocontroller.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <wiringPi.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>




Jeu::Jeu(IPluginsReport *report) : IPlugins(report)
{

}



bool Jeu::Game_Fail()
{
    QJsonObject play_sound
    {
        {"command", "stop"},
        {"id", "file:///home/pi/Downloads/test.mp3"}
    };

    std::cout << "YOU LOOSE !!!" << std::endl;
    Signal_RCon_AudioControler(play_sound);
    return (true);
}

void Jeu::loadConfigGameTech(QString config_file)
{
    QJsonDocument docGameTech;
    QDir dirGameTech;
    qDebug() << dirGameTech.path();
    QFile fileGameTech(config_file);
    if (!fileGameTech.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;
    docGameTech = QJsonDocument::fromJson(fileGameTech.readAll());
    QJsonObject object = docGameTech.object();
    qDebug("\n\n###\t start json file\t###");

    _OutputRelay = object.value("ControlerLedV").toString().toInt();

    qDebug("###\t end json file###\n\n");

}
void Jeu::loadConfigGameFonct(QString config_file)
{
    QJsonDocument docGame;
    QDir dirGame;
    qDebug() << dirGame.path();
    QFile fileGame(config_file);
    if (!fileGame.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;

    docGame = QJsonDocument::fromJson(fileGame.readAll());
    qDebug()<<docGame;
    QJsonObject object = docGame.object();


    qDebug("\n\n###\t start json file\t###");
    _SeuilDetect = object.value("SeuilDeDetection").toInt();
    qDebug("###\t end json file###\n\n");

}



void Jeu::Setup()
{
    wiringPiSetupGpio();

    pinMode(_OutputRelay,OUTPUT);
    digitalWrite(_OutputRelay,LOW);
    loadConfigGameFonct("./GameFonct.json");
    loadConfigGameTech("./GameTech.json");
}

bool Jeu::Game_Socketio()
{
    QJsonObject packet
    {
        {"io_socket", "iterate_and_pulse"},
        {"command", "{\"color\":\"0xFAFAFA\",\"delay\":\"1\",\"brightness_max\":\"255\",\"brightness_min\":\"32\",\"brightness_scale\":\"5\"pos_start\":\"0\",\"pos_end\":\"144\",\"scale\":\"i\"}"}
    };

    std::cout << "YOU Socket iO !!!" << std::endl;
    Signal_RCon_NetworkIO(packet);
    return (true);
}

void Jeu::Slot_RCon( QJsonObject packet )
{
   qDebug()<<">IPlugins::Slot_RCon<"<< "\n" << packet;

        if (packet.contains("command"))
        {
            if (packet.value("command")=="CallbackDetect")
            {
               _CurrentDetect = packet.value("DataDetect").toInt();
            }

        }
}


//GameFlow
void Jeu::main_game_callback()
{

      if(_SeuilDetect<_CurrentDetect)
      {
      digitalWrite(_OutputRelay, HIGH);
      }



}
