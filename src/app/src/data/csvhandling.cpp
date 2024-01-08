// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QSet>
#include <QElapsedTimer>
#include <QRegularExpression>
#include <QDebug>
#include <QThread>

#include <utility>
#include <algorithm>

#include "data/csvhandling.h"

#ifdef QT_DEBUG
#define ARRIVAL_DEBUG 1
#endif

namespace Arrival::App
{
    bool CSVCombinedData::isJobNumber(const QString& str)
    {
        // ^[0-9]{7}CL$ to match entire string.
        // BUT: Some entries seem to have two Jobnumbers?!??????
        static QRegularExpression regex("^[0-9]{9}CL");

        return regex.match(str).hasMatch();
    }

    int CSVCombinedData::findSingleJobNumberColumnIndex(const CSVDocument& document)
    {
        // No rows in the document. Thus no job number has been found.
        if (document.rowCount() <= 0)
        {
            return -1;
        }

        // Check the first row.
        // Assume the other cells in this row also contain the job number.
        const auto& firstRow = document.data().at(0);

        int singleJobNumberColumnIndex = 0;
        int foundJobNumberColumns = 0;
        for (int columnIterator = 0; columnIterator < firstRow.count(); columnIterator++)
        {
            if (isJobNumber(firstRow.at(columnIterator)))
            {
                singleJobNumberColumnIndex = columnIterator;
                foundJobNumberColumns++;
            }
        }

        // If more ore less than 1 Jobnumber column has been found return -1.
        // Otherwise return the index of the column holding the Jobnumber.
        return foundJobNumberColumns == 1 ? singleJobNumberColumnIndex : -1;
    }

    bool CSVCombinedData::rowExistsInCSVDocumentRowJobNumberSearch(const QList<QString>& row, const CSVDocument& document, int jobNumberIndex)
    {
        // Early exit.
        if (row.count() != document.data().front().size() || jobNumberIndex < 0 || jobNumberIndex > row.count())
        {
            return false;
        }

        for (const auto& documentRow : document.data())
        {
            // TODO: Ask what should happen if I have two Jobnumbers.
            if (row.at(jobNumberIndex) == documentRow.at(jobNumberIndex))
            {
                return true;
            }
        }

        // No match found.
        return false;
    }

    bool CSVCombinedData::rowExistsInCSVDocumentRowHashSearch(const QList<QString>& row, const CSVDocument& document)
    {
        // Create a QSet<QString> from the row. Used for faster lookup.
        QSet<QString> rowSet(row.begin(), row.end());
        for (const auto& documentRow : document.data())
        {
            if (rowSet == QSet<QString>(documentRow.begin(), documentRow.end()))
            {
                return true;
            }
        }

        // No match found.
        return false;
    }

    QString CSVCombinedData::computeFormatIdentifier(const QList<QString>& headerNames)
    {
        if (headerNames.empty())
        {
            return QString();
        }

        QString concatenatedString = headerNames.join("");
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(concatenatedString.toUtf8());
        QByteArray hashData = hash.result();
        QString uniqueHash = QString(hashData.toHex());
        return uniqueHash;
    }

