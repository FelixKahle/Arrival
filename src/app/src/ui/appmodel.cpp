// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QRegularExpression>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QThreadPool>

#include <functional>
#include <array>

#include "xlsxdocument.h"

#include "data/csvhandling.h"
#include "ui/appmodel.h"

namespace Arrival::App
{
    static QString correctPath(const QString& path)
    {
        static const QRegularExpression regex("^(file:\/{3})|(qrc:\/{2})|(http:\/{2})");
        QString correctPath = path;
        return correctPath.replace(regex, "");
    }

    AppModel::AppModel(QObject* parent)
        : QObject(parent)
        , m_jobTable(new JobTable(this))
        , m_templateList(new SelectedHeadersTemplateList(this))
        , m_jobTableFuture()
        , m_jobTableFutureWatcher()
    {
        m_templateList->loadFromJson("templates.json");
    }

    JobTable* AppModel::jobTable() const
    {
        return m_jobTable;
    }

    void AppModel::setJobTable(JobTable* jobTable)
    {
        m_jobTable = jobTable;
        emit jobTableChanged(jobTable);
    }

    SelectedHeadersTemplateList* AppModel::templateList() const
    {
        return m_templateList;
    }

    void AppModel::setTemplateList(SelectedHeadersTemplateList* list)
    {
        m_templateList = list;
        emit templateListChanged(list);
    }

    void AppModel::parseCSV(const QString &csvPath1, const QString &csvPath2)
    {
        // Emit the signal that the .csv parsing started.
        emit parsingStarted();

        // Catch invalid states.
        if (csvPath1.isEmpty() || csvPath2.isEmpty())
        {
            qWarning() << "Empty path";
            return;
        }

        // Correct the paths.
        QString filePath1 = correctPath(csvPath1);
        QString filePath2 = correctPath(csvPath2);

        // Catch invalid states.
        if (!QFile::exists(filePath1) || !QFile::exists(filePath2))
        {
            qWarning() << "File does not exist";
            return;
        }

        // Load the files and extract the data in a different thread.
        // This ensures that the main thread is not blocked and that the ui will run
        // without brakes.
        // Especially important on lower spec pc.
        m_jobTableFuture = QtConcurrent::run(QThreadPool::globalInstance(), [=](const QString& path1, const QString& path2)
        {
            CSVDocument doc1(filePath1);
            CSVDocument doc2(filePath2);
            return CSVCombinedData::getCSVCombinedData(doc1, doc2);
        }, filePath1, filePath2);
        connect(&m_jobTableFutureWatcher, &QFutureWatcher<std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError>>::finished, this, &AppModel::onCSVParsed);
        m_jobTableFutureWatcher.setFuture(m_jobTableFuture);
    }

    void AppModel::onCSVParsed()
    {
        // Transfer the data into the list that takes care of displaying the data.
        std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError> result = m_jobTableFutureWatcher.result();
        if (const auto result = m_jobTableFutureWatcher.result(); result.has_value())
        {
            m_jobTable->setTableData(result.value());
        }

        // Disconnect all.
        m_jobTableFutureWatcher.disconnect();
        m_jobTableFutureWatcher.setFuture(QFuture<std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError>>());

        // Emit the signal that the .csv parsing ended.
        emit parsingCompleted();
    }

    void AppModel::xlsxExport(const QString& path, QList<int> columns)
    {
        // Catch invalid states.
        if (path.isEmpty() || !m_jobTable || !m_jobTable->hasData() || m_jobTable->rowCount() <= 0 || columns.empty())
        {
            return;
        }

        // Correct the path.
        QString filePath = correctPath(path);

        // Check if the path/file has the file extension .xlsx
        // If not, add it.
        static const QRegularExpression regex("^.*\.(xlsx)$");
        if (!regex.match(filePath).hasMatch())
        {
            filePath.append(".xlsx");
        }

        // Create the document on the heap as it gets passed to a different thread later on.
        // This ensures that it does not run out of scope.
        QSharedPointer<QXlsx::Document> xlsxDocument = QSharedPointer<QXlsx::Document>::create();

        // Format.
        QXlsx::Format newAddedFormat;
        newAddedFormat.setPatternBackgroundColor(QColor::fromString(ARRIVAL_EXCEL_EXPORT_NEW_ADDED_CELL_COLOR));
        QXlsx::Format removedFormat;
        removedFormat.setPatternBackgroundColor(QColor::fromString(ARRIVAL_EXCEL_EXPORT_REMOVED_CELL_COLOR));

        // Write the headers.
        for (int headerIterator = 0; headerIterator < columns.count(); headerIterator++)
        {
            const QString& cellString = m_jobTable->data()->headerNames().at(columns.at(headerIterator));
            xlsxDocument->write(1, headerIterator + 1, cellString);
        }

        // Write the actual data.
        for (int i = 0; i < m_jobTable->rowCount(); i++)
        {
            int rowIndex = i + 2;
            const JobTableRow& row = m_jobTable->data()->rows().at(i);

            for (int columnIterator = 0; columnIterator < columns.count(); columnIterator++)
            {
                const int column = columns.at(columnIterator);
                QXlsx::Format format;
                if (row.state == JobTableRowState::Added)
                {
                    format = newAddedFormat;
                }
                else if (row.state == JobTableRowState::Removed)
                {
                    format = removedFormat;
                }
                else
                {
                    format = QXlsx::Format();
                }
                const QString& cellString = row.columns.at(column);
                xlsxDocument->write(rowIndex, columnIterator + 1, cellString, format);
            }
        }

        // Writing the file is done on a different thread.
        // This ensures the ui does not freeze.
        // Especially important on lower spec pc.
        auto future = QtConcurrent::run(QThreadPool::globalInstance(), [=](QSharedPointer<QXlsx::Document> document)
        {
            if (!xlsxDocument->saveAs(filePath))
            {
                qWarning() << "Error saving .xlsx document: " + filePath;
            }
        }, xlsxDocument);
    }
}
