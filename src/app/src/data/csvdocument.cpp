// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QDebug>

#include <utility>

#include "qtcsv/reader.h"

#include "data/csvdocument.h"

namespace Arrival::App
{
    CSVDocument::CSVDocument(const QString& path)
        : m_headerNames()
#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        , m_headerIndices()
#endif
        , m_data()
        , m_rowCount(0)
        , m_columnCount(0)
    {
        // Read the actual document.
        QList<QList<QString>> data = QtCSV::Reader::readToList(path);

        const int rowCount = data.count();
        // If no row is present, no data is in the document as a whole.
        // Escape the funtion early in that case.
        if (rowCount <= 0)
        {
            qWarning() << "Empty csv file: " + path;
            return;
        }
        m_rowCount = rowCount - 1;
        m_columnCount = data.at(0).count();

        // The headers are at the first row.
        // Transfer it to the list containg the headers and remove it afterwards.
        m_headerNames = std::move(data.at(0));
        data.removeAt(0);

#if ARRIVAL_CSVDOCUMENT_SUPPORTS_HEADER_INDICES
        // Set up the header indices.
        m_headerIndices.reserve(m_columnCount);
        for (int columnIterator = 0; columnIterator < m_headerNames.count(); columnIterator++)
        {
            m_headerIndices.insert(m_headerNames.at(columnIterator), columnIterator);
        }
#endif

        // Transfer the csv data into the list of lists of strings.
        m_data = std::move(data);
    }

    const QString& CSVDocument::at(int row, int column) const
    {
        return m_data.at(row).at(column);
    }
}
