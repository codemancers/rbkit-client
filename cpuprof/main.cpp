#include <QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <typeinfo>
#include "storage.h"
#include "mapping.h"
#include <iostream>
#include <string.h>

QString decodeJson(QByteArray&);
void parseFrames(QJsonObject *frame);


//storage *store = new storage();

/*int main(int argc, char *argv[])
{
    int b=1;

    QCoreApplication a(argc, argv);
    QFile file("/Users/ishan/Desktop/qt/textJson/rjson.txt");

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error, could not read file output";
    }

    else {
        QTextStream textStream(&file);
        QString jsonText = textStream.readAll();
        QByteArray byteJson = jsonText.toUtf8();
        qDebug() <<"file contents -> " << jsonText;
        qDebug() << decodeJson(byteJson);
    }

    //store->traverseNodes();
    //store->traverseFlatProfile();
    while(b != 3) {
        qDebug() << "\n\n1. Call Graph";
        qDebug() << "2. Flat Profile";
        qDebug() << "3. Exit";
        std::cin>>b;

        switch (b) {
        case 1:
            store->handleCallGraph();
            break;
        case 2:
            store->traverseFlatProfile();
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    //store->traverseCallGraph(store->getNodes().begin().value());
}

QString decodeJson(QByteArray &byteJson) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(byteJson);
    QJsonObject jsonObject = jsonDocument.object();

    QJsonArray Payload = jsonObject[MAPS::payload].toArray();

    for(int i = 0; i < Payload.size() ; i++) {
        QJsonObject sample = Payload[i].toObject();

        //detect starting of new frame
        store->clearFrameStack();

        QJsonArray framesArray = sample.value(MAPS::payload).toArray();
        for(int j = 0; j < framesArray.count(); j++) {
            QJsonObject frame = framesArray[j].toObject();
            parseFrames(&frame);
        }

        parseFrames(&sample);

        store->incrementSampleCount();
    }

    return "True";
}

void parseFrames(QJsonObject *frames) {
    QVariantMap newNodeData;
    //qDebug() << *frames;
    newNodeData = frames->toVariantMap();
    if(!store->exists(newNodeData[MAPS::method_name].toString())) {
        //method not yet added to the datastructure, add it
        store->addNewNode(newNodeData);
    } else {
        //method already added, update the values
        store->updateExistingMethod(newNodeData);
    }
}*/