    // This function takes in tow CSVDocuments and combines them.
    // It filters which rows have been added, removed and which stayed the same.
    // It tries to find a Jobnumber inside the files and then compares the rows based on their Jobnumber.
    // If no Jobnumber is found in one of the files or the column index of these Jobnumbers is different
    // in these two files, a different comparison method is used.
    // This method hashes the entire row and compares the hashes.
    // This means that a single change in the row would lead to a the recognition of removed/added.
    //
    // TODO: What should happen if we have tow Jobnumbers in one column.
    // TODO: What should happen if we have moe than one Jobnumbers in different columns.
    std::expected<QSharedPointer<CSVCombinedData>, CSVCombinedData::CombineCSVDocumentsError> CSVCombinedData::getCSVCombinedData(const CSVDocument& firstDocument, const CSVDocument& secondDocument)
    {
#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME || ARRIVAL_DEBUG
        QElapsedTimer timer;
        timer.start();
#endif

        // Two empty documents are not going to be compared.
        if (firstDocument.isEmpty() && secondDocument.isEmpty())
        {
            return std::unexpected<CSVCombinedData::CombineCSVDocumentsError>(CSVCombinedData::CombineCSVDocumentsError::BothEmpty);
        }

        // The two documents do not have the same amount of columns.
        // As they are not the same format, they cannot be compared.
        if (firstDocument.columnCount() != secondDocument.columnCount())
        {
            return std::unexpected<CSVCombinedData::CombineCSVDocumentsError>(CSVCombinedData::CombineCSVDocumentsError::DifferentFormat);
        }

        const int secondDocumentRowCount = secondDocument.rowCount();
        const int firstDocumentJobNumberIndex = findSingleJobNumberColumnIndex(firstDocument);
        const int secondDocumentJobNumberIndex = findSingleJobNumberColumnIndex(secondDocument);

        // If no job number has been found inside the document a hashing method is used to compare the
        // rows inside the documents.
        // Hashing each row of the documents takes longer and might result in ugly program outputs
        // because one change in a row result in the program trating the entire row as new added/removed.
        const bool useFallbackHashing = (firstDocumentJobNumberIndex == -1 || secondDocumentJobNumberIndex == -1 || firstDocumentJobNumberIndex != secondDocumentJobNumberIndex);

        // List of JobTableRows.
        // I dont know how many entries there will be, but I can estimate that it will be around the
        // size of the second document.
        // Reserve that amount of space.
        QList<JobTableRow> rows;
        rows.reserve(secondDocumentRowCount);

        // Search for remained and new added rows.
        int newAddedCount = 0;
        for (auto iterator = secondDocument.data().begin(); iterator != secondDocument.data().end(); iterator++)
        {
            // Check whether the row has been added.
            bool newAdded = false;
            if (useFallbackHashing)
            {
                newAdded = !(rowExistsInCSVDocumentRowHashSearch(*iterator, firstDocument));
            }
            else
            {
                newAdded = !(rowExistsInCSVDocumentRowJobNumberSearch(*iterator, firstDocument, firstDocumentJobNumberIndex));
            }

            // If the row was new added increase the count by one.
            if (newAdded)
            {
                newAddedCount += 1;
            }

            // Setup the row.
            JobTableRow row;
            row.state = newAdded ? JobTableRowState::Added : JobTableRowState::Remained;
            row.columns = *iterator;

            rows.append(row);
        }

        // Search for removed rows.
        int removedCount = 0;
        for (auto iterator = firstDocument.data().begin(); iterator != firstDocument.data().end(); iterator++)
        {
            // Check whether the row has been removed.
            bool removed = false;
            if (useFallbackHashing)
            {
                removed = !(rowExistsInCSVDocumentRowHashSearch(*iterator, secondDocument));
            }
            else
            {
                removed = !(rowExistsInCSVDocumentRowJobNumberSearch(*iterator, secondDocument, firstDocumentJobNumberIndex));
            }

            if (removed)
            {
                removedCount += 1;

                // Add the removed rows here.
                // New and remained rows have been added in the first for() loop.
                JobTableRow row;
                row.state = JobTableRowState::Removed;
                row.columns = *iterator;

                rows.append(row);
            }
        }

        std::sort(rows.begin(), rows.end(), [](const JobTableRow& a, const JobTableRow& b) -> bool {
            return a.state < b.state;
        });

        QSharedPointer<CSVCombinedData> result = QSharedPointer<CSVCombinedData>::create();
        result->m_formatIdentifier = computeFormatIdentifier(secondDocument.headersNames());
        result->m_headerNames = secondDocument.headersNames();
#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        result->m_headerIndices = secondDocument.headerIndices();
#endif
        result->m_rows = std::move(rows);
        result->m_newAddedCount = newAddedCount;
        result->m_removedCount = removedCount;


#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME || ARRIVAL_DEBUG
        const int duration = timer.elapsed();
#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME
        const int remainingTime = minimumGetCSVCombinedDataExecutionTime - duration;
#endif
#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME && ARRIVAL_DEBUG
        qDebug() << "getCSVCombinedData() took " << duration << "milliseconds to execute, waiting " << remainingTime << "milliseconds";
#elif ARRIVAL_DEBUG
        qDebug() << "getCSVCombinedData() took " << duration << "milliseconds to execute";
#endif
#endif
#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME
        if (remainingTime > 0)
        {
            QThread::msleep(remainingTime);
        }
#endif

        return result;
    }

    void CSVCombinedData::clear()
    {
        m_headerNames.clear();
#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        m_headerIndices.clear();
#endif
        m_rows.clear();
        m_newAddedCount = 0;
        m_removedCount = 0;
    }
}
