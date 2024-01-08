// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_CSVDOCUMENT_H
#define ARRIVAL_CSVDOCUMENT_H

#include <QHash>
#include <QList>
#include <QString>

#define ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES 0

namespace Arrival::App
{
    /*!
     * \brief The CSVDocument class stores data found in a .csv file in memory.
     */
    class CSVDocument
    {
    public:

        /*!
         * \brief CSVDocument constructs a new \c CSVDocument from a .csv file
         * \param path Path to the .csv file to construct from.
         */
        CSVDocument(const QString& path);

        /*!
         * \brief headersNames Returns the header names.
         * \return The header names.
         */
        const QList<QString>& headersNames() const
        {
            return m_headerNames;
        }

        /*!
         * \brief data returns the data.
         * \return the data of the csv document.
         */
        const QList<QList<QString>>& data() const
        {
            return m_data;
        }

        /*!
         * \brief rowCount Returns the number of rows in the document.
         * \return The number of rows in the document.
         */
        int rowCount() const
        {
            return m_rowCount;
        }

        /*!
         * \brief columnCount Returns the column count.
         * \return The column count.
         */
        int columnCount() const
        {
            return m_columnCount;
        }

        /*!
         * \brief isEmpty Checks whether the document is empty.
         * \return True if empty, false otherwise.
         */
        bool isEmpty() const
        {
            return m_rowCount <= 0 && m_headerNames.empty();
        }

#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        /*!
         * \brief headerIndices Returns a Hash that maps the hader names to their indices.
         * \return \c QHash containg the header names and their indices.
         */
        const QHash<QString, int>& headerIndices() const
        {
            return m_headerIndices;
        }

        /*!
         * \brief columnIndex Returns the columns index by the header name.
         * \param columnName The header name.
         * \return The index of the header name.
         */
        int columnIndex(const QString& columnName) const
        {
            return m_headerIndices[columnName];
        }
#endif

        /*!
         * \brief at Returns the data in a cell
         * \param row row index of the cell.
         * \param column column index of the cell.
         * \return returns the data in the cell as a \c QString.
         */
        const QString& at(int row, int column) const;

    private:
        /*!
         * \brief headerNames The names of the column headers.
         */
        QList<QString> m_headerNames;

#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        /*!
         * \brief m_columns stores the index of each column by name.
         * May be unused. Keep it here because it might become useful in the future.
         */
        QHash<QString, int> m_headerIndices;
#endif

        /*!
         * \brief m_data actual data of the .csv file stored in memory.
         */
        QList<QList<QString>> m_data;

        /*!
         * \brief m_rowCount Count of rows without the header row.
         */
        int m_rowCount;

        /*!
         * \brief m_columnCount Count of the columns.
         */
        int m_columnCount;
    };
}

#endif // ARRIVAL_CSVDOCUMENT_H
