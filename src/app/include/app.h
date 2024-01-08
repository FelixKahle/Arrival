// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_APP_H
#define ARRIVAL_APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

namespace Arrival::App
{
    /*!
     * \brief The ArrivalApp class starts propper execution of the program.
     */
    class ArrivalApp
    {
    public:
        /*!
         * \brief ArrivalApp Constructs a new \c ArrivalApp
         * \param argc number arguments.
         * \param argv arguments.
         */
        ArrivalApp(int argc, char *argv[]);

        /*!
         * \brief exec executes the app.
         * \return error code of the app.
         */
        int exec();

    private:
        /*!
         * \brief registerUiTypes register qml types for the qml engine.
         */
        void registerUiTypes();

    private:

        /*!
         * \brief guiApp The \c QGuiApplication of the app.
         */
        QGuiApplication guiApp;

        /*!
         * \brief qmlEngine The \c QQmlApplicationEngine of the app.
         */
        QQmlApplicationEngine qmlEngine;
    };
}

#endif // ARRIVAL_APP_H
