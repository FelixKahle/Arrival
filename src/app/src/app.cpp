// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QIcon>

#include "app.h"
#include "ui/appmodel.h"
#include "ui/headerlistmodel.h"
#include "ui/jobtablemodel.h"
#include "ui/selectedheaderstemplatemodel.h"

namespace Arrival::App
{
    ArrivalApp::ArrivalApp(int argc, char *argv[])
        : guiApp(argc, argv)
        , qmlEngine()
    {
        registerUiTypes();

        // Appearance
        guiApp.setWindowIcon(QIcon(":/resources/ArrivalLogo.ico"));
        QQuickStyle::setStyle("Fusion");

        QObject::connect(
            &qmlEngine, &QQmlApplicationEngine::objectCreationFailed,
            &guiApp, []()
            { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
        qmlEngine.loadFromModule("Arrival", "Main");
    }

    void ArrivalApp::registerUiTypes()
    {
        qmlRegisterType<AppModel>("Arrival", 1, 0, "AppModel");
        qmlRegisterType<HeaderListModel>("Arrival", 1, 0, "HeaderListModel");
        qmlRegisterType<JobTableModel>("Arrival", 1, 0, "JobTableModel");
        qmlRegisterType<SelectedHeadersTemplateModel>("Arrival", 1, 0, "SelectedHeadersTemplateModel");

        qmlRegisterUncreatableType<JobTableRowState>("Arrival", 1, 0, "JobTableRowState", "Cannot create JobTableRowState");
        qmlRegisterUncreatableType<JobTable>("Arrival", 1, 0, "JobTableBackend", "Cannot create JobList");
        qmlRegisterUncreatableType<SelectedHeadersTemplateList>("Arrival", 1, 0, "SelectedHeadersTemplateList", "Cannot create SelectedHeadersTemplateList");
        qmlRegisterUncreatableType<SelectedHeadersTemplate>("Arrival", 1, 0, "SelectedHeadersTemplate", "Cannot create SelectedHeadersTemplate");
    }

    int ArrivalApp::exec()
    {
        return guiApp.exec();
    }
}
