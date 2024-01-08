// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_APPMODEL_H
#define ARRIVAL_APPMODEL_H

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>

#include "data/csvhandling.h"
#include "data/jobtable.h"
#include "data/selectedheaderstemplatelist.h"

#define ARRIVAL_EXCEL_EXPORT_NEW_ADDED_CELL_COLOR "#248046"
#define ARRIVAL_EXCEL_EXPORT_REMOVED_CELL_COLOR "#DA373C"

namespace Arrival::App
{
    /*!
     * \brief The AppModel class used in qml. Stores important information for the app.
     * Used as an interface between the ui and the c++ backend.
     */
    class AppModel : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(JobTable* jobTable READ jobTable WRITE setJobTable NOTIFY jobTableChanged)
        Q_PROPERTY(SelectedHeadersTemplateList* templateList READ templateList WRITE setTemplateList NOTIFY templateListChanged)
    public:
        Q_INVOKABLE void parseCSV(const QString &csvPath1, const QString &csvPath2);
        Q_INVOKABLE void xlsxExport(const QString& path, QList<int> columns);

        JobTable* jobTable() const;
        void setJobTable(JobTable* list);

        SelectedHeadersTemplateList* templateList() const;
        void setTemplateList(SelectedHeadersTemplateList* list);

    public:
        /*!
         * \brief AppModel Constructs a new \c AppModel
         * \param parent
         */
        explicit AppModel(QObject* parent = nullptr);

    signals:
        void parsingStarted();
        void parsingCompleted();
        void jobTableChanged(JobTable* newValue);
        void templateListChanged(SelectedHeadersTemplateList* list);

    private:
        /*!
         * \brief onCSVParsed Executed when the .csv files have been parsed.
         */
        void onCSVParsed();

    private:
        JobTable* m_jobTable;
        SelectedHeadersTemplateList* m_templateList;

        QFuture<std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError>> m_jobTableFuture;
        QFutureWatcher<std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError>> m_jobTableFutureWatcher;
    };
}

#endif // ARRIVAL_APPMODEL_H
