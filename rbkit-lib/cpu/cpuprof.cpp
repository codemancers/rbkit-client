#include <QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <typeinfo>
#include "cpustorage.h"
#include "cpumapping.h"
#include <iostream>
#include <string.h>
#include <QDebug>

#include "cpuprof.h"


static storage *store = new storage();

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
}

QString decodeJson(QByteArray &byteJson) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(byteJson);
    QJsonArray jsonArray = jsonDocument.array();
    //qDebug() << " jsonArray => " << jsonArray;
    foreach(QJsonValue value, jsonArray) {
        QJsonObject sample = value.toObject();
        QJsonArray Payload = sample[MAPS::payload].toArray();
        //qDebug() << Payload;

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
    }

    return "True";
}

void CpuProf::testParsing() {
    int b=1;

    QCoreApplication a();
    QFile file("/Users/ishan/Desktop/qt/textJson/rjson.txt");

    if(!file.open(QIODevice::ReadOnly)) {
        "Error, could not read file output";
    }

    else {
        QTextStream textStream(&file);
        QString jsonText = textStream.readAll();
        QByteArray byteJson = jsonText.toUtf8();
        //qDebug() <<"file contents -> " << jsonText;
        decodeJson(byteJson);
    }

    //store->traverseNodes();
    //store->traverseFlatProfile();
    /*while(b != 3) {
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
    QBENCHMARK {
        store->handleCallGraph();
        store->traverseFlatProfile();
    }*/
}

