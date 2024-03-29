// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_CSVHANDLING_H
#define ARRIVAL_CSVHANDLING_H

#include <QSharedPointer>
#include <QSet>
#include <QString>
#include <QList>
#include <QHash>
#include <QDebug>
#include <QCryptographicHash>

#include <expected>

#include "data/csvdocument.h"
#include "data/jobtablerow.h"

#define ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME 1

namespace Arrival::App
{
    /*!
     * \brief The CSVCombinedData class stores data of two .csv documents combinded.
     */
    class CSVCombinedData
    {
    public:
        enum class CombineCSVDocumentsError
        {
            DifferentFormat,
            BothEmpty
        };

    public:
        /*!
         * \brief getCSVCombinedData takes in two csv documents and combines them.
         * It checks which rows have been deleted, new added and which rows stayed the same.
         * \param firstDocument The first document.
         * \param secondDocument The second document.
         * \return
         */
        static std::expected<QSharedPointer<CSVCombinedData>, CombineCSVDocumentsError> getCSVCombinedData(const CSVDocument& firstDocument, const CSVDocument& secondDocument);

        /*!
         * \brief findSingleJobNumberColumnIndex searches for a single Jobnumber column index inside a \c CSVDocument.
         * If more than or less than 1 matching column index is found, -1 is returned.
         * \param document The docuemt to check.
         * \return -1 if more or less than 1 index has been found, the found index otherwise.
         */
        static int findSingleJobNumberColumnIndex(const CSVDocument& document);

        /*!
         * \brief isJobNumber Checks if a string is aJobnumber.
         * \param str The string to check.
         * \return True if the string is a Job Number, false otherwise.
         */
        static bool isJobNumber(const QString& str);

        /*!
         * \brief rowExistsInCSVDocumentRowJobNumberSearch Checks if a row is inside a CSVDocument.
         * This function compares rows only on their Jobnumber.
         * It needs to know in which column to find the Jobnumber.
         * Row and document have to be formatted in the same way.
         * \param row The row to search for.
         * \param document The document to search in.
         * \param jobNumberIndex The index of the Jobnumber column in the row and in the document.
         * \return True if the row has been found inside the doument, false otherwise.
         */
        static bool rowExistsInCSVDocumentRowJobNumberSearch(const QList<QString>& row, const CSVDocument& document, int jobNumberIndex);

        /*!
         * \brief rowExistsInCSVDocumentRowHashSearch Checks if a row is inside a CSVDocument.
         * This funtion compares the rows by hashing the entire row and then compare the hash to the hash generated by
         * hashing each row in the document.
         * \param row The row to search for.
         * \param document The document to search in.
         * \return True if the row has been found inside the doument, false otherwise.
         */
        static bool rowExistsInCSVDocumentRowHashSearch(const QList<QString>& row, const CSVDocument& document);

        static QString computeFormatIdentifier(const QList<QString>& headerNames);

#if ARRIVAL_CSVCOMINATION_HAS_MINIMUM_EXECUTION_TIME
        /*!
         * \brief minimumGetCSVCombinedDataExecutionTime The minimun time the getCSVCombinedData function takes to execute.
         * This is used to avoid flashing the loading symbol.
         */
        static constexpr int minimumGetCSVCombinedDataExecutionTime = 400;
#endif

    public:
        CSVCombinedData()
            : m_headerNames()
#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
            , m_headerIndices()
#endif
            , m_rows()
            , m_newAddedCount(0)
            , m_removedCount(0)
        {}

        QString headerHash() const
        {
            if (m_headerNames.empty())
            {
                return QString();
            }

            QString concatenatedString = m_headerNames.join("");
            QCryptographicHash hash(QCryptographicHash::Sha256);
            hash.addData(concatenatedString.toUtf8());
            QByteArray hashData = hash.result();
            QString uniqueHash = QString(hashData.toHex());
            return uniqueHash;
        }

        const QString& formatIdentifier() const
        {
            return m_formatIdentifier;
        }

        /*!
         * \brief headerNames Returns a list with all header names.
         * \return \c QList<QString> with all header names.
         */
        const QList<QString>& headerNames() const
        {
            return m_headerNames;
        }

#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        /*!
         * \brief headerIndices maps the header names to their column indices.
         * \return \c QHash<QString, int> containg the header names and their column indices.
         */
        const QHash<QString, int>& headerIndices() const
        {
            return m_headerIndices;
        }
#endif
        /*!
         * \brief rows All rows
         * \return All rows.
         */
        const QList<JobTableRow>& rows() const
        {
            return m_rows;
        }

        /*!
         * \brief rowCount The row count without the headers.
         * \return The row count without the headers
         */
        int rowCount() const
        {
            return m_rows.count();
        }

        /*!
         * \brief columnCount The column count.
         * \return The column count.
         */
        int columnCount() const
        {
            return m_headerNames.count();
        }

        /*!
         * \brief newAddedCount The count of new added rows.
         * \return The amount of new added rows.
         */
        int newAddedCount() const
        {
            return m_newAddedCount;
        }

        /*!
         * \brief removedCount The amount of removed rows.
         * \return The amount of removed rows.
         */
        int removedCount() const
        {
            return m_removedCount;
        }

        /*!
         * \brief clear Clears the data.
         */
        void clear();

    private:
        QString m_formatIdentifier;

        /*!
         * \brief m_headerNames List containg all the header names.
         */
        QList<QString> m_headerNames;
#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        /*!
         * \brief m_headerIndices maps the header names to their column indices.
         */
        QHash<QString, int> m_headerIndices;
#endif
        /*!
         * \brief m_rows List containg information about the rows.
         */
        QList<JobTableRow> m_rows;

        /*!
         * \brief m_newAddedCount Amount of added rows relative to the old document.
         */
        int m_newAddedCount;

        /*!
         * \brief m_removedCount Amount of removed rows relative to the old document.
         */
        int m_removedCount;
    };
}

#endif // ARRIVAL_CSVHANDLING_H
